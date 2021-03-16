#pragma once

#include <functional>
#include <memory>
#include <type_traits>
#include <vector>

#include "Common/Utility/Timestep.hpp"
#include "Common/Utility/NonCopyable.hpp"

#include "ClientStates.hpp"

namespace Game
{
	class ClientGame;
}

namespace States
{
	class ClientState;

	class ClientStateManager final
		: public NonCopyable
	{
		friend Game::ClientGame;
		using StateCollection_T = std::vector< std::unique_ptr<ClientState> >;

	public:
		explicit ClientStateManager();
		~ClientStateManager();

		ClientState* Get( const ClientStates type );
		template<class T>
		T* Get()
		{
			static_assert(std::is_base_of<ClientState, T>::value, "Type must be a derivative of States::ClientState");
			return static_cast<T*>(Get( T::GetType() ));
		}

		const ClientState* Get( const ClientStates type ) const;
		template<class T>
		const T* Get() const
		{
			static_assert(std::is_base_of<ClientState, T>::value, "Type must be a derivative of States::ClientState");
			return static_cast<const T*>(Get( T::GetStaticType() ));
		}

		void ForEach( const std::function<void( ClientState& )>& func );
		void ForEach( const std::function<void( const ClientState& )>& func ) const;

		void BringToTop( ClientState& state );
		void SendToBack( ClientState& state );

		void OnFrame( const Timestep& ts );

	protected:
		void AddState( std::unique_ptr<ClientState> state, const bool start_enabled = false );
		void RemoveState( ClientState& state );
		void EnableState( ClientState& state );
		void DisableState( ClientState& state );

	private:
		StateCollection_T states;
	};
}
