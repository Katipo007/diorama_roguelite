#pragma once

namespace Graphics
{
	class VertexArray;
	class VertexBuffer;
	class IndexBuffer;
	class Shader;
	class Texture;
}

namespace Game
{
	class ClientZone;
}

namespace Graphical
{
	class ZoneMesh
	{
		class Builder;
		friend class Builder;

	public:
		ZoneMesh();
		virtual ~ZoneMesh();

		void Reconstruct( const Game::ClientZone& zone );

	protected:
		/// <summary>
		/// Find or add a texture
		/// </summary>
		/// <param name="texture_handle">Texture to locate or add</param>
		/// <returns>Texture index</returns>
		unsigned FindOrAddTexture( const std::shared_ptr<Graphics::Texture>& texture_handle );

	protected:
		std::shared_ptr<Graphics::VertexArray> va;
		std::shared_ptr<Graphics::Shader> shader;
		std::vector<std::shared_ptr<Graphics::Texture>> textures;
	};
}
