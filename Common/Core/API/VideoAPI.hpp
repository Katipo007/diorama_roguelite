#pragma once

#include <string>
#include "Common/Geometry/Size.hpp"

namespace Graphics
{
	class Texture;
	class Shader;
	class ShaderDefinition;

	class Window;
	struct WindowDefinition;
}

namespace API
{
	class VideoAPI
	{
	public:
		virtual ~VideoAPI() {}

		virtual void BeginRender() = 0;
		virtual void EndRender() = 0;

		virtual void SetWindow( Graphics::WindowDefinition&& definition ) = 0;
		virtual const Graphics::Window& GetWindow() const = 0;
		virtual bool HasWindow() const = 0;
		virtual void SetVSync( const bool ) {}

		virtual std::unique_ptr<Graphics::Texture> CreateTexture( Size<uint16_t> size ) = 0;
		virtual std::unique_ptr<Graphics::Shader> CreateShader( const Graphics::ShaderDefinition& definition ) = 0;

		virtual std::string_view GetShaderLanguage() const = 0;

		virtual void* GetImplementationPointer( std::string_view id ) { (void)id; return nullptr; }
	};
}
