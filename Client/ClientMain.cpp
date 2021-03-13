#include "Precompiled/pch.hpp"

#include <iostream>

#include "Visual/Device/Window.hpp"
#include "Visual/Device/RendererCommand.hpp"
#include "Visual/Input/Input.hpp"
#include "Visual/Events/ApplicationEvents.hpp"
#include "Visual/Events/KeyboardEvents.hpp"
#include "Visual/DearImGui/DearImGui.hpp"

#include "Common/Core/Base.hpp"
#include "Common/Utility/OsAbstraction.hpp"
#include "Common/Utility/Timestep.hpp"

#include "States/ClientStateManager.hpp"

std::unique_ptr<Visual::Device::Window> main_window;
std::unique_ptr<States::ClientStateManager> state_manager;

bool running = true;
bool user_wants_to_exit = false;

namespace VD = Visual::Device;

void WindowEventHandler( Visual::Device::Event& application_event )
{
	Visual::Device::EventDispatcher d( application_event );
	d.Dispatch< VD::ApplicationEvents::WindowClose >( [&]( VD::ApplicationEvents::WindowClose& e ) -> bool
		{
			(void)e;
			user_wants_to_exit = true;
			return false;
		} );

	d.Dispatch< VD::ApplicationEvents::WindowResized >( [&]( VD::ApplicationEvents::WindowResized& e ) -> bool
		{
#ifdef DEARIMGUI_ENABLED
			ImGuiIO& io = ImGui::GetIO();
			io.DisplaySize = ImVec2( (float)e.GetSender().GetWidth(), (float)e.GetSender().GetHeight() );
#endif

			return false;
		} );

	d.Dispatch< VD::KeyboardEvents::KeyPressed >( [&]( VD::KeyboardEvents::KeyPressed& e ) -> bool
		{
			bool handled = false;
#ifdef DEARIMGUI_ENABLED
			if (e.GetKeyCode() == Input::Key::F2 && e.GetRepeatCount() == 0)
			{
				DearImGui::SetEnabled( !DearImGui::IsEnabled() );
				handled |= true;
			}
#endif

			return handled;
		} );
}

int main( int argc, char** argv )
{
	// TODO: use application parameters
	(void)argc;
	(void)argv;

	//
	// initialise the logging system
	//
	Logging::Log::Init( Logging::Type::Client );
	auto& logger = Logging::Log::GetClientLogger(); (void)logger;

	LOG_INFO( "Client starting" );

#ifdef DEARIMGUI_ENABLED
	//
	// setup imgui
	//
	ImGuiContext* imgui_context = NULL;
	{
		IMGUI_CHECKVERSION();
		imgui_context = ImGui::CreateContext();
		ImGui::SetCurrentContext( imgui_context );

		ImGuiIO& io = ImGui::GetIO(); (void)io;
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

		io.IniFilename = NULL; // don't save settings
		ImGui::StyleColorsDark();
	}
#endif

	//
	// Setup window
	//
	{
		LOG_INFO( "Creating application window" );
		Visual::Device::Window::WindowCreationProperties window_props;
		window_props.title = "Client";
		window_props.width = 640;
		window_props.height = 480;
		window_props.renderer_api = RendererAPIs::OpenGL; // TODO: determine this automatically and/or pull from configuration file

#ifndef _DEBUG
		window_props.fullscreen = true;
#endif

		try
		{
			main_window = VD::Window::Create( window_props );
			main_window->SetEventCallback( &WindowEventHandler );
		}
		catch (std::runtime_error& e)
		{
			LOG_CRITICAL( "Error creating window: {0}", e.what() );
			OS::ShowMessageBox( "Failed to create the application window", "Error", OS::MessageBoxTypes::Error );
			main_window.reset();
			return -1;
		}

		ASSERT( main_window );
	}

	VD::RendererCommand::SetActiveContext( *main_window );

	//
	// setup managers
	//
	InputManager::SetWindowHandle( main_window->GetNativeWindow() );
	state_manager = std::make_unique<States::ClientStateManager>();


	// ============================================
	// main loop
	// ============================================
	LOG_INFO( "Application setup complete, starting main loop" );
	float last_frame_time = static_cast<float>(clock()) / CLOCKS_PER_SEC;
	while (running)
	{
		const float time = static_cast<float>(clock()) / CLOCKS_PER_SEC;
		Timestep timestep = time - last_frame_time;
		last_frame_time = time;
		ASSERT( timestep.GetSeconds() >= 0 );

		main_window->OnUpdateBegin();
#ifdef DEARIMGUI_ENABLED
		if (DearImGui::IsEnabled())
			ImGui::NewFrame();
#endif

		VD::RendererCommand::Clear();

		state_manager->OnFrame( timestep );

#ifdef DEARIMGUI_ENABLED
		if (DearImGui::IsEnabled())
			ImGui::Render();
#endif
		main_window->OnUpdateEnd();

		if (user_wants_to_exit) // TODO: any other pre-conditions here
			running = false;
	}

	LOG_INFO( "Application loop finished, exiting" );

	//
	// clear
	//
	{
		state_manager.reset();
		VD::RendererCommand::ClearActiveContext();
		main_window.reset();
		InputManager::SetWindowHandle( NULL );
	}

#ifdef DEARIMGUI_ENABLED
	//
	// shutdown imgui
	//
	if (imgui_context != NULL)
	{
		ImGui::DestroyContext( imgui_context );
		imgui_context = NULL;
	}
#endif

	LOG_INFO( "Client finished" );
	return 0;
}
