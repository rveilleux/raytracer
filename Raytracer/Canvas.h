#pragma once
#include "Color.h"
#include "Dynarray.h"

class PPM;
class PPMBinary;

class Canvas
{
public:
	Canvas(int width, int height);
	int GetWidth() const { return _width; }
	int GetHeight() const { return _height; }
	bool AllPixelColorAre(const Color& c) const;
	void SetAllPixels(const Color& c);
	void WritePixel(int x, int y, const Color& c);
	void WritePixelFast(int x, int y, const Color& c) { _pix[x + y * _width] = c; }
	void WritePixel(double x, double y, const Color& c) { WritePixel(static_cast<int>(x), static_cast<int>(y), c); }
	Color PixelAt(int x, int y) const;
	PPM ToPPM() const;
	PPMBinary ToPPMBinary() const;
	void SaveToFile(const std::string& fileName) const;
private:
	const int _width;
	const int _height;
	//std::vector<Color> _pix;
	Dynarray<Color> _pix;
};
