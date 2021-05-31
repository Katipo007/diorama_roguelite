#pragma once

#include <memory>

#include "Common/Core/Resources/ResourceHandle.hpp"
#include "Common/Utility/Math/Vec3.hpp"

namespace Graphics { class Sprite; }

class ResourceManager;

namespace Game
{
	/// <summary>
	/// Stand-alone object type for now. Planning to change into a "Player" component for objects
	/// so everything is in a unified system.
	/// Not doing that right away so I can actually iterate and get the basics of a runable game
	/// </summary>
	class PlayerObject
	{
	public:
		PlayerObject( ResourceManager& resources );
		virtual ~PlayerObject();

		const Resources::ResourceHandle<Graphics::Sprite>& GetSprite() const { return sprite; }

		glm::vec3& GetPosition() { return position; }
		const glm::vec3& GetPosition() const { return position; }

	protected:
		Resources::ResourceHandle<Graphics::Sprite> sprite;
		glm::vec3 position;
	};
}
