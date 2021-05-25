#pragma once

#include "CoreAPIsEnum.hpp"
#include "BaseAPI.hpp"

#include "Common/Geometry/Size.hpp"

namespace API
{
	class SystemAPI;

	class DearImGuiAPI
		: public BaseAPI
	{
		friend class ::Core;

	public:
		virtual ~DearImGuiAPI() {}

		static constexpr APIType GetType() noexcept { return CoreAPIs::DearImGui; }

		virtual void SetEnabled( const bool enable ) = 0;
		virtual bool GetEnabled() const noexcept = 0;

		virtual void ProcessSystemEvent( void* e ) = 0;

	private:
		virtual void OnWindowResized( Size<uint32_t> new_target_size ) = 0;
	};
}
