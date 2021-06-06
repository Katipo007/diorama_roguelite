#include "ClientSyncHelpers.hpp"

namespace Game::ClientSync::Helpers
{
    void UpdateEntity( const ecs::EntityHandle entity, Buffer_T& component_data )
    {
        ASSERT( entity.valid() );
        ASSERT( !component_data.empty() );

        if (component_data.empty())
            return;

        ReaderS serialiser{ std::begin( component_data ), std::size( component_data ) };
        uint32_t component_id;
        do
        {
            static_assert(sizeof( component_id ) == 4);
            serialiser.value4b( component_id );
            switch (component_id)
            {
            case ComponentIdentifiers::type<Name::NameComponent>:
                ReadComponent<Name::NameComponent>( entity, serialiser, false );
                break;

            //case ComponentIdentifiers::type<Sprite::SpriteComponent>:
            //    ReadComponent<Sprite::SpriteComponent>( entity, serialiser, false );
            //    break;

            default:
                throw std::runtime_error{ "Unrecognised component identifier" };
            }

        } while (true);
    }
}
