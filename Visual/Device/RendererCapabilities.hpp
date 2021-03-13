#pragma once

#include <cinttypes>

namespace Visual::Device
{
	struct RendererCapabilities
	{
		uint32_t max_texture_slots;

		uint32_t max_texture_2d_width;
		uint32_t max_texture_2d_height;

		uint32_t max_texture_3d_width;
		uint32_t max_texture_3d_height;
		uint32_t max_texture_3d_depth;

		uint32_t max_texture_cubemap_width;
		uint32_t max_texture_cubemap_height;

		uint32_t max_texture_coordinates;
	};
}
