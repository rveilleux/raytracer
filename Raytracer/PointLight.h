#pragma once
#include "Tuple.h"
#include "Color.h"

class PointLight {
public:
	PointLight(const Point& position, const Color& intensity) noexcept
		: _intensity(intensity)
		, _position(position)
	{}
	Color GetIntensity() const noexcept { return _intensity; }
	Point GetPosition() const noexcept { return _position; }
	void SetPosition(const Point& p) noexcept { _position = p; }
	void SetIntensity(const Color& color) noexcept { _intensity = color; }
	friend bool operator==(const PointLight& lhs, const PointLight& rhs) noexcept;
private:
	Color _intensity;
	Point _position;
};
