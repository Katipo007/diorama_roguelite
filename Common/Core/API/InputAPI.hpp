#pragma once

#include "CoreAPIsEnum.hpp"
#include "BaseAPI.hpp"

namespace API
{
	class InputAPI
		: public BaseAPI
	{
		friend class ::Core;

	public:
		virtual ~InputAPI() {}

		static constexpr APIType GetType() noexcept { return CoreAPIs::Input; }

		// TODO

	private:
		virtual void BeginEvents( const PreciseTimestep& ts ) = 0;
	};
}
