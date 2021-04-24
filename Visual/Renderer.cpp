#include "Renderer.hpp"

#include "Common/Core/API/VideoAPI.hpp"
#include "Visual/Colour.hpp"
#include "Visual/Graphics/Shader.hpp"
#include "Visual/Graphics/Texture.hpp"
#include "Visual/Graphics/VertexArray.hpp"
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

	Renderer::Renderer( ::API::VideoAPI& video_ )
		: video( video_ )
	{
	}

	Renderer::~Renderer()
	{
	}

	void Renderer::Init()
	{

	}

	void Renderer::Shutdown()
	{
	}

	void Renderer::OnWindowResize( const uint32_t width, const uint32_t height )
	{
		video.SetViewport( Rect<uint32_t>( 0, 0, width, height ) );
	}

	void Renderer::BeginScene( Camera& camera )
	{
		video.SetClearColour( 0x101010FF_rgba );
		video.Clear();

		static_scene_data->view_projection_matrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit( const std::shared_ptr<Graphics::Shader>& shader, const std::shared_ptr<Graphics::VertexArray>& vertex_array, const glm::mat4& model_transform )
	{
		shader->Bind();
		shader->SetMat4( "u_ViewProjection", static_scene_data->view_projection_matrix );
		shader->SetMat4( "u_Model", model_transform );

		vertex_array->Bind();
		video.DrawIndexed( vertex_array );
	}
}
