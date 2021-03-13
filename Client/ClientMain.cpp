#include "Precompiled/pch.hpp"

#include "Common/Core/Base.hpp"
#include "Visual/Input/Input.hpp"
#include "Visual/Device/Window.hpp"

#include <iostream>

std::unique_ptr<Visual::Device::Window> main_window;

int main( int argc, char** argv )
{
	// initialise the logging system
	Logging::Log::Init( Logging::Type::Client );
	auto& logger = Logging::Log::GetClientLogger();

	LOG_INFO( "Client starting" );

	LOG_INFO( "Creating window" );
	Visual::Device::Window::WindowCreationProperties window_props;
	window_props.title = "Client";
	window_props.fullscreen = false;
	window_props.renderer_api = RendererAPIs::OpenGL;
	window_props.width = 640;
	window_props.height = 480;

	main_window = Visual::Device::Window::Create( window_props );

	// pass window handle to the input manager
	InputManager::SetWindowHandle( main_window->GetNativeWindow() );

	main_window->OnUpdateBegin();

	main_window->OnUpdateEnd();

	LOG_INFO( "Client finished" );
	return 0;
}
