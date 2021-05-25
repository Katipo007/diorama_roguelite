#pragma once

#include <functional>
#include <optional>
#include <string>

#include "Common/Geometry/Size.hpp"

namespace Graphics
{
	enum class WindowState
	{
		Normal,
		Minimised,
		Maximised,
	};

	enum class WindowMode
	{
		None,
		Window,
		Fullscreen,
		BorderlessFullscreen,
	};

	struct WindowDefinition
	{
		std::optional<std::string> title;
		Size<uint32_t> size;
		WindowState state;
		WindowMode mode;
	};

	class Window
	{
	public:
		virtual ~Window() = default;

		virtual void SetSize( const Size<uint32_t> new_size ) = 0;
		virtual Size<uint32_t> GetSize() const = 0;
		virtual void SetMode( const WindowMode ) = 0;
		virtual WindowMode GetMode() const = 0;
		virtual void SetState( const WindowState ) = 0;
		virtual WindowState GetState() const = 0;

		virtual void Show() = 0;
		virtual void Hide() = 0;
		virtual void SetVSync( const bool ) = 0;

		virtual void SwapBuffers() = 0;

		virtual void* GetNativeHandle() { return nullptr; }
		virtual std::string_view GetNativeHandleType() const { return ""; }
	};
}
