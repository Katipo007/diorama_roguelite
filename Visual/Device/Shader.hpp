#pragma once

#include <string>
#include <unordered_map>

#include "Common/Utility/Math/Vector.hpp"
#include "Common/Utility/Math/Matrix.hpp"

namespace Visual::Device
{
	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetInt( const std::string& name, int value ) = 0;
		virtual void SetIntArray( const std::string& name, int* values, uint32_t count ) = 0;
		virtual void SetFloat( const std::string& name, float value ) = 0;
		virtual void SetFloat3( const std::string& name, glm::vec3 value ) = 0;
		virtual void SetFloat4( const std::string& name, glm::vec4 value ) = 0;
		virtual void SetMat4( const std::string& name, glm::mat4 value ) = 0;

		virtual std::string_view GetName() const = 0;
	};
}
