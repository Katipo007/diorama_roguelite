#pragma once

#include <cinttypes>

namespace Input
{
	typedef uint8_t MouseCode;

	namespace Mouse
	{
		enum : MouseCode
		{
			Button0 = 0,
			Button1,
			Button2,
			Button3,
			Button4,
			Button5,
			Button6,
			Button7,

			NumMouseButtons,

			ButtonLast = Button7,
			ButtonLeft = Button0,
			ButtonRight = Button1,
			ButtonMiddle = Button2
		};
	}
}
