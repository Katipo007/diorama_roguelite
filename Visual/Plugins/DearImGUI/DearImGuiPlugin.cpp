#include "DearImGuiPlugin.hpp"

#include "Visual/DearImGui/DearImGui.hpp"

namespace Graphics::API
{
	/// 
	/// Data
	/// 

	struct DearImGuiPlugin::Data
	{
		ImGuiContext* imgui_context = NULL;
	};


	/// 
	/// DearImGuiPlugin
	/// 
	
	DearImGuiPlugin::DearImGuiPlugin( ::API::SystemAPI& system )
		: data( std::make_unique<DearImGuiPlugin::Data>() )
	{
		(void)system; // TODO: hookup to window events

		IMGUI_CHECKVERSION();
	}

	DearImGuiPlugin::~DearImGuiPlugin() = default;

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
		if (data->imgui_context != NULL)
		{
			ImGui::DestroyContext( data->imgui_context );
			data->imgui_context = NULL;
		}
	}

	void DearImGuiPlugin::SetEnabled( const bool enable )
	{
		DearImGui::SetEnabled( !DearImGui::IsEnabled() );
	}

	void DearImGuiPlugin::OnWindowResized( Size<uint32_t> new_window_size )
	{
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2( static_cast<float>( new_window_size.width ), static_cast<float>( new_window_size.height ) );
	}
}