#include "Precompiled/pch.hpp"
#include "Window.hpp"

#if defined(PLATFORM_WINDOWS)
#	include "Visual/Platform/Windows/WindowsWindow.hpp"
#else
#	error "No Visual::Window implementation for this platform"
#endif

namespace Visual::Device
{
	std::unique_ptr<Window> Window::Create( const WindowCreationProperties properties )
	{
#if defined(PLATFORM_WINDOWS)
		return std::make_unique<WindowsWindow>( properties );
#else
		ASSERT( "Window creation failed, Visual::Window::Create() couldn't determine a type of window to create" );
		return std::unique_ptr<Window>();
#endif
	}
}
