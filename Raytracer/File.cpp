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
		try {
			_stream.close();
		}
		catch (...) {
			_ASSERT_EXPR(false, L"Can't close stream");
		}
	}

	FileInternal() = delete;
	FileInternal(const FileInternal&) = delete;
	FileInternal(FileInternal&&) = delete;
	FileInternal& operator=(const FileInternal&) = delete;
	FileInternal& operator=(FileInternal&&) = delete;

	void WriteString(const std::string& string) {
		_stream << string << std::endl;
	}

#pragma warning (push)
#pragma warning (disable: 26490)
	void SaveBytes(const TBytes& bytes) {
		// There is no easy way to get rid of this ugly cast:
		// changing to a basic_ofstream<uint8_t> would work but we'd lose the possibly
		// of writing WriteString() to such stream.
		_stream.write(reinterpret_cast<const char*>(bytes.GetData()), bytes.GetSize());
	}
#pragma warning (pop)

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
