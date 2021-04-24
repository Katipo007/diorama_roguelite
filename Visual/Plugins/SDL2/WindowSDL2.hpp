#pragma once

#include "Visual/Window.hpp"

struct SDL_WindowEvent;
struct SDL_Window;

namespace Graphics::API
{
	class SystemSDL2;

	class WindowSDL2
		: public ::Graphics::Window
	{
		friend class SystemSDL2;

	public:
		explicit WindowSDL2( SDL_Window* sdl_window );
		virtual ~WindowSDL2();

		void Destroy();

		WindowSDL2( const WindowSDL2& ) = delete;
		WindowSDL2( WindowSDL2&& ) = delete;
		WindowSDL2 operator=( const WindowSDL2& ) = delete;
		WindowSDL2 operator=( WindowSDL2&& ) = delete;

		virtual Size<uint32_t> GetSize() const override;
		virtual void SetSize( const Size<uint32_t> new_size ) override;

		virtual void SetMode( const WindowMode ) override;
		virtual WindowMode GetMode() const override;
		virtual void SetState( const WindowState ) override;
		virtual WindowState GetState() const override;

		virtual void Show() override;
		virtual void Hide() override;

		virtual void SetVSync( const bool ) override;
		virtual void SwapBuffers() override;

		virtual void* GetNativeHandle() override;
		virtual std::string_view GetNativeHandleType() const override;
		uint32_t GetSDLWindowId() const;
		SDL_Window* GetSDLWindow() const { return sdl_window; }

	protected:
		void HandleEvent( const SDL_WindowEvent& e );

	protected:
		SDL_Window* sdl_window = nullptr;
	};
}
