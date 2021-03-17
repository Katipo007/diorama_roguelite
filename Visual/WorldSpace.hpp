#pragma once

#include "Common/Utility/Math/Vec3.hpp"

namespace Worldspace
{
	namespace _constexpr
	{
		constexpr glm::vec3 Up3 = { 0.f, 0.f, -1.f };
		constexpr glm::vec3 Forward3 = { 0.f, 1.f, 0.f };
		constexpr glm::vec3 Right3 = { 1.f, 0.f, 0.f };

		static_assert(glm::cross( Up3, Forward3 ) == Right3);
	}

	inline static const glm::vec3 Up3 = _constexpr::Up3;
	inline static const glm::vec3 Forward3 = _constexpr::Forward3;
	inline static const glm::vec3 Right3 = _constexpr::Right3;

	inline static const glm::vec4 Up4 = { Up3, 1.f };
	inline static const glm::vec4 Forward4 = { Forward3, 1.f };
	inline static const glm::vec4 Right4 = { Right3, 1.f };
}
