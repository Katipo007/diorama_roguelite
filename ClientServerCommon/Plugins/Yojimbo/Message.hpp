#pragma once

#include "impl/YojimboHeader.hpp"
#include "Types.hpp"

namespace YojimboPlugin
{
	class MessageFactory;

	struct Message
		: public yojimbo::Message
	{
		friend class MessageFactory;

	public:
		virtual ~Message() {}

		MessageType GetType() const;
		MessageFactory& GetFactory();
		const MessageFactory& GetFactory() const;

	protected:
		MessageFactory* owner = nullptr;
	};
}
