#pragma once

#include <string>
#include <unordered_map>

#include "Common/Core/Resources/ResourceTypes.hpp"
#include "Common/Core/Resources/Resource.hpp"

#include "Common/DataTypes/Bytes.hpp"
#include "Common/Utility/Math/Vector.hpp"
#include "Common/Utility/Math/Matrix.hpp"

namespace Graphics
{
	enum class ShaderType
	{
		Vertex,
		Pixel,
		Combined,

		NumShaderTypes
	};

	class ShaderDefinition
	{
	public:
		std::string name;
		std::unordered_map<ShaderType, Bytes> shaders;
	};

	class ShaderFile
		: public Resources::Resource
	{
	public:
		constexpr static Resources::AssetType GetAssetType() { return Resources::AssetType::Shader; }

		std::unordered_map<ShaderType, Bytes> shaders;
	};

	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual int GetUniformLocation( const std::string& name, ShaderType stage ) = 0;
		virtual int GetBlockLocation( const std::string& name, ShaderType stage ) = 0;

		virtual std::string_view GetName() const = 0;
	};
}
