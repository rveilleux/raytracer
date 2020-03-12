#include "File.h"
#include <fstream>

namespace {
	constexpr int GetFileMode(bool binary) {
		int mode = std::ofstream::out;
		if (binary) mode |= std::ofstream::binary;
		return mode;
	}
};

class FileInternal {
public:
	FileInternal(const std::string& filename, bool binary) {
		_stream.open(filename, GetFileMode(binary));
	}

	~FileInternal() {
		_stream.close();
	}

	void WriteString(const std::string& string) {
		_stream << string << std::endl;
	}

	void SaveBytes(const TBytes& bytes) {
		_stream.write(reinterpret_cast<const char*>(bytes.GetData()), bytes.GetSize());
	}

private:
	std::ofstream _stream;

};

File::File(const std::string& filename, bool binary)
	: _fileInternal(std::make_unique<FileInternal>(filename, binary))
{
}

File::~File() {
}

void File::SaveStrings(const TStrings& strings) {
	for (const auto& string : strings) {
		_fileInternal->WriteString(string);
	}
}

void File::SaveBytes(const TBytes& bytes) {
	_fileInternal->SaveBytes(bytes);
}

void SaveToFile(const std::string& filename, const TStrings& strings) {
	File file(filename, false);
	file.SaveStrings(strings);
}
