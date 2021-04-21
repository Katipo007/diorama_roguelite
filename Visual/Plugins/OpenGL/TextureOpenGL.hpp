#pragma once

#include "Visual/Resources/Texture.hpp"

namespace Graphics::API
{
	class VideoOpenGL;

	class TextureOpenGL
		: public ::Graphics::Texture
	{
	public:
		explicit TextureOpenGL( VideoOpenGL& owner, Size<uint32_t> size );
		virtual ~TextureOpenGL() override;

		TextureOpenGL& operator=( TextureOpenGL&& to_move ) noexcept;

		void Bind( int slot ) const;
		unsigned int GetNativeId() const;

	private:
		void Create( Size<uint32_t> size );

	private:
		VideoOpenGL& owner;
		unsigned int texture_id = 0;
	};
}
