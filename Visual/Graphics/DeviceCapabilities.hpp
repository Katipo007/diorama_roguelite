
#include <cinttypes>

namespace Graphics
{
	struct DeviceCapabilities
	{
		uint32_t max_texture_slots = 0;
		uint32_t max_texture_width = 0, max_texture_height = 0;
		uint32_t max_cubemap_width = 0, max_cubemap_height = 0;
		uint32_t max_texture_coordinates = 0;
	};
}
