#pragma once

#include <memory>

namespace Visual::Device
{
	class Shader;
	class Texture2D;
	class VertexArray;
}

namespace Game
{
	/// <summary>
	/// Stand-alone object type for now. Planning to change into a "Player" component for objects
	/// so everything is in a unified system.
	/// Not doing that right away so I can actually iterate and get the basics of a runable game
	/// </summary>
	class PlayerObject
	{
	public:
		PlayerObject();
		virtual ~PlayerObject();

		std::shared_ptr<Visual::Device::Shader> GetShader() const { return shader; }
		std::shared_ptr<Visual::Device::Texture2D> GetTexture() const { return texture; }
		std::shared_ptr<Visual::Device::VertexArray> GetModel() const { return model; }

	protected:
		void CreateModel();

		std::shared_ptr<Visual::Device::Shader> shader;
		std::shared_ptr<Visual::Device::Texture2D> texture;
		std::shared_ptr<Visual::Device::VertexArray> model;
	};
}
