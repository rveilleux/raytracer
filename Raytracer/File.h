#pragma once

#include <memory>
#include "Dynarray.h"
#include "Strings.h"

void SaveToFile(const std::string& filename, const TStrings& strings);

class FileInternal;

class File {
public:
	File(const std::string& filename, bool binary);
	~File();
	void SaveStrings(const TStrings& strings);
	void SaveBytes(const TBytes& bytes);
private:
	std::unique_ptr<FileInternal> _fileInternal;
};
