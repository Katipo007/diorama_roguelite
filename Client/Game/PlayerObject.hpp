#pragma once

#include <memory>

#include "Common/Core/Resources/ResourceHandle.hpp"
#include "Visual/Resources/Image.hpp"

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

		const Resources::ResourceHandle<Resources::Image>& GetImage() const { return image; }

	protected:
		Resources::ResourceHandle<Resources::Image> image;
	};
}
