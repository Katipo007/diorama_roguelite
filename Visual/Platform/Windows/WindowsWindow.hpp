#if defined(PLATFORM_WINDOWS)

#pragma once

#include "Visual/Device/Window.hpp"

struct SDL_Window;

namespace Visual::Device
{
	class GraphicsContext;

	class WindowsWindow
		: public Window
	{
	public:
		explicit WindowsWindow( const WindowCreationProperties properties );
		virtual ~WindowsWindow();

		WindowsWindow( const WindowsWindow& ) = delete;
		WindowsWindow( WindowsWindow&& ) = delete;
		WindowsWindow operator=( const WindowsWindow& ) = delete;
		WindowsWindow operator=( WindowsWindow&& ) = delete;

		virtual uint32_t GetWidth() const override;
		virtual uint32_t GetHeight() const override;

		virtual void SetEventCallback( const EventCallbackFunc_t& callback ) override;

		virtual void* GetNativeWindow() const override;
		virtual RendererAPIs::API GetRendererAPIType() const override;
		virtual RendererAPI& GetRendererAPI() const override;

	protected:
		virtual void OnUpdateBegin() override;
		virtual void OnUpdateEnd() override;

	protected:
		SDL_Window* window = NULL;
		std::unique_ptr< GraphicsContext > graphics_context;
		std::unique_ptr< RendererAPI > renderer_api;

		EventCallbackFunc_t event_callback;
	};
}

#endif // PLATFORM_WINDOWS
