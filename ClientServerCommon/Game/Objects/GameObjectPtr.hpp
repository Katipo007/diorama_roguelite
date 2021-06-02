#pragma once

#include <memory>

namespace Game
{
	class GameObject;
	using GameObjectPtr = std::shared_ptr<GameObject>;
	using ConstGameObjectPtr = std::shared_ptr<const GameObject>;
}
