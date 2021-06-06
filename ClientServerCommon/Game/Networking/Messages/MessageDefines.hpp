#include "ClientServerCommon/Plugins/Yojimbo/YojimboHeader.hpp"
#include "ClientServerCommon/Plugins/Yojimbo/Concepts.hpp"

#define BEGIN_MESSAGE( name ) struct name : public yojimbo::Message { static constexpr std::string_view GetName() { return #name; }
#define END_MESSAGE() YOJIMBO_VIRTUAL_SERIALIZE_FUNCTIONS() }
#define BEGIN_BLOCK_MESSAGE( name ) struct name : public yojimbo::BlockMessage { static constexpr std::string_view GetName() { return #name; }
#define END_BLOCK_MESSAGE() YOJIMBO_VIRTUAL_SERIALIZE_FUNCTIONS() }

#define DEFINE_EMPTY_MESSAGE( name ) BEGIN_MESSAGE( name ) template<YojimboPlugin::Concepts::SerializeStream STREAM> bool Serialize( STREAM& ) { return true; } END_MESSAGE()
