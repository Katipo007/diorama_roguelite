#include "Renderer.hpp"

#include "Visual/Device/Shader.hpp"
#include "Visual/Device/Texture.hpp"
#include "Visual/Device/VertexArray.hpp"
#include "Visual/Device/RendererCommand.hpp"
#include "Camera.hpp"

namespace Visual
{
	// Static renderer data
	std::unique_ptr<Renderer::SceneData> Renderer::static_scene_data = std::make_unique<Renderer::SceneData>();

	struct Renderer::SceneData
	{
		glm::mat4 view_projection_matrix;
	};

	///
	/// Renderer
	/// 

	void Renderer::Init()
	{

	}

	void Renderer::Shutdown()
	{
	}

	void Renderer::SetDevice( Device::Window* device )
	{
		if (device)
			Device::RendererCommand::SetActiveContext( *device );
		else
			Device::RendererCommand::ClearActiveContext();
	}

	void Renderer::OnWindowResize( const uint32_t width, const uint32_t height )
	{
		Device::RendererCommand::SetViewport( 0, 0, width, height );
	}

	void Renderer::BeginScene( Camera& camera )
	{
		Device::RendererCommand::SetClearColour( 0x101010FF_rgba );
		Device::RendererCommand::Clear();

		static_scene_data->view_projection_matrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit( const std::shared_ptr<Device::Shader>& shader, const std::shared_ptr<Device::VertexArray>& vertex_array, const glm::mat4& model_transform )
	{
		shader->Bind();
		shader->SetMat4( "u_ViewProjection", static_scene_data->view_projection_matrix );
		shader->SetMat4( "u_Model", model_transform );

		vertex_array->Bind();
		Device::RendererCommand::DrawIndexed( vertex_array );
	}
}
