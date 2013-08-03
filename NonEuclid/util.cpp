#include "physfs.h"

#include <string>

#include "util.h"

std::string file_to_string(const char* filename)
{
	PHYSFS_file* file = PHYSFS_openRead( filename );

	int file_length = PHYSFS_fileLength(file);

	char* buffer;
	buffer = new char[ file_length ];
	int length_read = PHYSFS_read ( file, buffer, 1, file_length );

	std::string result;
	result.append(buffer, length_read);

	delete buffer;

	PHYSFS_close( file );

	return result;
}