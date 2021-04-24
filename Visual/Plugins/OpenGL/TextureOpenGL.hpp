#pragma once

#include "Common/File/Filepath.hpp"
#include "Visual/Graphics/Texture.hpp"

namespace Graphics::API
{
	class TextureOpenGL
		: public ::Graphics::Texture
	{
	public:
		TextureOpenGL( const ::Graphics::TextureDefinition& props );
		TextureOpenGL( const Filepath& path, const ::Graphics::TextureLoadProperties& props );
		virtual ~TextureOpenGL() override;

		virtual const Size<uint32_t>& GetSize() const noexcept override { return size; }

		virtual void SetData( void* data, uint32_t size ) override;

		virtual void Bind( uint32_t slot ) const override;

		virtual bool operator==( const Texture& other ) const override;

		virtual uint32_t GetNativeId() const noexcept override { return opengl_texture_id; }

	private:
		std::string filepath;
		Size<uint32_t> size;

		unsigned int opengl_internal_format;
		unsigned int opengl_data_format;
		unsigned int opengl_texture_id;
	};
}
