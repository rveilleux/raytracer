#pragma once

#include <memory>
#include "Dynarray.h"
#include "Strings.h"

void SaveToFile(const std::string& filename, const TStrings& strings);

class FileInternal;

class File {
public:
	File(const std::string& filename, bool binary);
	// The destructor is required in order for unique_ptr pimpl to compile correctly
	~File();
	File(const File&) = delete;
	File(File&&) = delete;
	File& operator=(const File&) = delete;
	File& operator=(File&&) = delete;
	void SaveStrings(const TStrings& strings);
	void SaveBytes(const TBytes& bytes);
private:
	std::unique_ptr<FileInternal> _fileInternal;
};
