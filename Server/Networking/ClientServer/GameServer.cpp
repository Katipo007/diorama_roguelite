#include "GameServer.hpp"

#include "ClientServerCommon/Networking/ClientServer/MessageFactory.hpp"
#include "Server/Networking/ClientServer/ActiveClient.hpp"
#include "Server/Networking/ClientServer/UnauthenticatedClient.hpp"

#include "Common/Utility/Containers.hpp"
#include "Common/Utility/MagicEnum.hpp"
#include "Common/Utility/StringUtility.hpp"

namespace Networking::ClientServer
{
	using ClientFlags = BaseClientConnection::Flags;

	//
	// GameServer
	//

	GameServer::GameServer( yojimbo::Address host_address, const int max_clients )
		: server( yojimbo::GetDefaultAllocator(), DefaultPrivateKey.data(), host_address, MakeConfiguration(), adapter, 0.0 )
		, adapter()
	{
		// Reserve memory
		unauthed_clients.reserve( max_clients );
		active_clients.reserve( max_clients );

		// Connect listeners
		adapter.ServerClientConnected.connect( &GameServer::ClientConnectedHandler, this );
		adapter.ServerClientDisconnected.connect( &GameServer::ClientDisconnectedHandler, this );

		// Start the server
		server.Start( max_clients );
		std::array<char, 256> server_address_buffer{ 0 };
		server.GetAddress().ToString( &server_address_buffer[0], static_cast<int>(std::size( server_address_buffer )) );

		if (!server.IsRunning())
			throw std::runtime_error( "Could not start server on '" + std::string( server_address_buffer.data() ) + "'" );
	}

	GameServer::~GameServer()
	{
		if (server.IsRunning())
			server.Stop();

		ASSERT( !IsRunning() );
	}

	bool GameServer::IsRunning() const noexcept
	{
		return server.IsRunning();
	}

	size_t GameServer::GetConnectedClientCount() const noexcept
	{
		return server.GetNumConnectedClients();
	}

	size_t GameServer::GetMaxClientCount() const noexcept
	{
		return server.GetMaxClients();
	}

	bool GameServer::AcceptClient( UnauthenticatedClient& client )
	{
		if (&client.GetOwner() != this)
			return false;

		const auto client_index = client.GetClientIndex();
		auto unauthed_obj = unauthed_clients.extract( client.GetClientIndex() );
		if (!unauthed_obj)
		{
			LOG_WARN( Server, "Client ({}) isn't in the 'unauthenticated' state", client_index );
			return false;
		}
		ASSERT( client_index == unauthed_obj.key() );

		const auto [accepted_it, success] = active_clients.try_emplace( client_index, unauthed_obj.mapped() );
		if (!success)
		{
			LOG_CRITICAL( Server, "Failed to transition client ({}) to 'active' because: Failed to emplace", client_index );
			server.DisconnectClient( client_index );
			return false;
		}

		LOG_INFO( Server, "Accepted client ({})", client_index );
		return true;
	}

	bool GameServer::RejectClient( UnauthenticatedClient& client )
	{
		if (&client.GetOwner() != this)
			return false;

		if (const auto it = unauthed_clients.find( client.GetClientIndex() ); it != std::end( unauthed_clients ))
		{
			DisconnectClient( it->second, "Login Rejected" );
			return true;
		}

		LOG_WARN( Server, "Client ({}) isn't in the 'unauthenticated' state", client.GetClientIndex() );
		return false;
	}

	void GameServer::DisconnectClient( BaseClientConnection& client, std::optional<std::string> reason )
	{
		if (&client.GetOwner() != this)
			return;

		SendMessage<Messages::ServerClientDisconnect>( client, ChannelType::Unreliable, [&reason]( Messages::ServerClientDisconnect& msg )
			{
				if (reason.has_value())
					StringUtility::StringToArray( *reason, msg.reason );
			} );

		if (reason.has_value())
			LOG_INFO( Server, "Disconnecting client ({}) for reason '{}'", client.GetClientIndex(), reason.value() );
		else
			LOG_INFO( Server, "Disconnecting client ({}) (no reason provided)", client.GetClientIndex() );
		client.SetFlag( ClientFlags::ToBeDisconnected );
	}

	void GameServer::DisconnectAllClients( std::optional<std::string> reason )
	{
		ForAllConnections( [&reason, this]( BaseClientConnection& client )
			{
				DisconnectClient( client, reason );
			} );
	}

	void GameServer::OnFixedUpdate( const PreciseTimestep& ts )
	{
		if (!IsRunning())
		{
			// TODO: should this throw? Do we want the server to be a state where it isn't running??
		}
		else
		{
			server.ReceivePackets();

			ProcessMessages();

			ForAllConnections( [&ts]( BaseClientConnection& client )
				{
					if (!client.TestFlag( ClientFlags::ToBeDisconnected ))
						client.OnFixedUpdate( ts );
				} );

			server.SendPackets();

			// Disconnect clients
			{
				// We push ids to a vector rather than calling server.DisconnectClient() while iterating because the ClientDisconnectedHandler()
				// erases the connection object

				static std::vector<YojimboPlugin::ClientIndex_T> indices_to_disconnect( server.GetMaxClients() );
				indices_to_disconnect.clear();

				ForAllConnections( [this]( BaseClientConnection& client )
					{
						ASSERT( server.IsClientConnected( client.GetClientIndex() ) );
						if (client.TestFlag( ClientFlags::ToBeDisconnected ))
							indices_to_disconnect.push_back( client.GetClientIndex() );
					} );

				for (auto idx : indices_to_disconnect)
					server.DisconnectClient( idx );
			}
		}

		server.AdvanceTime( server.GetTime() + ts.delta );
	}

	void GameServer::ForEachClient( std::invocable<ActiveClient&> auto func, std::predicate<const ActiveClient&> auto predicate )
	{
		for (auto& [index, client] : active_clients)
		{
			if (server.IsClientConnected( index ) && !client.TestFlag( ClientFlags::ToBeDisconnected ))
				func( client );
		};
	}

	bool GameServer::SendMessage( BaseClientConnection& client, ChannelType channel, YojimboPlugin::MessageType_T type, const std::function<void( yojimbo::Message& )>& initialiser )
	{
		if (&client.GetOwner() != this)
			return false;

		const auto client_index = client.GetClientIndex();
		const auto channel_idx = static_cast<YojimboPlugin::ChannelIndex_T>(channel);

		if (!server.IsClientConnected( client_index ) || !server.CanSendMessage( client_index, channel_idx ) || client.TestFlag( ClientFlags::ToBeDisconnected ))
			return false;

		if (auto* message = server.CreateMessage( client_index, type ))
		{
			initialiser( *message );
			server.SendMessage( client_index, channel_idx, message );
			return true;
		}
		else
		{
			LOG_CRITICAL( Server, "Failed to allocate message of type '{}' ({})", MessageFactory::GetMessageName( type ), type );
		}

		return false;
	}

	size_t GameServer::BroadcastMessage( ChannelType channel, YojimboPlugin::MessageType_T type, const std::function<void( ActiveClient&, yojimbo::Message& )>& initialiser, const detail::ActiveClientPredicate_T& inclusion_predicate )
	{
		size_t num_sent = 0;

		for (auto& [idx, client] : active_clients )
		{
			if (!!inclusion_predicate && inclusion_predicate( client ))
			{
				const auto initialiser_wrapper = [&]( yojimbo::Message& outgoing ) { return initialiser( client, outgoing ); };
				if (SendMessage( client, channel, type, initialiser_wrapper ))
					++num_sent;
			}
		}

		return num_sent;
	}

	void GameServer::ClientConnectedHandler( ClientServerAdapter&, YojimboPlugin::ClientIndex_T index )
	{
		ASSERT( FindClientConnection( index ) == nullptr );

		const auto [it, success] = unauthed_clients.try_emplace( index, *this, index );
		LOG_INFO( Server, "Client ({}) connected", index );
	}

	void GameServer::ClientDisconnectedHandler( ClientServerAdapter&, YojimboPlugin::ClientIndex_T index )
	{
		unauthed_clients.erase( index );
		active_clients.erase( index );
		LOG_INFO( Server, "Client ({}) disconnected", index );
	}

	void GameServer::ProcessMessages()
	{
		if (!IsRunning())
			return;

		for (int idx = 0, max_clients = server.GetMaxClients(); idx < max_clients; idx++)
		{
			if (!server.IsClientConnected( idx ))
				continue;

			auto* client = FindClientConnection( idx );
			if (client == nullptr)
			{
				LOG_CRITICAL( Server, "Found a connected client without a connection object, severing" );
				server.DisconnectClient( idx );
				continue;
			}

			const auto ShouldProcess = []( const BaseClientConnection& client ) -> bool { return !client.TestFlag( ClientFlags::ToBeDisconnected ); };
			if (!ShouldProcess( *client ))
				continue;

			for (auto channel : magic_enum::enum_values<ChannelType>())
			{
				const auto channel_idx = static_cast<YojimboPlugin::ClientIndex_T>(channel);
				yojimbo::Message* message = server.ReceiveMessage( idx, channel_idx );
				while (message != NULL && ShouldProcess( *client ))
				{
					const auto message_type = message->GetType();

					const bool handled = client->HandleMessage( *message );
					server.ReleaseMessage( idx, message );
					message = NULL;

					if (!handled)
					{
#ifdef _DEBUG
						using namespace std::string_literals;
						DisconnectClient( *client, "Unhandled message '"s + std::string{ MessageFactory::GetMessageName( message_type ) } + "'" );
#else
						DisconnectClient( *client );
#endif
					}
				}
				// release message in case ShouldProcessPred() returned false
				if (message != NULL)
				{
					server.ReleaseMessage( idx, message );
					message = NULL;
				}
			}
		}
	}

	void GameServer::ForAllConnections( std::invocable<BaseClientConnection&> auto func )
	{
		for (auto& [idx, entry] : unauthed_clients)
			func( entry );
		for (auto& [idx, entry] : active_clients)
			func( entry );
	}

	BaseClientConnection* GameServer::FindClientConnection( YojimboPlugin::ClientIndex_T client_index )
	{
		if (const auto active_it = active_clients.find( client_index ); active_it != std::end( active_clients ))
			return &active_it->second;

		else if (const auto unauthed_it = unauthed_clients.find( client_index ); unauthed_it != std::end( unauthed_clients ))
			return &unauthed_it->second;

		return nullptr;
	}
}