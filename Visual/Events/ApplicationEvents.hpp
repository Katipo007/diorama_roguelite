#pragma once

#include "Event.hpp"

namespace Visual::Device
{
	class Window;
}

namespace Visual::Device::ApplicationEvents
{
	template<class DERIVED>
	class BaseWindowEvent
		: public CompileTimeEvent< DERIVED >
	{
	public:
		BaseWindowEvent( Window& sender )
			: sender( sender )
		{}

		~BaseWindowEvent() override = default;

		Window& GetSender() { return sender; }
		const Window& GetSender() const { return sender; }

		static EventCategory GetStaticCategoryFlags() { return EventCategory::Application; }

	protected:
		Window& sender;
	};

	class WindowResized
		: public BaseWindowEvent< WindowResized >
	{
	public:
		WindowResized( Window& sender, uint16_t width, uint16_t height )
			: BaseWindowEvent( sender )
			, width( width )
			, height( height )
		{}

		~WindowResized() override = default;

		const uint16_t& GetWidth() const { return width; }
		const uint16_t& GetHeight() const { return height; }

		inline virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: " << width << ", " << height;
			return ss.str();
		}

		static EventType GetStaticType() { return EventType::WindowResize; }
		static EventCategory GetStaticCategoryFlags() { return EventCategory::Application; }

	private:
		uint16_t width;
		uint16_t height;
	};

	class WindowClose
		: public BaseWindowEvent< WindowClose >
	{
	public:
		WindowClose( Window& sender )
			: BaseWindowEvent( sender )
		{}
		~WindowClose() override = default;

		static EventType GetStaticType() { return EventType::WindowClose; }
		static EventCategory GetStaticCategoryFlags() { return EventCategory::Application; }
	};

	class ApplicationTick
		: public CompileTimeEvent< ApplicationTick >
	{
	public:
		ApplicationTick() = default;
		~ApplicationTick() override = default;

		static EventType GetStaticType() { return EventType::ApplicationTick; }
		static EventCategory GetStaticCategoryFlags() { return EventCategory::Application; }
	};

	class ApplicationUpdate
		: public CompileTimeEvent< ApplicationUpdate >
	{
	public:
		ApplicationUpdate() = default;
		~ApplicationUpdate() override = default;

		static EventType GetStaticType() { return EventType::ApplicationUpdate; }
		static EventCategory GetStaticCategoryFlags() { return EventCategory::Application; }
	};

	class ApplicationRender
		: public CompileTimeEvent< ApplicationRender >
	{
	public:
		ApplicationRender() = default;
		~ApplicationRender() override = default;

		static EventType GetStaticType() { return EventType::ApplicationRender; }
		static EventCategory GetStaticCategoryFlags() { return EventCategory::Application; }
	};
}
