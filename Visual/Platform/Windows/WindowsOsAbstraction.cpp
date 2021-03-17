#include "Common/Utility/OsAbstraction.hpp"

#ifdef PLATFORM_WINDOWS
#include "WindowsHeader.hpp"

namespace
{
	// UT8 conversion functions sourced here: https://stackoverflow.com/a/3999597, only C++11 and up
	// note that WideCharToMultiByte and MultiByteToWideChar are windows only functions

	// Convert a wide Unicode string to an UTF8 string
	std::string utf8_encode( const std::wstring& wstr )
	{
		if (wstr.empty()) return std::string();
		int size_needed = WideCharToMultiByte( CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL );
		std::string strTo( size_needed, 0 );
		WideCharToMultiByte( CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL );
		return strTo;
	}

	// Convert an UTF8 string to a wide Unicode String
	std::wstring utf8_decode( const std::string& str )
	{
		if (str.empty()) return std::wstring();
		int size_needed = MultiByteToWideChar( CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0 );
		std::wstring wstrTo( size_needed, 0 );
		MultiByteToWideChar( CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed );
		return wstrTo;
	}
}

namespace OS
{
	void ShowMessageBox( const std::string& message_str, const std::string& title_str, const MessageBoxTypes type )
	{
		std::wstring title = utf8_decode( title_str );
		std::wstring msg = utf8_decode( message_str );

		UINT flags = MB_APPLMODAL;
		switch (type)
		{
		case MessageBoxTypes::Info:
			flags |= MB_OK | MB_ICONINFORMATION;
			break;

		case MessageBoxTypes::Warning:
			flags |= MB_OK | MB_ICONWARNING;
			break;

		case MessageBoxTypes::Error:
			flags |= MB_OK | MB_ICONERROR;
			break;
		}

		::MessageBoxW( NULL, msg.c_str(), title.empty() ? NULL : title.c_str(), flags );
	}
}

#endif

