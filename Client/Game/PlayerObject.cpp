#include "PlayerObject.hpp"

#include "Visual/Device/RendererCommand.hpp"
#include "Visual/Device/RendererAPI.hpp"
#include "Visual/Device/Shader.hpp"
#include "Visual/Device/Texture.hpp"
#include "Visual/Device/VertexArray.hpp"

#include "Visual/WorldSpace.hpp"

namespace Game
{
	PlayerObject::PlayerObject()
	{
		auto& renderer = Visual::Device::RendererCommand::GetRendererAPI();
		shader = renderer.CreateShader( "Shaders/DefaultTexture.glsl" );

		texture = renderer.CreateTexture2D( 1, 1 );
		uint32_t white_texture_data = 0xffffffff;
		texture->SetData( &white_texture_data, sizeof( uint32_t ) );

		CreateModel();
	}

	PlayerObject::~PlayerObject()
	{
	}

	void PlayerObject::CreateModel()
	{
		auto& renderer = Visual::Device::RendererCommand::GetRendererAPI();

		const auto vertices = std::vector<float>{
				+0.f, -4.f, +0.f, 1, 0, 0, 1, 0, 0, 0,
				-4.f, +4.f, +0.f, 0, 1, 0, 1, 0, 0, 0,
				+4.f, +4.f, +0.f, 0, 0, 1, 1, 0, 0, 0,

				+1.f, +0.f, +0.f, 1, 1, 1, 1, 0, 0, 0,
				+0.f, +0.f, -2.f, 1, 1, 1, 1, 0, 0, 0,
				-1.f, +0.f, +0.f, 1, 1, 1, 1, 0, 0, 0,
				+0.f, +1.f, +0.f, 1, 0, 1, 1, 0, 0, 0,
				+0.f, +0.f, -2.f, 1, 0, 1, 1, 0, 0, 0,
				+0.f, -1.f, +0.f, 1, 0, 1, 1, 0, 0, 0,
		};

		// VBO
		Visual::Device::VertexBuffer::CreationProperties vb_props;
		{
			vb_props.name = "Player model VBO";
			vb_props.layout = {
				Visual::Device::BufferElement( Visual::Device::ShaderDataType::Float3, "Position", false ),
				Visual::Device::BufferElement( Visual::Device::ShaderDataType::Float4, "Colour", false ),
				Visual::Device::BufferElement( Visual::Device::ShaderDataType::Float2, "TexCoord", false ),
				Visual::Device::BufferElement( Visual::Device::ShaderDataType::uInt, "TextureIndex", false ),
			};
			vb_props.SetDataFromVector( vertices );
		}

		// IBO
		Visual::Device::IndexBuffer::CreationProperties ib_props;
		ib_props.name = "Player model IBO";
		ib_props.indices = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };

		// VAO
		Visual::Device::VertexArray::CreationProperties va_props;
		va_props.name = "Player VAO";
		va_props.vertex_buffers = { renderer.CreateVertexBuffer( vb_props ) };
		va_props.index_buffer = renderer.CreateIndexBuffer( ib_props );
		model = renderer.CreateVertexArray( va_props );
	}
}
