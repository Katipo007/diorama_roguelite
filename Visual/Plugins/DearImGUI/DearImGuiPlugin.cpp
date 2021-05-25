#include "DearImGuiPlugin.hpp"

#include "Common/Core/API/SystemAPI.hpp"
#include "Common/Core/API/VideoAPI.hpp"
#include "Common/Core/Graphics/Window.hpp"

#include "Visual/Plugins/OpenGL/OpenGLHeader.hpp"
#include "Visual/DearImGui/DearImGui.hpp"
#define IMGUI_IMPL_OPENGL_LOADER_GLEW
#include "Visual/Vendor/dearimgui/backends/imgui_impl_opengl3.h"
#include "Visual/Vendor/dearimgui/backends/imgui_impl_sdl.h"

#include "Visual/Plugins/SDL2/WindowSDL2.hpp"

using namespace std::string_literals;

namespace
{
	enum class Impl
	{
		None,
		SDL2_OpenGL
	};
}

namespace Graphics::API
{
	/// 
	/// Data
	/// 

	struct DearImGuiPlugin::Data
	{
		ImGuiContext* imgui_context = NULL;
		Impl impl = Impl::None;
		bool implementation_initalised = false;
	};


	/// 
	/// DearImGuiPlugin
	/// 
	
	DearImGuiPlugin::DearImGuiPlugin( ::API::SystemAPI& system_, ::API::VideoAPI& video_ )
		: data( std::make_unique<DearImGuiPlugin::Data>() )
		, system( system_ )
		, video( video_ )
	{
#ifdef _DEBUG
		enabled = true;
#endif

		// TODO: hookup to window events

		IMGUI_CHECKVERSION();


		auto system_plugin_name = system.GetName();
		auto video_plugin_name = video.GetName();

		if ((system_plugin_name == "SDL2") && (video_plugin_name == "OpenGL"))
			data->impl = Impl::SDL2_OpenGL;
		else
			throw std::runtime_error( "DearImGui plugin currently doesn't support the combination of '"s + std::string( system_plugin_name ) + "' system plugin and '"s + std::string( video_plugin_name ) + "' video plugin" );
	}

	DearImGuiPlugin::~DearImGuiPlugin() = default;

	void DearImGuiPlugin::OnVariableUpdate( const PreciseTimestep&, const StepType type )
	{
		if (!enabled)
			return;

		switch (type)
		{
		case StepType::PreGameStep:
		{
			if (!data->implementation_initalised)
			{
				ASSERT( video.HasWindow() );
				switch (data->impl)
				{
				case Impl::SDL2_OpenGL:
				{
					auto* sdl_window = static_cast<const WindowSDL2&>(video.GetWindow()).GetSDLWindow();
					ASSERT( sdl_window );
					ImGui_ImplSDL2_InitForOpenGL( sdl_window, NULL /*temp*/ );
					ImGui_ImplOpenGL3_Init( "#version 410" );
					break;
				}
				}
				data->implementation_initalised = true;
			}

			if (data->implementation_initalised)
			{
				switch (data->impl)
				{
				case Impl::SDL2_OpenGL:
				{
					auto* sdl_window = static_cast<const WindowSDL2&>(video.GetWindow()).GetSDLWindow();
					ASSERT( sdl_window );
					ImGui_ImplOpenGL3_NewFrame();
					ImGui_ImplSDL2_NewFrame( sdl_window );


					const auto window_size = video.GetWindow().GetSize();
					auto& io = ImGui::GetIO();
					io.DisplaySize = ImVec2( (float)window_size.width, (float)window_size.height );
					break;
				}
				}
			}

			ImGui::NewFrame();
			break;
		}

		case StepType::PostGameStep:
		{
			ImGui::Render();
			break;
		}
		}
	}

	void DearImGuiPlugin::OnRender( const PreciseTimestep&, const StepType type )
	{
		if (!enabled || (type != StepType::PostGameStep))
			return;

		if (data->implementation_initalised)
		{
			auto* draw_data = ImGui::GetDrawData();
			if (draw_data)
			{
				switch (data->impl)
				{
				case Impl::SDL2_OpenGL:
					ImGui_ImplOpenGL3_RenderDrawData( draw_data );
					break;
				}
			}
			else
				LOG_WARN( Application, "DearImGui draw data was NULL" );
		}
	}

	void DearImGuiPlugin::Init()
	{
		data->imgui_context = ImGui::CreateContext();
		ImGui::SetCurrentContext( data->imgui_context );

		ImGuiIO& io = ImGui::GetIO(); (void)io;
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

		io.IniFilename = NULL; // don't save settings
		ImGui::StyleColorsDark();
	}

	void DearImGuiPlugin::Shutdown()
	{
		if (data->implementation_initalised)
		{
			switch (data->impl)
			{
			case Impl::SDL2_OpenGL:
				ImGui_ImplOpenGL3_Shutdown();
				ImGui_ImplSDL2_Shutdown();
				break;
			}
			data->implementation_initalised = false;
		}

		if (data->imgui_context != NULL)
		{
			ImGui::DestroyContext( data->imgui_context );
			data->imgui_context = NULL;
		}
	}

	void DearImGuiPlugin::SetEnabled( const bool enable_ )
	{
		enabled = enable_;
	}

	void DearImGuiPlugin::ProcessSystemEvent( void* raw_event )
	{
		if (data->implementation_initalised)
		{
			switch (data->impl)
			{
			case Impl::SDL2_OpenGL:
			{
				ImGui_ImplSDL2_ProcessEvent( static_cast<SDL_Event*>(raw_event) );
				break;
			}
			}
		}
	}

	void DearImGuiPlugin::OnWindowResized( Size<uint32_t> new_window_size )
	{
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2( static_cast<float>( new_window_size.width ), static_cast<float>( new_window_size.height ) );
	}
}

///
/// Compile the ImGui implementations
///
#pragma warning(push, 0)
#	if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#		define _CRT_SECURE_NO_WARNINGS
#		pragma warning(disable : 4996)
#	endif

#	include "Visual/Vendor/dearimgui/backends/imgui_impl_opengl3.cpp"
#	include "Visual/Vendor/dearimgui/backends/imgui_impl_sdl.cpp"
#pragma warning(pop)
