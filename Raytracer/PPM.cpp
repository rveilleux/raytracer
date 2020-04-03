#include "PPM.h"
#include <string_view>
#include <cmath>
#include "ColorString.h"

void PPMBase::Add(std::string&& st) {
	_strings.emplace_back(std::move(st));
}

namespace {
	constexpr int kMaxLineLength = 70 - 4;
}

PPM::PPM(int width, int height) {
	Add("P3");
	Add(std::to_string(width) + " " + std::to_string(height));
	Add("255");
}

void PPM::AddPixel(const Color& c) {
	AppendAndFlush(ColorToByteString(c.x));
	AppendAndFlush(ColorToByteString(c.y));
	AppendAndFlush(ColorToByteString(c.z));
}

void PPM::AppendAndFlush(const std::string_view& s) {
	if (!_currentPixelLine.empty()) {
		_currentPixelLine += ' ';
	}
	_currentPixelLine.append(s);
	if (_currentPixelLine.length() >= kMaxLineLength) {
		EndLine();
	}
}

void PPM::EndLine() {
	Add(std::move(_currentPixelLine));
	_currentPixelLine.clear();
}

PPMBinary::PPMBinary(int width, int height):
	_pixelData(width * height * 3)
{
	Add("P6");
	Add(std::to_string(width) + " " + std::to_string(height));
	Add("255");
	//_pixelData.reserve(width * height * 3);
}

void PPMBinary::AddPixel(const Color& c) noexcept {
	//_pixelData.push_back(Color::DoubleToByte(c.x));
	//_pixelData.push_back(Color::DoubleToByte(c.y));
	//_pixelData.push_back(Color::DoubleToByte(c.z));
	_pixelData[_addPixelIndex++] = Color::DoubleToByte(c.x);
	_pixelData[_addPixelIndex++] = Color::DoubleToByte(c.y);
	_pixelData[_addPixelIndex++] = Color::DoubleToByte(c.z);
}
