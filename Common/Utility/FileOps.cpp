#include "Precompiled/pch.hpp"
#include "FileOps.hpp"

#include <filesystem>

namespace FileOps
{
	std::filesystem::path GetFileDirectory( const std::string& filepath, bool relative_to_working_directory )
	{
		const auto working_directory = std::filesystem::current_path();
		if( relative_to_working_directory )
		{
			auto path = std::filesystem::path( filepath );
			if( path.has_filename() )
				path = path.parent_path();

			const auto final_path = ( working_directory / path ).lexically_normal();

			auto[ root_end, nothing ] = std::mismatch( working_directory.begin(), working_directory.end(), final_path.begin() );

			if( root_end != working_directory.end() )
				throw std::runtime_error( "Given path is not relative to the working directory" );
			
			return std::filesystem::relative( final_path, working_directory );
		}
		else
		{
			const auto path = std::filesystem::path( filepath );

			if( path.has_filename() )
				return path.parent_path();
			else
				return path;
		}
	}
}
