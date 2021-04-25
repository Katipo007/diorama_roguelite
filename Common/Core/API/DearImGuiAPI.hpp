#pragma once

#include "APITypesEnum.hpp"
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

		static constexpr APIType GetType() noexcept { return APIType::DearImGui; }

		virtual void SetEnabled( const bool enable ) = 0;
		virtual bool GetEnabled() const noexcept = 0;

		virtual void ProcessSystemEvent( void* e ) = 0;

	private:
		virtual void OnFrameBegin() = 0;
		virtual void OnFrameEnd() = 0;
		virtual void DoRender() = 0;

		virtual void OnWindowResized( Size<uint32_t> new_target_size ) = 0;
	};
}
