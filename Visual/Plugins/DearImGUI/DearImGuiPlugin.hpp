#pragma once

#include "Common/Core/API/DearImGuiAPI.hpp"

#include "Common/Geometry/Size.hpp"

namespace API { class SystemAPI; }

namespace Graphics::API
{
	class DearImGuiPlugin final
		: public ::API::DearImGuiAPI
	{
		friend class ::Core;

	public:
		DearImGuiPlugin( ::API::SystemAPI& system );
		~DearImGuiPlugin();

		virtual void SetEnabled( const bool enable ) override;
		virtual bool GetEnabled() const noexcept override { return enabled; }

		virtual std::string_view GetName() const noexcept override { return "DearImGui Plugin"; }

	protected:
		void Init() override;
		void Shutdown() override;

		virtual void OnWindowResized( Size<uint32_t> new_window_size ) override;

	private:
		struct Data;
		std::unique_ptr<Data> data;
		bool enabled = false;
	};
}
