#pragma once

#include <cinttypes>
#include "Common/Geometry/Size.hpp"
#include "ClientServerCommon/Game/Constants.hpp"

namespace Game
{
	typedef uint32_t ZoneId;
	static const ZoneId InvalidZoneId = std::numeric_limits<ZoneId>::max();

	// Base class for ClientZone and ServerZone
	// A zone is a single, playable diorama part of the larger GameWorld
	class ZoneCommon
	{
	public:
		explicit ZoneCommon( const ZoneId id, std::string_view template_filename );
		virtual ~ZoneCommon() = default;

		ZoneId GetId() const { return id; }
		std::string_view GetName() const { return display_name; }

		// in tiles
		const Size<uint16_t> GetSize() const { return size; }
		const Size<uint32_t> GetWorldSize() const { return { size.width * Game::Constants::TileSize, size.height * Game::Constants::TileSize }; }


	protected:
		const ZoneId id;
		std::string display_name;
		Size<uint16_t> size; // in tiles, actual size is `size * Game::TileSize`
	};
}
