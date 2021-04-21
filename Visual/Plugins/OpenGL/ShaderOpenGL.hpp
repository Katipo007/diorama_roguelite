#pragma once

#include "Visual/Resources/Shader.hpp"

namespace Graphics::API
{
	class ShaderOpenGL final
		: public ::Graphics::Shader
	{
	public:
		explicit ShaderOpenGL( const ShaderDefinition& definition );
		~ShaderOpenGL();

		void Bind();
		void Unbind();

		void Destroy();
		void SetUniformBlockBinding( unsigned int block_index, unsigned int binding );

		int GetUniformLocation( const std::string& name, ShaderType stage ) override;
		int GetBlockLocation( const std::string& name, ShaderType stage ) override;
		int GetAttributeLocation( const std::string& name, ShaderType stage );

		virtual std::string_view GetName() const { return name; }

	protected:
		// OpenGL impl
		void UploadUniformInt( const std::string& name, int value );
		void UploadUniformUInt( const std::string& name, unsigned int value );
		void UploadUniformIntArray( const std::string& name, int* values, uint32_t count );
		void UploadUniformUIntArray( const std::string& name, unsigned int* values, uint32_t count );

		void UploadUniformFloat( const std::string& name, float value );
		void UploadUniformFloat2( const std::string& name, glm::vec2 value );
		void UploadUniformFloat3( const std::string& name, glm::vec3 value );
		void UploadUniformFloat4( const std::string& name, glm::vec4 value );

		void UploadUniformMat3( const std::string& name, glm::mat3 value );
		void UploadUniformMat4( const std::string& name, glm::mat4 value );

	private:
		void LoadShaders( const std::unordered_map<ShaderType, std::vector<std::byte>>& shaders );
		void Compile();

	private:
		std::string name;
		unsigned int id = 0;
		bool ready = false;

		std::vector<Bytes> vertex_sources;
		std::vector<Bytes> pixel_sources;
		std::vector<unsigned int> program_ids;

		std::unordered_map<std::string, unsigned int> attribute_locations;
		std::unordered_map<std::string, unsigned int> uniform_locations;
		std::unordered_map<std::string, unsigned int> block_locations;
	};

}
