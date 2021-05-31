#pragma once

typedef unsigned int APIType;

namespace CoreAPIs
{
	enum Type : APIType
	{
		/// should be defined in the order of importance.
		/// i.e. Most important at top, ones which depend on others lower.

		System = 0,
		Input,
		Video,
		DearImGui,

		User, // additional plugins external to the engine should have a unique ID of User + some index
	};
}
