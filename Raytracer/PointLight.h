#pragma once
#include "Tuple.h"
#include "Color.h"

class PointLight {
public:
	PointLight(const Point& position, const Color& intensity)
		: _intensity(intensity)
		, _position(position)
	{}
	Color GetIntensity() const { return _intensity; }
	Point GetPosition() const { return _position; }
	friend bool operator==(const PointLight& lhs, const PointLight& rhs);
private:
	Color _intensity;
	Point _position;
};
