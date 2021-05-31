#pragma once

#include "Common/Core/API/DearImGuiAPI.hpp"

#include "Common/Geometry/Size.hpp"

namespace API
{
	class SystemAPI;
	class VideoAPI;
}

namespace Graphics::API
{
	class DearImGuiPlugin final
		: public ::API::DearImGuiAPI
	{
		friend class ::Core;

	public:
		DearImGuiPlugin( ::API::SystemAPI& system, ::API::VideoAPI& video );
		~DearImGuiPlugin();

		void SetEnabled( const bool enable ) override;
		bool GetEnabled() const noexcept override { return enabled; }

		bool WantsToCaptureKeyboard() const noexcept override;
		bool WantsToCaptureMouse() const noexcept override;

		std::string_view GetName() const noexcept override { return "DearImGui Plugin"; }

	protected:
		void Init() override;
		void Shutdown() override;

		void OnVariableUpdate( const PreciseTimestep&, const StepType ) override;
		void OnRender( const PreciseTimestep&, const StepType ) override;

		void ProcessSystemEvent( void* e ) override;
		void OnWindowResized( Size<uint32_t> new_window_size ) override;

	private:
		::API::SystemAPI& system;
		::API::VideoAPI& video;

		struct Data;
		std::unique_ptr<Data> data;
		bool enabled = false;
	};
}
