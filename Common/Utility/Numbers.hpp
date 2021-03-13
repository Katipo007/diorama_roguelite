#pragma once

namespace Numbers
{
	constexpr double pi = 3.14159265358979323846;
	constexpr double two_pi = pi * 2;
	constexpr double half_pi = pi / 2;

	constexpr float pi_f = static_cast<float>( pi );
	constexpr float two_pi_f = static_cast<float>( pi * 2 );
	constexpr float half_pi_f = static_cast<float>( pi / 2 );
}