#pragma once

#include <tuple>

#include "Keycodes.hpp"
#include "MouseCodes.hpp"

int main( int, char** );

class InputManager
{
	friend int ::main( int, char** );

public:
	// left undefined, gets defined in by platform input .cpp s

	static bool IsKeyPressed( const ::Input::KeyCode key );

	static bool IsMouseButtonPressed( const ::Input::MouseCode button );
	static std::pair<float, float> GetMousePosition();
	static float GetMouseX();
	static float GetMouseY();

private:
	static void SetWindowHandle( void* native_window_handle );
};
