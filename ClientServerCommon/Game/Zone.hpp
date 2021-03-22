#pragma once

namespace Game
{
	typedef uint32_t ZoneId;

	// Base class for ClientZone and ServerZone
	// A zone is a single, playable diorama part of the larger GameWorld
	class Zone
	{
	public:
		explicit Zone( const ZoneId id, std::string_view template_filename );
		virtual ~Zone() = default;

		ZoneId GetId() const { return id; }
		std::string_view GetName() const { return name; }

	protected:
		const ZoneId id;
		std::string name;
	};
}
