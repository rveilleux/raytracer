#pragma once

#include "Canvas.h"
#include "BaseTransform.h"
#include "World.h"

class Camera: public BaseTransform {
public:
	Camera(int hsize, int vsize, double fov);
	int GetHSize() const { return _hsize; }
	int GetVSize() const { return _vsize; }
	double GetFOV() const { return _fieldOfView; }
	double GetPixelSize() const { return _pixelSize; }
	Ray RayForPixel(int x, int y) const;
	Canvas Render(const World& w) const;

protected:
	void OnTransformedChanged() override;

private:
	const int _hsize;
	const int _vsize;
	const double _fieldOfView;
	const struct THalfSize {
		double width;
		double height;
	} _halfSize;
	const double _pixelSize;
	Point _localOrigin = Point(0, 0, 0);

	double ComputePixelSize() const;
	Camera::THalfSize ComputeHalfSize() const;
	void RenderLine(const World& w, Canvas& image, int y) const;
	void RenderLines(const World& w, Canvas& image, int begin, int end) const;
};