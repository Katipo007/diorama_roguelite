#pragma once

#include <memory>

#include "Visual/Resources/Image.hpp"

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
		PlayerObject();
		virtual ~PlayerObject();

		const Resources::Image& GetImage() const { return image; }

	protected:
		Resources::Image image;
	};
}
