#pragma once

#include <array>
#include <exception>
#include <type_traits>
#include <tuple>
#include <string_view>

#include "Common/Utility/TupleReflection.hpp"

#include "YojimboHeader.hpp"
#include "Concepts.hpp"
#include "Types.hpp"

namespace YojimboPlugin
{
	template<Concepts::Message... MESSAGES>
	class TemplateMessageFactory final
		: public yojimbo::MessageFactory
	{
		using MessageTypesTuple_T = std::tuple<MESSAGES...>;

	public: // con/de-structors

		TemplateMessageFactory( yojimbo::Allocator& allocator )
			: yojimbo::MessageFactory( allocator, NumMessageTypes )
		{}
		~TemplateMessageFactory() = default;


	public: // Message Types

		/// Number of message types this factory can produce.
		static constexpr size_t NumMessageTypes = std::tuple_size<MessageTypesTuple_T>::value;
		static_assert(NumMessageTypes > 0, "Must have at least one message type");

		static constexpr bool HasMessageType( const MessageType_T type ) noexcept { return (type >= 0) && (type < NumMessageTypes); }
		template<Concepts::Message MESSAGE_T>
		inline static constexpr bool HasMessageType() noexcept { HasMessageType( GetMessageType<MESSAGE_T>() ); }

		template<Concepts::Message MESSAGE_T>
		inline static constexpr MessageType_T GetMessageType() noexcept
		{
			static_assert(TupleReflection::tuple_contains<MessageTypesTuple_T, MESSAGE_T>(), "Message type does not belong to this factory");
			return TupleReflection::tuple_index<MessageTypesTuple_T, MESSAGE_T>::value;
		}


	public: // Message Creation

		inline yojimbo::Message* CreateUntypedMessage( const MessageType_T type ) { return ::yojimbo::MessageFactory::CreateMessage( type ); }

		template<Concepts::Message MESSAGE_T>
		inline MESSAGE_T* CreateMessage() { return dynamic_cast<MESSAGE_T*>(CreateUntypedMessage( GetMessageType<MESSAGE_T>() )); }


	public: // Message Names

		static constexpr std::string_view GetMessageName( const MessageType_T type ) noexcept { return (static_cast<size_t>(type) < NumMessageTypes) ? message_names.at( type ) : ""; }

		template<Concepts::Message MESSAGE_T>
		inline static constexpr std::string_view GetMessageName() noexcept { return GetMessageName( GetMessageType<MESSAGE_T>() ); }


	private:

		virtual yojimbo::Message* CreateMessageInternal( int type )
		{
			if (!HasMessageType( type ))
				return NULL;

			yojimbo::Allocator& allocator = GetAllocator();
			if (yojimbo::Message* const message = message_constructors.at( type )(allocator))
			{
				SetMessageType( message, type );
				return message;
			}

			return NULL;
		}

	private:

		inline static constexpr std::array<std::string_view, NumMessageTypes> message_names = { MESSAGES::GetName()... };

		using InternalMessageConstructorFunc_T = std::function<yojimbo::Message* (yojimbo::Allocator&)>;
		inline static const std::array<InternalMessageConstructorFunc_T, NumMessageTypes> message_constructors = { []( yojimbo::Allocator& a ) { return YOJIMBO_NEW( a, MESSAGES ); }... };
	};
}
