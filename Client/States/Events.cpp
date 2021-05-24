#include "Events.hpp"

#include "Client/Networking/ClientServer/ServerConnection.hpp"
#include "Common/Utility/StringUtility.hpp"

namespace ClientStates
{
	ConnectedToServerEvent::ConnectedToServerEvent( Networking::ClientServer::ServerConnection& connection )
		: connection( connection )
	{
	}

	ConnectingToServerEvent::ConnectingToServerEvent( Networking::ClientServer::ServerConnection& connection )
		: connection( connection )
	{
	}

	DisconnectedFromServerEvent::DisconnectedFromServerEvent( Networking::ClientServer::ServerConnection& connection_, std::string_view reason_ )
		: connection( connection_ )
	{
		StringUtility::StringToArray( reason_, this->reason );
	}

	DearImGuiFrameEvent::DearImGuiFrameEvent( ::API::DearImGuiAPI& dearimgui_ )
		: dearimgui_api( dearimgui_ )
	{
	}

	FrameEvent::FrameEvent( PreciseTimestep step )
		: precise_timestep( step )
		, timestep( step )
	{
	}

	ServerMessageEvent::ServerMessageEvent( Networking::ClientServer::ServerConnection& connection, const yojimbo::Message& message )
		: connection( connection )
		, message( message )
	{
	}

	void ServerMessageEvent::FlagAsHandled() const
	{
		if (auto* non_const_this = const_cast<ServerMessageEvent*>(this))
			non_const_this->handled = true;

	}
}
