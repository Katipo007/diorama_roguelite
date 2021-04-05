#include "ClientZone.hpp"

namespace Game
{
	ClientZone::ClientZone( const ZoneId id, std::string_view zone_template_id )
		: ZoneCommon( id, zone_template_id )
	{
	}

	ClientZone::~ClientZone()
	{
	}
}
