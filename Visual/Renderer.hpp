#pragma once

#include "Common/Utility/Math/Mat4.hpp"

namespace API { class VideoAPI; }

namespace Graphics
{
	class Shader;
	class VertexArray;
	class Window;
}

namespace Visual
{
	class Camera;

	// 3D renderer interface
	class Renderer
	{
	public:
		explicit Renderer( ::API::VideoAPI& video );
		virtual ~Renderer();

		void Init();
		void Shutdown();

		void OnWindowResize( const uint32_t width, const uint32_t height );

		void BeginScene( Camera& camera );
		void EndScene();

		void Submit( const std::shared_ptr<Graphics::Shader>& shader, const std::shared_ptr<Graphics::VertexArray>& vertex_array, const glm::mat4& model_transform = glm::mat4( 1.f ) );

	private:
		::API::VideoAPI& video;

		struct SceneData;
		static std::unique_ptr<SceneData> static_scene_data;
	};
}
