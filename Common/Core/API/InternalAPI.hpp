#pragma once

#include "InputAPI.hpp"
#include "SystemAPI.hpp"
#include "VideoAPI.hpp"

#include "Common/Utility/Timestep.hpp"

class Core;

namespace API
{
	class InternalAPI
	{
		friend class ::Core;

	public:
		virtual ~InternalAPI() {}

		virtual void Init() = 0;
		virtual void Shutdown() = 0;

		virtual std::string_view GetName() const = 0;
	};

	class InternalVideoAPI
		: public VideoAPI
		, public InternalAPI
	{
	public:
		virtual ~InternalVideoAPI() {}
	};

	class InternalInputAPI
		: public InputAPI
		, public InternalAPI
	{
	public:
		virtual ~InternalInputAPI() {}

		virtual void BeginEvents( const PreciseTimestep& ts ) = 0;
	};

	class InternalSystemAPI
		: public SystemAPI
		, public InternalAPI
	{
	public:
		virtual ~InternalSystemAPI() {}

		virtual void Update( const PreciseTimestep& ts ) = 0;
	};
}
