#include "FileOps.hpp"

namespace
{
	static std::streamoff StreamSize( std::istream& file )
	{
		std::istream::pos_type current_pos = file.tellg();

		if (current_pos == std::istream::pos_type( -1 ))
			return -1;

		file.seekg( 0, std::istream::end );
		std::istream::pos_type end_pos = file.tellg();
		file.seekg( current_pos );
		return end_pos - current_pos;
	}
}

namespace FileOps
{
	bool StreamReadString( std::istream& file, std::string& out_file_contents )
	{
		std::streamoff len = StreamSize( file );
		if (len == -1)
			return false;

		out_file_contents.resize( static_cast<std::string::size_type>(len) );

		file.read( &out_file_contents[0], out_file_contents.length() );
		return true;
	}

	bool ReadFile( const Filepath& filename, std::string& file_contents )
	{
		std::ifstream file( filename, std::ios::binary );

		if (!file.is_open())
			return false;

		const bool success = StreamReadString( file, file_contents );

		file.close();

		return success;
	}

	std::vector<std::string> GetFilesInFolder( const Filepath& path )
	{
		auto filenames = std::vector<std::string>();
		for (const auto& entry : std::filesystem::directory_iterator( path ))
			filenames.emplace_back( std::move( entry.path().string() ) );

		return filenames;
	}

	Filepath GetFileDirectory( const Filepath& filepath, bool relative_to_working_directory )
	{
		const auto working_directory = std::filesystem::current_path();
		if( relative_to_working_directory )
		{
			auto path = Filepath( filepath );
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
			const auto path = Filepath( filepath );

			if( path.has_filename() )
				return path.parent_path();
			else
				return path;
		}
	}
}
