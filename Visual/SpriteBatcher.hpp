#pragma once

#include "Common/DataTypes/Colour.hpp"
#include "Common/DataTypes/Vec2.hpp"
#include "Common/DataTypes/Vec3.hpp"
#include "Common/DataTypes/Mat4.hpp"

#include "Common/Core/Resources/ResourceHandle.hpp"

namespace API { class VideoAPI; }
namespace Graphics { class Texture; class Sprite; }
namespace Resources { class Image; }


namespace Visual
{
	class Camera;

	// Unlike Renderer this does not provide a static interface and allows multiple instances to be created
	//
	// "Flat" refers to a "floor" or laid on the ground
	// "Standing" refers to standing upright, perpendicular to the floor
	class SpriteBatcher final
	{
	private:
		using TextureSlotId = unsigned int;

	public:
		struct Statistics
		{
			uint32_t n_draw_calls = 0;
			uint32_t n_quads = 0;

			uint32_t GetTotalVertexCount() const { return n_quads * 4; }
			uint32_t GetTotalIndexCount() const { return n_quads * 6; }
		};

	public:
		explicit SpriteBatcher( ::API::VideoAPI& video );
		virtual ~SpriteBatcher();

		void Begin( const Camera& camera, const glm::mat4& world_transform = glm::mat4( 1.f ) );
		void EndScene();

#pragma region Drawing state
		void PushMultiplyColour( ColourRGBA colour );
		void PopMultiplyColour();

		void Flush();
#pragma endregion

#pragma region Submission
		void DrawStandingSprite( const Resources::ResourceHandle<Graphics::Sprite>& sprite, glm::vec3 location );
#pragma endregion

		const Statistics& GetStatistics() const { return statistics; }
		void ClearStats();

	private:
		void StartBatch();
		void NextBatch();

		// Warning: can cause batch breaks
		TextureSlotId FindOrAddTexture( const std::shared_ptr<const Graphics::Texture>& texture_handle );

	private:
		::API::VideoAPI& video;

		struct Data;

		std::unique_ptr<Data> data;
		Statistics statistics;
		bool active = false;
	};
}
