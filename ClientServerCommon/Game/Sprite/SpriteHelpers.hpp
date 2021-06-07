#pragma once

#include "ClientServerCommon/ecs.hpp"

#include "Common/DataTypes/Colour.hpp"

namespace Game::Sprite::Helpers
{
	void SetSprite( ecs::EntityHandle& entity, std::string_view sprite_id, std::optional<ColourRGBA> blend = std::nullopt );
	void SetBlend( ecs::EntityHandle& entity, ColourRGBA blend = 0xFFFFFFFF );
	void RemoveSprite( ecs::EntityHandle& entity );
}
