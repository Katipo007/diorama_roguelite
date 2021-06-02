#pragma once

#include <memory>

namespace Game
{
	class GameObject;
	using GameObjectWeakPtr = std::weak_ptr<GameObject>;
	using ConstGameObjectWeakPtr = std::weak_ptr<const GameObject>;
}
