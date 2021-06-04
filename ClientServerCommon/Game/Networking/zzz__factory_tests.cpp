#include "MessageFactory.hpp"

namespace
{
#if 1 // Some random tests stuff

	struct Msg1
		: yojimbo::Message
	{
		static constexpr std::string_view GetName() { return "Msg1"; }

		template<typename STREAM>
		bool Serialize( STREAM& ) { return true; }

		YOJIMBO_VIRTUAL_SERIALIZE_FUNCTIONS();
	};
	struct Msg2
		: yojimbo::Message
	{
		static constexpr std::string_view GetName() { return "Msg2"; }

		template<typename STREAM>
		bool Serialize( STREAM& ) { return true; }

		YOJIMBO_VIRTUAL_SERIALIZE_FUNCTIONS();
	};
	struct MsgWrong
		: yojimbo::Message
	{
		static constexpr std::string_view GetName() { return "MsgWrong"; }

		template<typename STREAM>
		bool Serialize( STREAM& ) { return true; }

		YOJIMBO_VIRTUAL_SERIALIZE_FUNCTIONS();
	};

	using TestMessageFactory = YojimboPlugin::TemplateMessageFactory<Msg1, Msg2>;

#pragma warning(push)
#pragma warning(disable:4505)
	static void Test1()
	{
		TestMessageFactory factory( yojimbo::GetDefaultAllocator() );

		static_assert(TestMessageFactory::NumMessageTypes == 2);
		static_assert(TestMessageFactory::HasMessageType( 0 ) == true);
		static_assert(TestMessageFactory::HasMessageType( 1 ) == true);
		static_assert(TestMessageFactory::HasMessageType( 2 ) == false);

		static_assert(TestMessageFactory::GetMessageName<Msg1>() == "Msg1");
		static_assert(TestMessageFactory::GetMessageName( 0 ) == "Msg1");
		static_assert(TestMessageFactory::GetMessageName<Msg2>() == "Msg2");
		static_assert(TestMessageFactory::GetMessageName( 1 ) == "Msg2");

		static_assert(TestMessageFactory::GetMessageType<Msg1>() == 0);
		static_assert(TestMessageFactory::GetMessageType<Msg2>() == 1);

		Msg1* msg1 = factory.CreateMessage<Msg1>();
		auto* msg1u = factory.CreateUntypedMessage( 0 );
		Msg2* msg2 = factory.CreateMessage<Msg2>();
		auto* msg2u = factory.CreateUntypedMessage( 1 );

		auto* msgWrongu = factory.CreateUntypedMessage( 2 );

		factory.ReleaseMessage( msg1 );
		factory.ReleaseMessage( msg1u );
		factory.ReleaseMessage( msg2 );
		factory.ReleaseMessage( msg2u );
		factory.ReleaseMessage( msgWrongu );

		//
		// these ones should not compile
		//
#ifdef __INTELLISENSE__
		static_assert(TestMessageFactory::GetMessageName<MsgWrong>() == "MsgWrong");
		static_assert(TestMessageFactory::GetMessageType<MsgWrong>() == 0);

		MsgWrong* msgWrong = factory.CreateMessage<MsgWrong>();
		factory.ReleaseMessage( msgWrong );
#endif
	}
#pragma warning(pop)
#endif
}
