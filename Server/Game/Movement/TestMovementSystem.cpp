#include "TestMovementSystem.hpp"

#include "ClientServerCommon/Game/Transform/Components/PositionComponent.hpp"
#include "ClientServerCommon/Game/Transform/TransformHelpers.hpp"

namespace Game::Movement
{
	void TestSystem( ecs::Registry& registry, const PreciseTimestep& ts )
	{
		for (auto entity : registry.view<Transform::PositionComponent>())
			Transform::Helpers::SetPosition( { registry, entity }, glm::vec3{ std::sin( ts.time * 10 ) * 10, std::cos( ts.time * 10 ) * 10, 0.f } );
	}
}
