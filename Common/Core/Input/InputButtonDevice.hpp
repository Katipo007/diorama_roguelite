#pragma once

#include <vector>

#include "InputDevice.hpp"

namespace Input
{
	using ButtonCode_T = int;

	class InputButtonDevice
		: public InputDevice
	{
	public:
		InputButtonDevice( size_t num_buttons );
		virtual ~InputButtonDevice();

		virtual size_t GetButtonCount() const noexcept;

		virtual std::string_view GetButtonName( ButtonCode_T code ) const;

		virtual bool IsAnyButtonPressed();
		virtual bool IsAnyButtonReleased();
		virtual bool IsAnyButtonDown();

		virtual bool IsButtonPressed( ButtonCode_T code );
		virtual bool IsButtonPressedRepeat( ButtonCode_T code );
		virtual bool IsButtonReleased( ButtonCode_T code );
		virtual bool IsButtonDown( ButtonCode_T code );

		virtual void ClearButton( ButtonCode_T code );
		virtual void ClearButtonPress( ButtonCode_T code );
		virtual void ClearButtonRelease( ButtonCode_T code );

		void ClearButtonPresses();

		/// <summary>
		/// Used to update button states for polled button devices
		/// </summary>
		void OnPolledButtonStatus( ButtonCode_T code, bool is_down );

	protected:
		std::vector<uint8_t> button_pressed;
		std::vector<uint8_t> button_pressed_repeat;
		std::vector<uint8_t> button_released;
		std::vector<uint8_t> button_down;

		void Init( size_t num_buttons );

		virtual void OnButtonPressed( ButtonCode_T code );
		virtual void OnButtonReleased( ButtonCode_T code );
	};
}
