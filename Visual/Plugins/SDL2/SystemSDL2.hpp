#pragma once

#include "Common/Core/API/InternalAPI.hpp"

namespace Graphics::API
{
	class SystemSDL2 final
		: public ::API::InternalSystemAPI
	{
	public:
		SystemSDL2();
		~SystemSDL2();

	protected:
		void Init() override;
		void Shutdown() override;
	};
}
