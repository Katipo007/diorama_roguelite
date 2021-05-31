#pragma once

#include <string>
#include <string_view>

namespace Input
{
	class InputDevice
	{
	public:
		virtual ~InputDevice() = default;

		virtual bool IsEnabled() const noexcept { return true; }
		virtual std::string_view GetName() const { return ""; }
	};
}
