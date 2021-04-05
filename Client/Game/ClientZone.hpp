#pragma once

#include "ClientServerCommon/Game/ZoneCommon.hpp"

namespace Graphical
{
	class ZoneMesh;
}

namespace Game
{
	class ClientZone
		: public ZoneCommon
	{
		friend class ZoneMesh;

	public:
		ClientZone( const ZoneId id, std::string_view zone_template_id );
		virtual ~ClientZone();

		std::shared_ptr<const Graphical::ZoneMesh> GetMesh() const { return mesh; }

	protected:
		void RebuildMesh();

	protected:
		std::shared_ptr<Graphical::ZoneMesh> mesh;
	};
}
