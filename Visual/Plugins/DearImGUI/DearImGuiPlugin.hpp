#pragma once

#include "Common/Core/API/BaseAPI.hpp"

#include "Common/Geometry/Size.hpp"

namespace API { class SystemAPI; }

namespace Graphics::API
{
	class DearImGuiPlugin final
		: ::API::BaseAPI
	{
	public:
		DearImGuiPlugin( ::API::SystemAPI& system );
		~DearImGuiPlugin();

		void SetEnabled( const bool enable );
		bool GetEnabled() const noexcept { return enabled; }

		void OnWindowResized( Size<uint32_t> new_window_size );

	protected:
		void Init() override;
		void Shutdown() override;

	private:
		struct Data;
		std::unique_ptr<Data> data;
		bool enabled = false;
	};
}
