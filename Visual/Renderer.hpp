#pragma once

#include "Common/Utility/Math/Mat4.hpp"

namespace Visual::Device
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
		static void Init();
		static void Shutdown();

		static void SetDevice( Device::Window* device );
		static void OnWindowResize( const uint32_t width, const uint32_t height );

		static void BeginScene( Camera& camera );
		static void EndScene();

		static void Submit( const std::shared_ptr<Device::Shader>& shader, const std::shared_ptr<Device::VertexArray>& vertex_array, const glm::mat4& model_transform = glm::mat4( 1.f ) );

	private:
		struct SceneData;

		static std::unique_ptr<SceneData> static_scene_data;
	};
}
