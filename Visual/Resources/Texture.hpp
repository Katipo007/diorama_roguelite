#pragma once

#include <cinttypes>

#include "Common/Core/Resources/Resource.hpp"
#include "Common/Geometry/Size.hpp"

namespace Graphics
{
	/// <summary>
	/// A standard 2D texture
	/// </summary>
	class Texture
		: public Resources::Resource
	{
	public:
		Texture( Size<uint32_t> size );
		virtual ~Texture() = default;

		virtual Size<uint32_t> GetSize() const { return size; }

	protected:
		Size<uint32_t> size;
	};
}
