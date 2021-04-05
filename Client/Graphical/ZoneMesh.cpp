#include "ZoneMesh.hpp"

#include "Visual/Device/RendererAPI.hpp"
#include "Visual/Device/RendererCommand.hpp"
#include "Visual/Device/GraphicsBuffer.hpp"
#include "Visual/Device/Shader.hpp"
#include "Visual/Device/Texture.hpp"
#include "Visual/Device/VertexArray.hpp"

#include "Visual/Resources/ImageResourceManager.hpp"

#include "Common/Geometry/Size.hpp"

#include "Client/Game/ClientZone.hpp"

namespace
{
#pragma pack(push, 1)
	struct Vertex
	{
		glm::vec3 position;
		glm::vec2 texcoord;
		unsigned int texture_index;
	};
#pragma pack(pop)

	Visual::Device::BufferLayout Layout
	{
		{ Visual::Device::ShaderDataType::Float3, "a_Position" }
		, { Visual::Device::ShaderDataType::Float2, "a_Texcoord" }
		, { Visual::Device::ShaderDataType::uInt, "a_TextureIndex" }
	};

	using VertexList_T = std::vector<Vertex>;
	using IndexList_T = std::vector<uint32_t>;
}

namespace Graphical
{
	class ZoneMesh::Builder
	{
	public:
		enum class ImageRotation
		{
			None,
			Rot90DegClockwise,
			Rot90DegAntiClockwise,
		};

	public:
		Builder( ZoneMesh& mesh, VertexList_T& vertices, IndexList_T& indices )
			: mesh( mesh )
			, vertices( vertices )
			, indices( indices )
		{
			//border_corner_img = Resources::ImageResourceManager::GetImageById( "2DArt/Tiles/WorldBorder1" );
			//border_edge_img = Resources::ImageResourceManager::GetImageById( "2DArt/Tiles/WorldBorder2" );
			NOT_IMPLEMENTED;

			ASSERT( !border_corner_img.IsNull() );
			ASSERT( !border_edge_img.IsNull() );
		}

		void AddZoneBounds( Size<float> world_size )
		{
			NOT_IMPLEMENTED;
			(void)world_size;

		}

		void AddTexturedFloorRect( Rect<float> rect, const Resources::Image& img, const ImageRotation& rot )
		{
			ASSERT( !img.IsNull() );
			//const auto tex_id = mesh.FindOrAddTexture( img.GetSharedTexture() );
			NOT_IMPLEMENTED;
			(void)rect;
			(void)rot;

		}

	protected:
		ZoneMesh& mesh;
		VertexList_T& vertices;
		IndexList_T& indices;

		Resources::Image border_corner_img;
		Resources::Image border_edge_img;
	};

	ZoneMesh::ZoneMesh()
	{
	}

	ZoneMesh::~ZoneMesh()
	{
	}

	void ZoneMesh::Reconstruct( const Game::ClientZone& zone )
	{
		auto& api = Visual::Device::RendererCommand::GetRendererAPI();
		const auto name_prefix = "Zone #" + std::to_string( zone.GetId() );

		VertexList_T vertices;
		IndexList_T indices;

		Visual::Device::VertexBuffer::CreationProperties vb_props;
		vb_props.name = name_prefix + " mesh VB";
		vb_props.layout = Layout;
		vb_props.SetDataFromVector( vertices );

		Visual::Device::IndexBuffer::CreationProperties ib_props;
		ib_props.name = name_prefix + " mesh IB";
		ib_props.indices = indices;

		Visual::Device::VertexArray::CreationProperties va_props;
		va_props.name = name_prefix + " mesh VA";
		va_props.vertex_buffers = { api.CreateVertexBuffer( vb_props ) };
		va_props.index_buffer = api.CreateIndexBuffer( ib_props );
		va = api.CreateVertexArray( va_props );
	}

	unsigned ZoneMesh::FindOrAddTexture( const std::shared_ptr<Visual::Device::Texture2D>& texture_handle )
	{
		const auto start_it = std::begin( textures );
		const auto end_it = std::end( textures );
		const auto it = std::find( start_it, end_it, texture_handle );
		if (it != end_it)
			return static_cast<unsigned>(std::distance( start_it, it ));
		
		const auto max_textures_slots = Visual::Device::RendererCommand::GetRendererAPI().GetCapabilities().max_texture_slots;
		ASSERT( textures.size() <= max_textures_slots, "Exceeded maximum number of textures for a single submission" );

		textures.emplace_back( texture_handle );
		return (unsigned)textures.size();
	}
}
