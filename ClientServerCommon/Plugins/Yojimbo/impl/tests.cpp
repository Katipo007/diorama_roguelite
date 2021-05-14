#include "../Basic/TemplateMessageFactory.hpp"
#include "../Message.hpp"
#include "../impl/YojimboHeader.hpp"

static constexpr bool strings_equal( char const* a, char const* b ) { return std::string_view( a ) == b; }

using namespace YojimboPlugin;

#define BASIC_MESSAGE( name ) \
	struct name : public Message \
	{ \
		static constexpr std::string_view GetName() noexcept { return #name; } \
		\
		template<typename STREAM> \
		bool Serialize( STREAM& ) { return true; } \
		\
		YOJIMBO_VIRTUAL_SERIALIZE_FUNCTIONS(); \
	};

BASIC_MESSAGE( Msg1 );
BASIC_MESSAGE( Msg2 );
BASIC_MESSAGE( Msg3 );
BASIC_MESSAGE( MsgOther );

using TestFactory_T = TemplateMessageFactory< Msg1, Msg2, Msg3 >;

void Test1()
{
	TestFactory_T factory;

	auto msg1 = factory.CreateMessage<Msg1>();
	assert( msg1 != nullptr );
	auto msg2 = factory.CreateMessage<Msg2>();
	assert( msg2 != nullptr );
	auto msg3 = factory.CreateMessage<Msg3>();
	assert( msg3 != nullptr );

	const auto x = factory.CreateUntypedMessage( 4 );
	ASSERT( x == nullptr );

	ASSERT( factory.GetMessageName( 0 ) == "Msg1" );

	//auto invalid_idx = factory.GetMessageType<MsgOther>();
	//assert( invalid_idx );
	//static_assert(TestFactory_T::GetMessageType<MsgOther>() == 0, "GetMessageType implementation is wrong!");
}

static_assert(TestFactory_T::GetMessageType<Msg1>() == 0, "GetMessageType implementation is wrong!");
static_assert(TestFactory_T::GetMessageType<Msg2>() != 0, "GetMessageType implementation is wrong!");
static_assert(TestFactory_T::GetMessageType<Msg2>() == 1, "GetMessageType implementation is wrong!");
static_assert(TestFactory_T::GetMessageType<Msg3>() == 2, "GetMessageType implementation is wrong!");


static_assert(std::is_same<Message*, decltype(std::declval<TestFactory_T&>().CreateUntypedMessage( 0 )) >::value, "CreateMessage by message type number implemenation is wrong");
