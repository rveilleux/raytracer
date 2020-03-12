#include "Canvas.h"
#include "PPM.h"
#include "File.h"

Canvas::Canvas(int width, int height)
	: _width(width)
	, _height(height)
	, _pix(_width * _height)
{
}

bool Canvas::AllPixelColorAre(const Color& c) const {
	//for (const auto& pix: _pix) {
	for (int i = 0; i < _width * _height; i++) {
		const auto& pix = _pix[i];
		if (pix != c) return false;
	}
	return true;
}

void Canvas::SetAllPixels(const Color& c) {
	//for (auto& pix : _pix) {
	for (int i = 0; i < _width * _height; i++) {
		auto& pix = _pix[i];
		pix = c;
	}
}

void Canvas::WritePixel(int x, int y, const Color& c) {
	if (x < 0 || x >= _width || y < 0 || y >= _height) return;
	WritePixelFast(x, y, c);
}

Color Canvas::PixelAt(int x, int y) const {
	if (x < 0 || x >= _width || y < 0 || y >= _height) return Color();
	return _pix[x + y * _width];
}

PPM Canvas::ToPPM() const {
	PPM ppm(_width, _height);
	for (int y = 0; y < _height; y++) {
		for (int x = 0; x < _width; x++) {
			ppm.AddPixel(_pix[x + y * _width]);
		}
		ppm.EndLine();
	}
	ppm.EndLine();
	return ppm;
}

PPMBinary Canvas::ToPPMBinary() const {
	PPMBinary ppm(_width, _height);
	for (int y = 0; y < _height; y++) {
		for (int x = 0; x < _width; x++) {
			ppm.AddPixel(_pix[x + y * _width]);
		}
	}
	return ppm;
}

void Canvas::SaveToFile(const std::string& filename) const {
	PPMBinary ppm = ToPPMBinary();
	File file(filename, true);
	file.SaveStrings(ppm.GetLines());
	file.SaveBytes(ppm.GetBytes());
}
