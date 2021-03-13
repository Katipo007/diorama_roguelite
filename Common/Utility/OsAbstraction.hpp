#pragma once

#include <string>

namespace OS
{
	enum class MessageBoxTypes
	{
		Error,
		Warning,
		Info,

		NumTypes
	};

	void ShowMessageBox( const std::string& message, const std::string& title, const MessageBoxTypes type = MessageBoxTypes::Info );
}
