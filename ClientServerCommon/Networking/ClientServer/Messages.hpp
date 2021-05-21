#pragma once

#include "ClientServerCommon/Plugins/Yojimbo/YojimboHeader.hpp"

namespace Networking::ClientServer
{
#define START_MESSAGE( name ) struct name : public yojimbo::Message { static constexpr std::string_view GetName() { return #name; }
#define END_MESSAGE YOJIMBO_VIRTUAL_SERIALIZE_FUNCTIONS(); }
#define BASIC_MESSAGE( name ) START_MESSAGE( name ) template<typename STREAM> bool Serialize( STREAM& ) { return true; } END_MESSAGE


	///
	/// DEFINE MESSAGES HERE
	///
	
	BASIC_MESSAGE( DummyMessage );


	START_MESSAGE( ClientServerChatMessage )

		std::array<char, 256> message;

		template<typename STREAM>
		bool Serialize( STREAM& stream )
		{
			serialize_string( stream, &message[0], static_cast<int>(std::size( message )) );
			return true;
		}
		
	END_MESSAGE;


	/// 
	/// STOP DEFINING MESSAGES
	/// 

#undef BASIC_MESSAGE
#undef END_MESSAGE
#undef START_MESSAGE
}
