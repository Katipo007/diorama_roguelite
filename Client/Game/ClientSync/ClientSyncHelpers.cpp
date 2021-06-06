#include "ClientSyncHelpers.hpp"

namespace Game::ClientSync::Helpers
{
    void UpdateEntity( const ecs::EntityHandle entity, const Buffer_T& component_data )
    {
        ASSERT( !!entity );
        ASSERT( !component_data.empty() );

        if (component_data.empty())
            return;

        Deserialiser deserialiser{ Reader( std::begin( component_data ), std::end( component_data ) ) };
        ComponentIdentifiers::identifier_type component_id;
        while (!deserialiser.adapter().isCompletedSuccessfully())
        {
            static_assert(sizeof( component_id ) == 4);
            deserialiser.value4b( component_id );
            switch (component_id)
            {
            case ComponentIdentifiers::type<Name::NameComponent>:
                ReadComponent<Name::NameComponent>( entity, deserialiser, false );
                break;

            //case ComponentIdentifiers::type<Sprite::SpriteComponent>:
            //    ReadComponent<Sprite::SpriteComponent>( entity, serialiser, false );
            //    break;

            default:
                throw std::runtime_error{ "Unrecognised component identifier" };
            }

        }
    }
}
