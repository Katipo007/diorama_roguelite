#pragma once

#include <string>

#include "Common/Utility/Timestep.hpp"
#include "Common/Utility/NonCopyable.hpp"

#include "ClientStates.hpp"

namespace States
{
	class ClientStateManager;

	class ClientState
		: public NonCopyable
	{
		friend class ClientStateManager;

	public:
		explicit ClientState( const ClientStates type );
		virtual ~ClientState();

		virtual std::string_view GetName() const = 0;
		ClientStates GetType() const { return type; }

		bool IsEnabled() const;
		void Enable();
		void Disable();

	protected:
		virtual void OnAttached() {}  ///< Called when connected to the state manager
		virtual void OnDetached() {}  ///< Called when disconnected from the state manager

		virtual void OnEnabled() {}
		virtual void OnDisabled() {}

		virtual void OnFrame( const Timestep& ) {}  ///< Only gets called if enabled
		virtual void OnDearImGuiRender() {}  ///< Only gets called if enabled

		ClientStateManager* GetClientStateManager() { return manager; }
		const ClientStateManager* GetClientStateManager() const { return manager; }

	private:
		ClientStateManager* manager = nullptr;
		bool is_enabled = false;
		const ClientStates type;
	};
}
