#pragma once

#include <array>
#include <exception>
#include <type_traits>
#include <tuple>
#include <string_view>

#include "YojimboHeader.hpp"
#include "Types.hpp"

namespace YojimboPlugin
{
	namespace __detail
	{
		template <class T, class Tuple>
		struct index { static_assert(!std::is_same_v<Tuple, std::tuple<>>, "Could not find `T` in given `Tuple`"); };

		template <class T, class... Types>
		struct index<T, std::tuple<T, Types...>> { static const std::size_t value = 0; };

		template <class T, class U, class... Types>
		struct index<T, std::tuple<U, Types...>> { static const std::size_t value = 1 + index<T, std::tuple<Types...>>::value; };

		template< bool... Bs >
		using bool_sequence = std::integer_sequence< bool, Bs... >;

		template< bool... Bs >
		using bool_and = std::is_same< bool_sequence< Bs... >, bool_sequence< (Bs || true)... > >;

		template<class... B>
		using or_ = std::disjunction<B...>;

		template <typename T, typename Tuple>
		struct has_type;

		template <typename T, typename... Us>
		struct has_type<T, std::tuple<Us...>> : or_<std::is_same<T, Us>...> {};
	}

	template<class... MESSAGES>
	class TemplateMessageFactory final
		: public yojimbo::MessageFactory
	{
		static_assert(__detail::bool_and< std::is_base_of< ::yojimbo::Message, MESSAGES >::value... >::value, "All message types must derive from yojimbo::Message");
		static_assert(__detail::bool_and< std::is_default_constructible< MESSAGES >::value... >::value, "All message types must be default constructable");
		static_assert(__detail::bool_and< std::is_same< std::string_view, decltype(std::declval<MESSAGES&>().GetName()) >::value... >::value, "All message types must provide a `static constexpr std::string_view GetName() noexcept` method");

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
		template<class MESSAGE_T>
		inline static constexpr bool HasMessageType() noexcept { HasMessageType( GetMessageType<MESSAGE_T>() ); }

		template<class MESSAGE_T>
		inline static constexpr MessageType_T GetMessageType() noexcept
		{
			static_assert(__detail::has_type<MESSAGE_T, MessageTypesTuple_T>::value, "Message type does not belong to this factory");
			return __detail::index<MESSAGE_T, MessageTypesTuple_T>::value;
		}


	public: // Message Creation

		inline yojimbo::Message* CreateUntypedMessage( const MessageType_T type ) { return ::yojimbo::MessageFactory::CreateMessage( type ); }

		template<class MESSAGE_T>
		inline MESSAGE_T* CreateMessage() { return dynamic_cast<MESSAGE_T*>(CreateUntypedMessage( GetMessageType<MESSAGE_T>() )); }


	public: // Message Names

		static constexpr std::string_view GetMessageName( const MessageType_T type ) noexcept { return (static_cast<size_t>(type) < NumMessageTypes) ? message_names.at( type ) : ""; }

		template<class MESSAGE_T>
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
