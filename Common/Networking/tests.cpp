#include "MessageFactory.hpp"

#include <string_view>

namespace
{
	constexpr bool strings_equal( char const* a, char const* b )
	{
		return std::string_view( a ) == b;
	}
}

struct Msg1 : public ::Networking::Message { static constexpr std::string_view GetName() noexcept { return "Msg1"; } };
struct Msg2 : public ::Networking::Message { static constexpr std::string_view GetName() noexcept { return "Msg2"; } };
struct Msg3 : public ::Networking::Message { static constexpr std::string_view GetName() noexcept { return "Msg3"; } };
struct MsgOther : public ::Networking::Message {};

using TestFactory_T = Networking::MessageFactory< Msg1, Msg2, Msg3 >;
TestFactory_T factory;

void Test1()
{
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

//static_assert(std::is_same< std::unique_ptr<Msg1>, decltype(std::declval<TestFactory_T&>().<0>()) >::value, "CreateMessageByType implemenation is wrong");

static_assert(std::is_same< std::unique_ptr<::Networking::Message>, decltype(std::declval<TestFactory_T&>().CreateUntypedMessage( 0 )) >::value, "CreateMessage by message type number implemenation is wrong");
