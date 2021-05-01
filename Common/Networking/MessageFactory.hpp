#pragma once

#include <array>
#include <exception>
#include <memory>
#include <type_traits>
#include <tuple>

#include "Message.hpp"

namespace Networking
{
	namespace __detail
	{
		template <class T, class Tuple>
		struct index
		{
			static_assert(!std::is_same_v<Tuple, std::tuple<>>, "Could not find `T` in given `Tuple`");
		};

		template <class T, class... Types>
		struct index<T, std::tuple<T, Types...>>
		{
			static const std::size_t value = 0;
		};

		template <class T, class U, class... Types>
		struct index<T, std::tuple<U, Types...>>
		{
			static const std::size_t value = 1 + index<T, std::tuple<Types...>>::value;
		};

		template< bool... Bs >
		using bool_sequence = std::integer_sequence< bool, Bs... >;

		template< bool... Bs >
		using bool_and = std::is_same< bool_sequence< Bs... >,
			bool_sequence< (Bs || true)... > >;

		template<class... B>
		using or_ = std::disjunction<B...>;

		template <typename T, typename Tuple>
		struct has_type;

		template <typename T, typename... Us>
		struct has_type<T, std::tuple<Us...>> : or_<std::is_same<T, Us>...> {};
	}

	class IMessageFactory
	{
	public:
		virtual ~IMessageFactory() {}

		virtual size_t GetNumMessageTypes() const noexcept = 0;
		virtual bool HasType( const MessageType ) const noexcept = 0;
		virtual std::unique_ptr<::Networking::Message> CreateUntypedMessage( const MessageType ) = 0;

		virtual std::string_view GetMessageName( const MessageType ) const = 0;
		virtual size_t GetSizeOfMessageType( const MessageType ) const = 0;
	};


	template<class... MESSAGES>
	class MessageFactory final
		: public IMessageFactory
	{
		static_assert(__detail::bool_and< std::is_base_of< ::Networking::Message, MESSAGES >::value... >::value, "All message types must derive from ::Networking::Message");
		static_assert(__detail::bool_and< std::is_default_constructible< MESSAGES >::value... >::value, "All message types must be default constructable");
		static_assert(__detail::bool_and< std::is_same< std::string_view, decltype(std::declval<MESSAGES&>().GetName()) >::value... >::value, "All message types must provide a `static constexpr std::string_view GetName() noexcept` method");

		using MessageTypesTuple = std::tuple<MESSAGES...>;
		static constexpr size_t NumMessageTypes = std::tuple_size<MessageTypesTuple>::value;

		using MessageConstructor_T = std::function<std::unique_ptr<Message>( void )>;

	public:
		MessageFactory() = default;
		~MessageFactory() = default;

	public: // overrides
		size_t GetNumMessageTypes() const noexcept override { return NumMessageTypes; }
		bool HasType( const MessageType type ) const noexcept override { return message_constructors.size() < type; }

		std::unique_ptr<::Networking::Message> CreateUntypedMessage( const MessageType type ) override
		{
			ASSERT( (type >= 0) && (type < NumMessageTypes), "Invalid message index" );
			if ((type >= 0) && (type < NumMessageTypes))
				throw std::out_of_range( "Factory has no message type '" + std::to_string( type ) + "'" );
			
			const auto& constructor = message_constructors.at( type );
			return constructor();
		}

		std::string_view GetMessageName( const MessageType type ) const override
		{
			ASSERT( (type >= 0) && (type < NumMessageTypes), "Invalid message index" );
			if ((type >= 0) && (type < NumMessageTypes))
				throw std::out_of_range( "Factory has no message type '" + std::to_string( type ) + "'" );

			return message_names.at( type );
		}

		size_t GetSizeOfMessageType( const MessageType type ) const override
		{
			ASSERT( (type >= 0) && (type < NumMessageTypes), "Invalid message index" );
			if ((type >= 0) && (type < NumMessageTypes))
				throw std::out_of_range( "Factory has no message type '" + std::to_string( type ) + "'" );

			return message_sizes.at( type );
		}

	public: // static helpers
		template<class MESSAGE_T>
		static constexpr bool HasType() noexcept { return __detail::has_type<MESSAGE_T, MessageTypesTuple>::value; }

		template<class MESSAGE_T>
		static constexpr MessageType GetMessageType() noexcept
		{
			static_assert(__detail::has_type<MESSAGE_T, MessageTypesTuple>::value, "Message type is invalid for this factory");
			return __detail::index<MESSAGE_T, MessageTypesTuple>::value;
		}

		template<class MESSAGE_T, typename... Args>
		static std::unique_ptr<MESSAGE_T> CreateMessage( Args&&... args )
		{
			static_assert(__detail::has_type<MESSAGE_T, MessageTypesTuple>::value, "Message type is invalid for this factory");
			return std::make_unique<MESSAGE_T>( std::forward<Args>( args )... );
		}

	private:
		const std::array<MessageConstructor_T, NumMessageTypes> message_constructors = {
			[]() -> std::unique_ptr<::Networking::Message> { return std::make_unique<MESSAGES>(); }...
		};

		const std::array<size_t, NumMessageTypes> message_sizes = {
			sizeof( MESSAGES )...
		};

		const std::array<std::string_view, NumMessageTypes> message_names = {
			MESSAGES::GetName()...
		};
	};
}
