#pragma once

#include "Common/File/Filepath.hpp"
#include "Visual/Graphics/Shader.hpp"

namespace Graphics::API
{
	class ShaderOpenGL
		: public ::Graphics::Shader
	{
	public:
		ShaderOpenGL( const Filepath& filepath );
		ShaderOpenGL( std::string_view name, std::string_view vertex_src, std::string_view fragment_src );
		virtual ~ShaderOpenGL() override;

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetInt( const std::string& name, int value ) override;
		virtual void SetUInt( const std::string& name, unsigned int value ) override;
		virtual void SetIntArray( const std::string& name, int* values, uint32_t count ) override;
		virtual void SetUIntArray( const std::string& name, unsigned int* values, uint32_t count ) override;
		virtual void SetFloat( const std::string& name, float value ) override;
		virtual void SetFloat3( const std::string& name, glm::vec3 value ) override;
		virtual void SetFloat4( const std::string& name, glm::vec4 value ) override;
		virtual void SetMat4( const std::string& name, glm::mat4 value ) override;

		virtual std::string_view GetName() const { return name; }

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

		// FOR DEBUGGING PURPOSES ONLY!!!
		int GetNativeProgramID() const { return opengl_program_id; }

	private:
		std::string ReadFile( const std::filesystem::path& filepath );
		std::unordered_map<unsigned int, std::string> PreProcess( std::string_view source );
		void Compile( const std::unordered_map<unsigned int, std::string>& shader_sources );

	private:
		std::string name;
		int opengl_program_id;
	};

}
