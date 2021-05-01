#pragma once

namespace API
{
	enum class APIType
	{
		/// should be defined in the order of importance.
		/// i.e. Most important at top, ones which depend on others lower

		System,
		Input,
		Video,
		Network,
		DearImGui,

		NumAPITypes
	};
}
