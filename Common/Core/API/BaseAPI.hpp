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
		virtual void OnFixedUpdate( const PreciseTimestep&, const StepType ) {}
		virtual void OnVariableUpdate( const PreciseTimestep&, const StepType ) {}
		virtual void OnRender( const PreciseTimestep&, const StepType ) {}
	};
}
