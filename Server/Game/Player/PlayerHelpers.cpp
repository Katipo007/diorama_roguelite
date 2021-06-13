#include "PlayerHelpers.hpp"

#include "ClientServerCommon/Game/Transform/TransformHelpers.hpp"

namespace Game::Player::Helpers
{
	bool ApplyMovementFromClient( const ecs::EntityHandle player_entity, const glm::vec3& client_specified_position )
	{
		const auto current_position = Transform::Helpers::GetAbsolutePosition( player_entity );
		const auto delta = client_specified_position - current_position;

		const auto delta_sqr = (delta.x * delta.x) + (delta.y * delta.y);
		constexpr auto max_delta_sqr = 100;
		if (delta_sqr > max_delta_sqr)
		{
			// TODO: Send position message to client with their current position
			return false;
		}

		// TODO: Collisions

		Transform::Helpers::SetPosition( player_entity, client_specified_position );
		return true;
	}
}
