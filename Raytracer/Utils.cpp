#include "Utils.h"

File::File(const std::string& filename, bool binary) {
	int mode = std::ofstream::out;
	if (binary) mode = std::ofstream::out | std::ofstream::binary;
	_stream.open(filename,  mode);
}

File::~File() {
	_stream.close();
}

void File::SaveStrings(const TStrings& strings) {
	for (const auto& string : strings) {
		_stream << string << std::endl;
	}
}

void File::SaveBytes(const TBytes& bytes) {
	//_stream.write(reinterpret_cast<const char*>(bytes.data()), bytes.size());
	_stream.write(reinterpret_cast<const char*>(bytes.GetData()), bytes.GetSize());
}

void SaveToFile(const std::string& filename, const TStrings& strings) {
	File file(filename, false);
	file.SaveStrings(strings);
}
