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
	bool StreamReadString( std::istream& file, std::string& out_file_contents );

	bool ReadFile( const std::filesystem::path& filename, std::string& out_file_contents );
	inline bool ReadFile( std::string_view filename, std::string& out_file_contents ) { return ReadFile( std::filesystem::path( filename ), out_file_contents ); }

	std::vector<std::string> GetFilesInFolder( const std::filesystem::path& path );
	inline std::vector<std::string> GetFilesInFolder( std::string_view path ) { return GetFilesInFolder( std::filesystem::path( path ) ); }

	std::filesystem::path GetFileDirectory( const std::filesystem::path& filepath, bool relative_to_working_directory = true );
	inline std::filesystem::path GetFileDirectory( std::string_view filepath, bool relative_to_working_directory = true ) { return GetFileDirectory( std::filesystem::path( filepath ), relative_to_working_directory ); }
}
