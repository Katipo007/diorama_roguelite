#pragma once

#include <array>
#include <exception>
#include <type_traits>
#include <tuple>
#include <string_view>

#include "../Message.hpp"
#include "../MessageFactory.hpp"

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
		: public MessageFactory
		, private yojimbo::MessageFactory
	{
		static_assert(__detail::bool_and< std::is_base_of< Message, MESSAGES >::value... >::value, "All message types must derive from ::Networking::Message");
		static_assert(__detail::bool_and< std::is_default_constructible< MESSAGES >::value... >::value, "All message types must be default constructable");
		static_assert(__detail::bool_and< std::is_same< std::string_view, decltype(std::declval<MESSAGES&>().GetName()) >::value... >::value, "All message types must provide a `static constexpr std::string_view GetName() noexcept` method");

		using MessageTypesTuple_T = std::tuple<MESSAGES...>;
		static constexpr size_t NumMessageTypes = std::tuple_size<MessageTypesTuple_T>::value;

	public:
		TemplateMessageFactory() : yojimbo::MessageFactory( yojimbo::GetDefaultAllocator(), NumMessageTypes ) {}
		~TemplateMessageFactory() {}

	public: // overrides
		size_t GetNumMessageTypes() const noexcept override { return NumMessageTypes; }
		bool HasType( const MessageType type ) const noexcept override { return (type >= 0) && (type < NumMessageTypes); }

		Message* CreateUntypedMessage( const MessageType type ) override
		{
			ASSERT( (type >= 0) && (type < NumMessageTypes), "Invalid message index" );
			return dynamic_cast<Message*>(::yojimbo::MessageFactory::CreateMessage( type ));
		}

		std::string_view GetMessageName( const MessageType type ) const override
		{
			ASSERT( (type >= 0) && (type < NumMessageTypes), "Invalid message index" );
			return message_names.at( type );
		}

	public: // static helpers
		template<class MESSAGE_T>
		static constexpr bool HasType() noexcept { return __detail::has_type<MESSAGE_T, MessageTypesTuple_T>::value; }

		template<class MESSAGE_T>
		MESSAGE_T* CreateMessage()
		{
			return dynamic_cast<MESSAGE_T*>( ::yojimbo::MessageFactory::CreateMessage( GetMessageType<MESSAGE_T>() ) );
		}

		template<class MESSAGE_T>
		static constexpr MessageType GetMessageType() noexcept
		{
			static_assert(__detail::has_type<MESSAGE_T, MessageTypesTuple_T>::value, "Message type is invalid for this factory");
			return __detail::index<MESSAGE_T, MessageTypesTuple_T>::value;
		}

		template<class MESSAGE_T>
		static constexpr std::string_view GetMessageName() noexcept
		{
			static_assert(__detail::has_type<MESSAGE_T, MessageTypesTuple_T>::value, "Message type is invalid for this factory");
			return message_names[GetMessageType<MESSAGE_T>()];
		}

	private:
		virtual yojimbo::Message* CreateMessageInternal( int type ) override
		{
			if (!HasType( type ))
				return NULL;

			yojimbo::Allocator& allocator = GetAllocator();
			yojimbo::Message* const message = message_constructors[type]( allocator );
			if (!message)
				return NULL;

			SetMessageType( message, type );
			return message;
		}

	private:
		inline static constexpr std::array<std::string_view, NumMessageTypes> message_names = { MESSAGES::GetName()... };

		using InternalMessageConstructorFunc_T = std::function<yojimbo::Message* (yojimbo::Allocator&)>;
		inline static const std::array<InternalMessageConstructorFunc_T, NumMessageTypes> message_constructors = { []( yojimbo::Allocator& a ) { return YOJIMBO_NEW( a, MESSAGES ); }... };
	};
}
