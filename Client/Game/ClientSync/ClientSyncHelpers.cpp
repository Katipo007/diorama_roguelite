#include "ClientSyncHelpers.hpp"

#include "ClientServerCommon/Game/ClientSync/ClientSyncMessages.hpp"
#include "ClientServerCommon/Game/ClientSync/SyncableComponents.hpp"
#include "Common/DataTypes/Bytes.hpp"
#include "Common/Utility/Math/Vec3.hpp"

namespace Game::ClientSync::Helpers
{
    namespace
    {
        template<SerialisableComponent C>
        inline void ReadComponent( C& component, Deserialiser& deserialiser, const bool expects_identifier = true )
        {
            if (expects_identifier)
            {
                constexpr auto expected = ComponentIdentifiers::template type<C>;
                ComponentIdentifiers::identifier_type actual;
                static_assert(sizeof( actual ) == 4);
                deserialiser.value4b( actual );
                ASSERT( actual == expected );
                if (actual != expected)
                    throw std::runtime_error{ "Mismatched component type!" };
            }
            return component.Serialise( deserialiser );
        }

        template<SerialisableComponent C>
        void ReadComponent( const ecs::EntityHandle entity, Deserialiser& deserialiser, const bool expects_identifier = true )
        {
            ASSERT( !!entity );
            auto& component = entity.get_or_emplace<C>();
            return ReadComponent( component, deserialiser, expects_identifier );
        }

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

    bool HandleMessage( ecs::Registry& registry, SyncedEntityMap_T& map, const Messages::ServerClientAddEntity& msg )
    {
        if (map.contains( msg.entity_sync_id ))
        {
            LOG_WARN( LoggingChannels::Client, "Ignoring ServerClientAddEntity message because entity with sync id '{}' already exists", msg.entity_sync_id );
            return true;
        }

        const auto entity = registry.create();
        map[msg.entity_sync_id] = ecs::EntityHandle{ registry, entity };
        
        // TODO: use entity type prefab id
        // TODO: use position

        return true;
    }

    bool HandleMessage( SyncedEntityMap_T& map, const Messages::ServerClientEntitySync& msg )
    {
        const auto entity_it = map.find( msg.entity_sync_id );
        if (entity_it == std::end( map ))
        {
            LOG_ERROR( LoggingChannels::Client, "Got entity sync data for an unrecognised sync id '{}'", msg.entity_sync_id );
            return false;
        }

        Buffer_T data{ msg.GetBlockData(), msg.GetBlockData() + msg.GetBlockSize() };
        UpdateEntity( entity_it->second, data );
        return true;
    }

    bool HandleMessage( SyncedEntityMap_T& map, const Messages::ServerClientRemoveEntity& msg )
    {
        const auto found_it = map.find( msg.entity_sync_id );
        if (found_it != std::end( map ))
        {
            found_it->second.destroy();
            map.erase( found_it );
        }
        // fine to ignore ids we didn't find

        return true;
    }
}
