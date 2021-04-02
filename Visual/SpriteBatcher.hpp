#pragma once

#include "Common/Utility/Math/Vec2.hpp"
#include "Common/Utility/Math/Vec3.hpp"
#include "Common/Utility/Math/Mat4.hpp"
#include "Visual/Graphics/Colour.hpp"

namespace Resources
{
	class Image;
}

namespace Visual::Device
{
	class Texture2D;
}

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
		explicit SpriteBatcher();
		virtual ~SpriteBatcher();

		void Begin( const Camera& camera, const glm::mat4& world_transform = glm::mat4( 1.f ) );
		void EndScene();

#pragma region Drawing state
		void PushMultiplyColour( ColourRGBA colour );
		void PopMultiplyColour();

		void Flush();
#pragma endregion

#pragma region Submission
		void DrawStandingImage( const Resources::Image& img, glm::vec3 location, glm::vec2 pivot = { 0.5f, 1.f } );
#pragma endregion

		const Statistics& GetStatistics() const { return statistics; }
		void ClearStats();

	private:
		void StartBatch();
		void NextBatch();

		// Warning: can cause batch breaks
		TextureSlotId FindOrAddTexture( const std::shared_ptr<const Device::Texture2D>& texture_handle );

	private:
		struct Data;

		std::unique_ptr<Data> data;
		Statistics statistics;
		bool active = false;
	};
}
