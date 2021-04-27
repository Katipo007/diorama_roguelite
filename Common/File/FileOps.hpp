#pragma once

#include <fstream>
#include <iostream>
#include <filesystem>

#include "Filepath.hpp"

// stream string operations derived from:
// Optimized C++ by Kurt Guntheroth (O’Reilly).
// Copyright 2016 Kurt Guntheroth, 978-1-491-92206-4
// Updated to c++17 by Benjamin Edwards, 2020

namespace FileOps
{
	bool StreamReadString( std::istream& file, std::string& out_file_contents );

	bool ReadFile( const Filepath& filename, std::string& out_file_contents );
	inline bool ReadFile( std::string_view filename, std::string& out_file_contents ) { return ReadFile( Filepath( filename ), out_file_contents ); }

	std::vector<std::string> GetFilesInFolder( const Filepath& path );
	inline std::vector<std::string> GetFilesInFolder( std::string_view path ) { return GetFilesInFolder( Filepath( path ) ); }

	Filepath GetFileDirectory( const Filepath& filepath, bool relative_to_working_directory = true );
	inline Filepath GetFileDirectory( std::string_view filepath, bool relative_to_working_directory = true ) { return GetFileDirectory( Filepath( filepath ), relative_to_working_directory ); }
}
