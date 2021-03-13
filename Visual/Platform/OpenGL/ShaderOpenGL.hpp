#pragma once

#include "Visual/Device/Shader.hpp"

#ifdef RENDERER_IMPLEMENTATION_OPENGL
#include "OpenGLHeader.hpp"

namespace Visual::Device::OpenGL
{
	class ShaderOpenGL
		: public ::Visual::Device::Shader
	{
	public:
		ShaderOpenGL( const std::string& filepath );
		ShaderOpenGL( std::string_view name, std::string_view vertex_src, std::string_view fragment_src );
		virtual ~ShaderOpenGL() override;

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetInt( const std::string& name, int value ) override;
		virtual void SetIntArray( const std::string& name, int* values, uint32_t count ) override;
		virtual void SetFloat( const std::string& name, float value ) override;
		virtual void SetFloat3( const std::string& name, glm::vec3 value ) override;
		virtual void SetFloat4( const std::string& name, glm::vec4 value ) override;
		virtual void SetMat4( const std::string& name, glm::mat4 value ) override;

		virtual std::string_view GetName() const { return name; }

		// OpenGL impl

		void UploadUniformInt( const std::string& name, int value );
		void UploadUniformIntArray( const std::string& name, int* values, uint32_t count );

		void UploadUniformFloat( const std::string& name, float value );
		void UploadUniformFloat2( const std::string& name, glm::vec2 value );
		void UploadUniformFloat3( const std::string& name, glm::vec3 value );
		void UploadUniformFloat4( const std::string& name, glm::vec4 value );

		void UploadUniformMat3( const std::string& name, glm::mat3 value );
		void UploadUniformMat4( const std::string& name, glm::mat4 value );

		// FOR DEBUGGING PURPOSES ONLY!!!
		GLint GetNativeProgramID() const { return opengl_program_id; }

	private:
		std::string ReadFile( const std::string& filepath );
		std::unordered_map<GLenum, std::string> PreProcess( std::string_view source );
		void Compile( const std::unordered_map<GLenum, std::string>& shader_sources );

	private:
		std::string name;
		GLint opengl_program_id;
	};

}

#endif
