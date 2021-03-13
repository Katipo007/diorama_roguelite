#pragma once

#include <functional>
#include <string>

#include "Visual/RendererAPIs.hpp"

//
// forward declarations
//
int main( int, char** );

namespace Visual::Device
{
	class Event;

	class RendererAPI;

	class Window
	{
		friend int ::main( int, char** );

	public:
		using EventCallbackFunc_t = std::function<void( Event& )>;

		struct WindowCreationProperties
		{
			std::string title = "";
			uint32_t width = 0, height = 0;
			RendererAPIs::API renderer_api = RendererAPIs::NumAPIs;
			bool fullscreen = false;
		};

	public:
		virtual ~Window() = default;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void SetEventCallback( const EventCallbackFunc_t& callback ) = 0;

		virtual void* GetNativeWindow() const = 0;
		virtual RendererAPIs::API GetRendererAPIType() const = 0;
		virtual RendererAPI& GetRendererAPI() const = 0;

	protected:
		virtual void OnUpdateBegin() = 0;
		virtual void OnUpdateEnd() = 0;

		// Made accessable to main
		static std::unique_ptr< Window > Create( const WindowCreationProperties properties );
	};
}
