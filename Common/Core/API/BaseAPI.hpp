#pragma once

#include "Common/Utility/Timestep.hpp"

class Core;

namespace API
{
	class BaseAPI
	{
		friend class ::Core;

	public:
		virtual ~BaseAPI() {}

		virtual std::string_view GetName() const noexcept = 0;

	private:
		virtual void Init() = 0;
		virtual void Shutdown() = 0;
		virtual void Update( const PreciseTimestep& ) {}
	};
}
