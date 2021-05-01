#pragma once

#include <string>

namespace Networking
{
	struct Address
	{
		std::string value;
		int port;

		Address() : value( "" ), port( 0 ) {}
		Address( std::string_view address ) : value( address ), port( 0 ) {}
		Address( std::string_view address, int port ) : value( address ), port( port ) {}
	};
}
