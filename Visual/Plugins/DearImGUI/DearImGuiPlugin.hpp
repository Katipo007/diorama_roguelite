#pragma once

#include "Common/Core/API/InternalAPI.hpp"

#include "Common/Geometry/Size.hpp"

namespace Graphics::API
{
	class DearImGuiPlugin final
		: ::API::InternalAPI
	{
	public:
		DearImGuiPlugin( ::API::SystemAPI& system );
		~DearImGuiPlugin();

		void SetEnabled( const bool enable );
		bool GetEnabled() const noexcept;

		void OnWindowResized( Size<uint32_t> new_window_size );

	protected:
		void Init() override;
		void Shutdown() override;

	private:
		struct Data;
		std::unique_ptr<Data> data;
	};
}
