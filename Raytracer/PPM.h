#pragma once
#include "Strings.h"
#include "Dynarray.h"
#include "Color.h"

class PPMBase
{
public:
	TStrings GetLines() const { return _strings; }
protected:
	TStrings _strings;
	void Add(std::string&& st);
};

// Text-format implementation of PPM:
class PPM : public PPMBase
{
public:
	PPM(int width, int height);
	void AddPixel(const Color& c);
	void EndLine();
private:
	std::string _currentPixelLine;

	void AppendAndFlush(const std::string_view& s);
};

// Binary-format version:
class PPMBinary : public PPMBase
{
public:
	PPMBinary(int width, int height);
	void AddPixel(const Color& c) noexcept;
	const TBytes& GetBytes() const noexcept { return _pixelData; }
private:
	TBytes _pixelData;
	int _addPixelIndex = 0;
};
