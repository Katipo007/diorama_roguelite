#include "SpriteBatcher.hpp"

#include <cinttypes>
#include <numeric>
#include <stack>

#include "Common/Core/API/VideoAPI.hpp"

#include "Visual/Camera.hpp"
#include "Visual/Graphics/DeviceCapabilities.hpp"
#include "Visual/Graphics/GraphicsBuffer.hpp"
#include "Visual/Graphics/VertexArray.hpp"
#include "Visual/Graphics/Shader.hpp"
#include "Visual/Graphics/Texture.hpp"
#include "Visual/Resources/Image.hpp"

namespace
{
#pragma pack(push, 1)
	struct QuadVertex
	{
		glm::vec3 pos;
		glm::vec4 colour;
		glm::vec2 texcoord;
		uint32_t texture_index;

		QuadVertex()
			: pos( { 0.f, 0.f, 0.f } ), colour( { 1.f, 1.f, 1.f, 1.f } ), texcoord( { 0.f, 0.f } ), texture_index( 0 )
		{}
		QuadVertex( glm::vec3 pos, glm::vec4 colour, glm::vec2 texcoord, uint32_t texture_index )
			: pos( pos ), colour( colour ), texcoord( texcoord ), texture_index( texture_index )
		{}
	};
#pragma pack(pop)

	static const Graphics::BufferLayout VertexLayout( {
			{ Graphics::ShaderDataType::Float3, "a_Position" }
			, { Graphics::ShaderDataType::Float4, "a_Colour" }
			, { Graphics::ShaderDataType::Float2, "a_Texcoord" }
			, { Graphics::ShaderDataType::uInt, "a_TextureIndex" }
		} );

	constexpr uint32_t DefaultTextureIndex = 0; // index that the plain white texture should be loaded in
}

namespace Visual
{
	///
	/// Data
	/// 
	struct SpriteBatcher::Data
	{
		const uint32_t NMaxQuads;
		const uint32_t NMaxVertices;
		const uint32_t NMaxIndices;
		const uint32_t NMaxTextureSlots;

		::API::VideoAPI& video;

		// multiply colour
		std::stack<glm::vec4> multiply_colour;

		// device objects
		std::shared_ptr<Graphics::VertexArray> va;
		std::shared_ptr<Graphics::VertexBuffer> vb;
		std::shared_ptr<Graphics::Shader> default_shader;
		std::shared_ptr<Graphics::Texture> white_texture;
		std::vector<std::shared_ptr<const Graphics::Texture>> texture_slots;

		// data pointers/counters
		std::vector<QuadVertex> vertex_data;
		uint32_t quad_index_count = 0;
		uint32_t texture_slot_index = 0;
		std::shared_ptr<Graphics::Shader> active_shader;

		// current scene data
		const Visual::Camera* scene_camera = nullptr;
		glm::mat4 scene_transform = glm::mat4( 1.f );

		Data( ::API::VideoAPI& video_, const uint32_t max_quads_, const uint32_t max_texture_slots_ )
			: video( video_ )
			, NMaxQuads( max_quads_ )
			, NMaxVertices( max_quads_ * 4 )
			, NMaxIndices( max_quads_ * 6 )
			, NMaxTextureSlots( std::min( (uint32_t)32, max_texture_slots_ ) )
		{
			if (NMaxQuads < 1)
				throw std::runtime_error( "Device reported it can't render quads?" );
			if (NMaxTextureSlots <= 1)
				throw std::runtime_error( "Device reported not enough texture slots" );

			std::shared_ptr<Graphics::IndexBuffer> ib;

			// vertex buffer
			{
				Graphics::VertexBufferDefinition vb_props;
				vb_props.name = "SpriteBatcher VB";
				vb_props.layout = VertexLayout;
				vb_props.data.reserve( sizeof( QuadVertex ) * NMaxVertices );
				vb_props.SetDataFromVector( std::vector<QuadVertex>( NMaxVertices ) );

				vb = video.CreateVertexBuffer( vb_props );
			}

			// index buffer
			{
				Graphics::IndexBufferDefinition ib_props;
				ib_props.name = "SpriteBatcher IB";
				ib_props.indices.reserve( NMaxIndices );

				// generate the indices, a repeating pattern
				constexpr uint32_t quad_indices[] = { 0, 1, 2, 1, 3, 2 };
				constexpr uint32_t n_quad_indices = sizeof( quad_indices ) / sizeof( quad_indices[0] );
				uint32_t i = 0;
				std::generate_n( std::back_inserter( ib_props.indices ), NMaxIndices, [&]() mutable { return ((i / n_quad_indices) * 4) + quad_indices[i++ % n_quad_indices]; } );

				ib = video.CreateIndexBuffer( ib_props );
			}

			// vertex array
			{
				Graphics::VertexArrayDefinition va_props;
				va_props.name = "SpriteBatcher VA";
				va_props.vertex_buffers = { vb };
				va_props.index_buffer = std::move( ib );

				va = video.CreateVertexArray( va_props );
			}

			// texture slots
			{
				texture_slots.resize( NMaxTextureSlots );
				std::fill( std::begin( texture_slots ), std::end( texture_slots ), nullptr );
			}

			// white texture
			{
				Graphics::TextureDefinition texture_props;
				texture_props.size = { 1, 1 };
				white_texture = video.CreateTexture( texture_props );
				uint32_t white_texture_data = 0xffffffff;
				white_texture->SetData( &white_texture_data, sizeof( decltype(white_texture_data) ) );

				// first slot is always the white texture
				texture_slots[0] = white_texture;
			}

			// default shader
			{
				auto* initial_samplers = new int[NMaxTextureSlots];
				std::iota( initial_samplers, initial_samplers + (size_t)NMaxTextureSlots, 0 ); // fill initial samplers with 0, 1, 2, 3, etc

				default_shader = video.CreateShader( std::filesystem::path( "Shaders/DefaultSpriteBatchShader.glsl" ) ); // TODO: replace extention once we support multiple pipelines
				default_shader->Bind();
				default_shader->SetIntArray( "u_Textures", initial_samplers, NMaxTextureSlots );

				delete[] initial_samplers;
			}

			// default states
			{
				vertex_data.reserve( NMaxQuads );

				multiply_colour.emplace( glm::vec4{ 1.f, 1.f, 1.f, 1.f } );
			}
		}

		~Data() = default;
	};


	///
	/// SpriteBatcher
	/// 

	SpriteBatcher::SpriteBatcher( ::API::VideoAPI& video_ )
		: video( video_ )
	{
		const auto& capabilities = video.GetDeviceCapabilities();

		// TODO: determine max number of vertexes/indices
		data = std::make_unique<Data>( video, 20000, std::min( (uint32_t)capabilities.max_texture_slots, (uint32_t)10 ) );
	}

	SpriteBatcher::~SpriteBatcher()
	{
		data.reset();
	}

	void SpriteBatcher::Begin( const Camera& camera, const glm::mat4& world_transform )
	{
		ASSERT( !active );

		data->scene_camera = &camera;
		data->scene_transform = world_transform;

		data->default_shader->Bind();
		data->default_shader->SetMat4( "u_ViewProjection", camera.GetViewProjectionMatrix() );
		data->default_shader->SetMat4( "u_Model", world_transform );

		data->active_shader = data->default_shader;

		active = true;
		StartBatch();
	}

	void SpriteBatcher::EndScene()
	{
		ASSERT( active );
		Flush();
		active = false;
	}

	void SpriteBatcher::Flush()
	{
		if (data->vertex_data.empty())
			return; // nothing currently pending
		
		if (data->active_shader == nullptr)
			data->active_shader = data->default_shader;

		data->active_shader->Bind();

		// bind textures
		for (uint32_t i = 0; i < data->texture_slot_index; i++)
			data->texture_slots[i]->Bind( i );

		data->va->Bind();

		// move data into the vertex buffer object
		data->vb->SetData( data->vertex_data.data(), (uint32_t)data->vertex_data.size() * sizeof( QuadVertex ) );

		video.DrawIndexed( data->va, data->quad_index_count );

		data->va->Unbind();

		++statistics.n_draw_calls;
	}

	void SpriteBatcher::StartBatch()
	{
		ASSERT( active );
		data->quad_index_count = 0;
		data->vertex_data.clear(); // reset data pointer to the start
		ASSERT( data->vertex_data.capacity() > 0 );
		data->texture_slot_index = DefaultTextureIndex + 1;
	}

	void SpriteBatcher::NextBatch()
	{
		ASSERT( active );
		Flush();
		StartBatch();
	}

	SpriteBatcher::TextureSlotId SpriteBatcher::FindOrAddTexture( const std::shared_ptr<const Graphics::Texture>& texture_handle )
	{
		SpriteBatcher::TextureSlotId index = std::numeric_limits<TextureSlotId>::max();

		const auto first = std::begin( data->texture_slots );
		const auto last = first + (size_t)data->texture_slot_index;
		const auto it = std::find( first, last, texture_handle );

		// found the entry
		if (it < last)
			index = static_cast<SpriteBatcher::TextureSlotId>(std::distance( first, it ));
		// add a new entry
		else
		{
			// break batches if exceeding max number of bound texture slots
			if (data->texture_slot_index >= data->NMaxTextureSlots)
				NextBatch();

			index = static_cast<SpriteBatcher::TextureSlotId>(data->texture_slot_index);
			data->texture_slots[index] = texture_handle;
			data->texture_slot_index++;
		}

		ASSERT( index > 0 && index < std::numeric_limits<TextureSlotId>::max() );
		return index;
	}

	void SpriteBatcher::DrawStandingImage( const Resources::Image& img, glm::vec3 location, glm::vec2 pivot )
	{
		if (img.IsNull())
			return;

		if (data->quad_index_count >= data->NMaxIndices)
			NextBatch();

		const auto img_size_vec = glm::vec2( img.GetSizeF().width, img.GetSizeF().height );
		const auto min = -img_size_vec * pivot;
		const auto max = min + img_size_vec;

		const auto& texture_id = FindOrAddTexture( img.GetSharedTexture() );
		const auto& uvs = img.GetUVs();
		const auto& multiply_colour = data->multiply_colour.top();

		data->vertex_data.emplace_back( glm::vec3{ location.x + min.x, location.y + 0.f, location.z + min.y }, multiply_colour, glm::vec2( uvs.GetLeft(), uvs.GetTop() ), texture_id ); // top left
		data->vertex_data.emplace_back( glm::vec3{ location.x + max.x, location.y + 0.f, location.z + min.y }, multiply_colour, glm::vec2( uvs.GetRight(), uvs.GetTop() ), texture_id ); // top right
		data->vertex_data.emplace_back( glm::vec3{ location.x + min.x, location.y + 0.f, location.z + max.y }, multiply_colour, glm::vec2( uvs.GetLeft(), uvs.GetBottom() ), texture_id ); // bottom left
		data->vertex_data.emplace_back( glm::vec3{ location.x + max.x, location.y + 0.f, location.z + max.y }, multiply_colour, glm::vec2( uvs.GetRight(), uvs.GetBottom() ), texture_id ); // bottom right

		data->quad_index_count += 6;
		statistics.n_quads++;
	}

	void SpriteBatcher::PushMultiplyColour( ColourRGBA colour )
	{
		data->multiply_colour.emplace( std::move( colour.AsFloatsRGBA() ) );
	}

	void SpriteBatcher::PopMultiplyColour()
	{
		ASSERT( data->multiply_colour.size() > 1 );
		data->multiply_colour.pop();
	}

	void SpriteBatcher::ClearStats()
	{
		memset( &statistics, 0, sizeof( SpriteBatcher::Statistics ) );
	}
}
