#include "InputButtonDevice.hpp"

#include "Common/Utility/Containers.hpp"

namespace Input
{
	InputButtonDevice::InputButtonDevice( size_t num_buttons )
	{
		Init( num_buttons );
	}

	InputButtonDevice::~InputButtonDevice() = default;

	size_t InputButtonDevice::GetButtonCount() const noexcept
	{
		return button_down.size();
	}

	std::string_view InputButtonDevice::GetButtonName( ButtonCode_T code ) const
	{
		static std::string button_name;
		return (button_name = "Button(" + std::to_string( code ) + ")");
	}

	bool InputButtonDevice::IsAnyButtonPressed()
	{
		return AnyOf( button_pressed, []( const auto& value ) { return value != 0; } );
	}

	bool InputButtonDevice::IsAnyButtonReleased()
	{
		return AnyOf( button_released, []( const auto& value ) { return value != 0; } );
	}

	bool InputButtonDevice::IsAnyButtonDown()
	{
		return AnyOf( button_down, []( const auto& value ) { return value != 0; } );
	}

	bool InputButtonDevice::IsButtonPressed( ButtonCode_T code )
	{
		return ((code >= 0) && (code < button_pressed.size())) && button_pressed[code];
	}

	bool InputButtonDevice::IsButtonPressedRepeat( ButtonCode_T code )
	{
		return ((code >= 0) && (code < button_pressed_repeat.size())) && button_pressed_repeat[code];
	}

	bool InputButtonDevice::IsButtonReleased( ButtonCode_T code )
	{
		return ((code >= 0) && (code < button_released.size())) && button_released[code];
	}

	bool InputButtonDevice::IsButtonDown( ButtonCode_T code )
	{
		return ((code >= 0) && (code < button_down.size())) && button_down[code];
	}

	void InputButtonDevice::ClearButton( ButtonCode_T code )
	{
		if ((code >= 0) && (code < button_down.size()))
			button_pressed[code] = button_pressed_repeat[code] = button_down[code] = button_released[code] = 0;
	}

	void InputButtonDevice::ClearButtonPress( ButtonCode_T code )
	{
		if ((code >= 0) && (code < button_down.size()))
			button_pressed[code] = button_pressed_repeat[code] = 0;
	}

	void InputButtonDevice::ClearButtonRelease( ButtonCode_T code )
	{
		if ((code >= 0) && (code < button_down.size()))
			button_released[code] = 0;
	}

	void InputButtonDevice::ClearButtonPresses()
	{
		Fill( button_pressed, 0 );
		Fill( button_pressed_repeat, 0 );
		Fill( button_released, 0 );
	}

	void InputButtonDevice::OnPolledButtonStatus( ButtonCode_T code, const bool is_down )
	{
		const bool was_down = button_down[code];
		button_down[code] = is_down;

		if (was_down && !is_down)
			button_released[code] = true;

		if (!was_down && is_down)
			button_pressed[code] = button_pressed_repeat[code] = true;
	}

	void InputButtonDevice::Init( size_t num_buttons )
	{
		button_pressed.resize( num_buttons );
		button_pressed_repeat.resize( num_buttons );
		button_released.resize( num_buttons );
		button_down.resize( num_buttons );
	}

	void InputButtonDevice::OnButtonPressed( ButtonCode_T code )
	{
		button_pressed_repeat[code] = true;
		if (!button_down[code])
		{
			button_pressed[code] = true;
			button_down[code] = true;
		}
	}

	void InputButtonDevice::OnButtonReleased( ButtonCode_T code )
	{
		if (button_down[code])
		{
			button_released[code] = true;
			button_down[code] = false;
		}
	}
}
