#include "ClientSyncHelpers.hpp"

#include "ClientServerCommon/Game/ClientSync/ClientSyncMessages.hpp"
#include "ClientServerCommon/Game/ClientSync/SyncableComponents.hpp"
#include "Client/Game/Sprite/SpriteComponent.hpp"
#include "Common/DataTypes/Bytes.hpp"
#include "Common/DataTypes/Vec3.hpp"

namespace Game::ClientSync::Helpers
{
    namespace
    {
        template<SerialisableComponent C, SerialisableComponent As = C>
        inline void ReadComponent( As& component, Deserialiser& deserialiser, const bool expects_identifier = false )
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

        template<SerialisableComponent C, SerialisableComponent As = C>
        void ReadComponent( const ecs::EntityHandle entity, Deserialiser& deserialiser, const bool expects_identifier = false )
        {
            ASSERT( !!entity );
            auto& component = entity.get_or_emplace<As>();
            return ReadComponent<C, As>( component, deserialiser, expects_identifier );
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
                    ReadComponent<Name::NameComponent>( entity, deserialiser );
                    break;

                case ComponentIdentifiers::type<Sprite::CommonSpriteComponent>:
                    ReadComponent<Sprite::CommonSpriteComponent, Sprite::SpriteComponent>( entity, deserialiser );
                    break;

                case ComponentIdentifiers::type<Transform::PositionComponent>:
                    ReadComponent<Transform::PositionComponent>( entity, deserialiser );
                    break;

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

        LOG_TRACE( LoggingChannels::Client, "Adding entity with sync id '{}'", msg.entity_sync_id );
        const auto entity = registry.create();
        map[msg.entity_sync_id] = ecs::EntityHandle{ registry, entity };
        
        // TODO: use entity type prefab id
        // TODO: use position

        return true;
    }

    bool HandleMessage( SyncedEntityMap_T& map, const Messages::ServerClientEntitySync& msg, const Networking::ChannelType channel )
    {
        const auto entity_it = map.find( msg.entity_sync_id );
        if (entity_it == std::end( map ))
        {
            if (channel == Networking::ChannelType::Unreliable)
                return true; // safe to ignore?

            LOG_ERROR( LoggingChannels::Client, "Got entity sync data for an unrecognised sync id '{}'", msg.entity_sync_id );
            return false;
        }

        //LOG_TRACE( LoggingChannels::Client, "Updating entity with sync id '{}'", msg.entity_sync_id );
        Buffer_T data{ msg.GetBlockData(), msg.GetBlockData() + msg.GetBlockSize() };
        UpdateEntity( entity_it->second, data );
        return true;
    }

    bool HandleMessage( SyncedEntityMap_T& map, const Messages::ServerClientRemoveEntity& msg )
    {
        for (auto& sync_id : msg.entity_sync_ids)
        {
            const auto found_it = map.find( sync_id );
            if (found_it != std::end( map ))
            {
                LOG_TRACE( LoggingChannels::Client, "Removing entity with sync id '{}'", sync_id );
                found_it->second.destroy();
                map.erase( found_it );
            }
            // fine to ignore ids we didn't find
        }

        return true;
    }
}
