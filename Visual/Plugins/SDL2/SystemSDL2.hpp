#pragma once

#include "Common/Core/API/InternalAPI.hpp"

namespace Graphics::API
{
	class SystemSDL2 final
		: public ::API::InternalSystemAPI
	{
	public:
		explicit SystemSDL2();
		virtual ~SystemSDL2();

	protected:
		void Init() override;
		void Shutdown() override;
	};
}
