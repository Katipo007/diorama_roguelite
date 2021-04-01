#pragma once

#include <fstream>
#include <iostream>
#include <filesystem>

// stream string operations derived from:
// Optimized C++ by Kurt Guntheroth (O’Reilly).
// Copyright 2016 Kurt Guntheroth, 978-1-491-92206-4
// Updated to c++17 by Benjamin Edwards, 2020

namespace FileOps
{
	inline static std::streamoff StreamSize( std::istream& file )
	{
		std::istream::pos_type current_pos = file.tellg();

		if( current_pos == std::istream::pos_type( -1 ) )
			return -1;

		file.seekg( 0, std::istream::end );
		std::istream::pos_type end_pos = file.tellg();
		file.seekg( current_pos );
		return end_pos - current_pos;
	}

	inline bool StreamReadString( std::istream& file, std::string& file_contents )
	{
		std::streamoff len = StreamSize( file );
		if( len == -1 )
			return false;

		file_contents.resize( static_cast<std::string::size_type>( len ) );

		file.read( &file_contents[ 0 ], file_contents.length() );
		return true;
	}

	inline bool ReadFile( std::string_view filename, std::string& file_contents )
	{
		std::ifstream file( filename, std::ios::binary );

		if( !file.is_open() )
			return false;

		const bool success = StreamReadString( file, file_contents );

		file.close();

		return success;
	}

	inline std::vector<std::string> GetFilesInFolder( const std::string& path )
	{
		auto filenames = std::vector<std::string>();
		for( const auto & entry : std::filesystem::directory_iterator( path ) )
			filenames.emplace_back( std::move( entry.path().string() ) );

		return filenames;
	}

	std::filesystem::path GetFileDirectory( const std::string& filepath, bool relative_to_working_directory = true );
}
