#include "TestMovementSystem.hpp"

#include "ClientServerCommon/Game/Transform/Components/PositionComponent.hpp"
#include "ClientServerCommon/Game/Transform/TransformHelpers.hpp"

namespace Game::Movement
{
	void TestSystem( ecs::Registry& registry, const PreciseTimestep& ts )
	{
		for (auto entity : registry.view<Transform::PositionComponent>())
		{
			const float t = (float)ts.time * 10.f + int( entity ) * 2.f;
			Transform::Helpers::SetPosition( { registry, entity }, glm::vec3{ std::sin( t ) * 10, std::cos( t ) * 10, 0.f } );
		}
	}
}
