#include "Camera.h"
#include "Thread.h"
#include <iostream>
#include <thread>

Camera::Camera(int hsize, int vsize, double fov)
	: _hsize(hsize)
	, _vsize(vsize)
	, _fieldOfView(fov)
	, _halfSize(ComputeHalfSize())
	, _pixelSize(ComputePixelSize())
{
}

void Camera::OnTransformedChanged() {
	BaseTransform::OnTransformedChanged();
	_localOrigin = GetInverseTransform() * Point(0, 0, 0);
}

Camera::THalfSize Camera::ComputeHalfSize() const {
	double halfView = std::tan(_fieldOfView / 2);
	double aspect = static_cast<double>(_hsize) / _vsize;
	double halfWidth = halfView;
	double halfHeight = halfView;

	if (aspect >= 1) {
		halfHeight /= aspect;
	}
	else {
		halfWidth *= aspect;
	}
	return { halfWidth, halfHeight };
}

double Camera::ComputePixelSize() const {
	return _halfSize.width * 2 / _hsize;
}

Ray Camera::RayForPixel(int x, int y) const {
	double xoffset = (x + 0.5) * _pixelSize;
	double yoffset = (y + 0.5) * _pixelSize;
	double worldX = _halfSize.width - xoffset;
	double worldY = _halfSize.height - yoffset;
	const auto& inv = GetInverseTransform();
	Point pixel = inv * Point(worldX, worldY, -1);
	Vector direction = (pixel - _localOrigin).Normalize();
	return Ray(_localOrigin, direction);
}

Canvas Camera::Render(const World& w) const {
	Canvas image(_hsize, _vsize);

	{
		ThreadList threads;
		int nbLinesPerThread = std::max((_vsize + threads.GetCount() - 1) / threads.GetCount(), 1);

		int begin = 0;
		int end = nbLinesPerThread;
		for (int i = 0; i < threads.GetCount() && begin < _vsize; i++) {
			if (i == threads.GetCount() - 1) {
				end = _vsize;
			}
			//std::cout << "thread " << i << " begin=" << begin << " end=" << end << std::endl;
			threads.Add([=, &w, &image]() { RenderLines(w, image, begin, end); });
			begin += nbLinesPerThread;
			end += nbLinesPerThread;
		}
	}

	//for (int y = 0; y < _vsize; y++) {
	//	RenderLine(w, image, y);
	//}

	//std::cout << "Camera::Render: finished." << std::endl;
	return image;
}

void Camera::RenderLine(const World& w, Canvas& image, int y) const {
	for (int x = 0; x < _hsize; x++) {
		Ray ray = RayForPixel(x, y);
		Color color = w.ColorAt(ray);
		image.WritePixel(x, y, color);
	}
}

void Camera::RenderLines(const World& w, Canvas& image, int begin, int end) const {
	//std::cout << "Camera::RenderLines: " << begin << " to " << end << " start " << std::endl;
	for (int y = begin; y < end; y++) {
		RenderLine(w, image, y);
	}
	//std::cout << "Camera::RenderLines: " << begin << " to " << end << " end " << std::endl;
}
