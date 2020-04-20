#include "Camera.h"
#include "Thread.h"
#include "World.h"
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

void Camera::OnTransformedChanged() noexcept {
	BaseTransform::OnTransformedChanged();
	_localOrigin = GetInverseTransform() * Point(0, 0, 0);
}

Camera::THalfSize Camera::ComputeHalfSize() const noexcept {
	const double halfView = std::tan(_fieldOfView / 2);
	const double aspect = static_cast<double>(_hsize) / _vsize;
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

double Camera::ComputePixelSize() const noexcept {
	return _halfSize.width * 2 / _hsize;
}

Ray Camera::RayForPixel(int x, int y) const noexcept {
	const double xoffset = (x + 0.5) * _pixelSize;
	const double yoffset = (y + 0.5) * _pixelSize;
	const double worldX = _halfSize.width - xoffset;
	const double worldY = _halfSize.height - yoffset;
	const auto& inv = GetInverseTransform();
	const Point pixel = inv * Point(worldX, worldY, -1);
	const Vector direction = (pixel - _localOrigin).Normalize();
	return Ray(_localOrigin, direction);
}

Canvas Camera::Render(const World& w, int maxRecursion) const {
	Canvas image(_hsize, _vsize);
	Render(w, maxRecursion, image);
	return image;
}

constexpr bool MULTITHREAD_RENDERING = true;

void Camera::Render(const World& w, int maxRecursion, Canvas& image) const {
	if (MULTITHREAD_RENDERING) {
		ThreadList threads;
		const int nbLinesPerThread = std::max((_vsize + threads.GetCount() - 1) / threads.GetCount(), 1);

		int begin = 0;
		int end = nbLinesPerThread;
		for (int i = 0; i < threads.GetCount() && begin < _vsize; i++) {
			if (i == threads.GetCount() - 1) {
				end = _vsize;
			}
			//std::cout << "thread " << i << " begin=" << begin << " end=" << end << std::endl;
			threads.Add([=, &w, &image]() { RenderLines(w, image, begin, end, maxRecursion); });
			begin += nbLinesPerThread;
			end += nbLinesPerThread;
		}
	}
	else {
		for (int y = 0; y < _vsize; y++) {
			RenderLine(w, image, y, maxRecursion);
		}
	}
	//std::cout << "Camera::Render: finished." << std::endl;
}

void Camera::RenderLine(const World& w, Canvas& image, int y, int maxRecursion) const {
	for (int x = 0; x < _hsize; x++) {
		const Ray ray = RayForPixel(x, y);
		const Color color = w.ColorAt(ray, maxRecursion);
		image.WritePixel(x, y, color);
	}
}

void Camera::RenderLines(const World& w, Canvas& image, int begin, int end, int maxRecursion) const {
	//std::cout << "Camera::RenderLines: " << begin << " to " << end << " start " << std::endl;
	for (int y = begin; y < end; y++) {
		RenderLine(w, image, y, maxRecursion);
	}
	//std::cout << "Camera::RenderLines: " << begin << " to " << end << " end " << std::endl;
}
