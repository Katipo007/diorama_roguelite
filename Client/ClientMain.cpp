
#include <iostream>

#include "Visual/Device/Window.hpp"
#include "Visual/Renderer.hpp"
#include "Visual/Input/Input.hpp"
#include "Visual/Events/ApplicationEvents.hpp"
#include "Visual/Events/KeyboardEvents.hpp"
#include "Visual/DearImGui/DearImGui.hpp"

#include "Common/Core/Base.hpp"
#include "Common/Utility/OsAbstraction.hpp"
#include "Common/Utility/Timestep.hpp"

#include "Client/ClientGame.hpp"
#include "ClientServerCommon/Vendor/Wrappers/Networking.hpp"

std::unique_ptr<Visual::Device::Window> main_window;
std::unique_ptr<Game::ClientGame> client_game;

bool running = true;
bool user_wants_to_exit = false;

namespace VD = Visual::Device;

static void WindowEventHandler( Visual::Device::Event& application_event )
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

			Visual::Renderer::OnWindowResize( e.GetWidth(), e.GetHeight() );

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

static int YojimboLoggingRoute( const char* fmt, ... )
{
	char buffer[4 * 1024];
	va_list args;
	va_start( args, fmt );
	vsprintf_s( buffer, fmt, args );
	va_end( args );
	const size_t length = strlen( buffer );
	if (buffer[length - 1] == '\n')
		buffer[length - 1] = '\0';

	LOG_INFO( Client, "[yojimbo] {}", buffer );
	return 0;
}

int main( int argc, char** argv )
{
	// TODO: use application parameters
	(void)argc;
	(void)argv;

	Logging::InitDefaultClientSinks();

	LOG_INFO( Application, "Client starting" );
	
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
	// Setup Yojimbo
	//
	{
		if (!InitializeYojimbo())
		{
			LOG_CRITICAL( Client, "Critical Error: failed to initialize Yojimbo!" );
			return 1;
		}

#ifdef _DEBUG
		yojimbo_log_level( YOJIMBO_LOG_LEVEL_INFO );
#endif
		yojimbo_set_printf_function( YojimboLoggingRoute );
	}

	//
	// Setup window
	//
	{
		LOG_INFO( Application, "Creating application window" );
		Visual::Device::Window::WindowCreationProperties window_props;
		window_props.title = "Client";
		window_props.width = 640;
		window_props.height = 480;
		window_props.renderer_api = RendererAPIs::API::OpenGL; // TODO: determine this automatically and/or pull from configuration file

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
			LOG_CRITICAL( Application, "Error creating window: {0}", e.what() );
			OS::ShowMessageBox( "Failed to create the application window", "Error", OS::MessageBoxTypes::Error );
			main_window.reset();
			return -1;
		}

		ASSERT( main_window );
	}

	Visual::Renderer::Init();
	Visual::Renderer::SetDevice( main_window.get() );

	//
	// setup managers
	//
	InputManager::SetWindowHandle( main_window->GetNativeWindow() );
	
	client_game.reset( new Game::ClientGame() );


	// ============================================
	// main loop
	// ============================================
	// TODO: refactor to use fixed timesteps

	LOG_INFO( Application, "Application setup complete, starting main loop" );
	auto last_frame_time = main_window->GetApplicationTime();
	while (running)
	{
		const auto time = main_window->GetApplicationTime();
		PreciseTimestep timestep( time,  time - last_frame_time );
		last_frame_time = time;

		main_window->OnUpdateBegin();
#ifdef DEARIMGUI_ENABLED
		if (DearImGui::IsEnabled())
			ImGui::NewFrame();
#endif

		client_game->OnFrame( timestep );

#ifdef DEARIMGUI_ENABLED
		if (DearImGui::IsEnabled())
		{
			client_game->OnDearImGuiFrame();

			ImGui::Render();
		}
#endif
		main_window->OnUpdateEnd();

		user_wants_to_exit |= client_game->ShouldExit();

		if (user_wants_to_exit) // TODO: any other pre-conditions here
			running = false;
	}

	LOG_INFO( Application, "Application loop finished, exiting" );

	//
	// clear
	//
	{
		client_game.reset();
		Visual::Renderer::Shutdown();
		main_window.reset();
		InputManager::SetWindowHandle( NULL );
	}

	//
	// Shutdown Yojimbo
	//
	ShutdownYojimbo();

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

	LOG_INFO( Application, "Client finished" );
	return 0;
}
