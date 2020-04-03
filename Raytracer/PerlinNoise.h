#pragma once

struct Point;

class PerlinNoise {
public:
	//PerlinNoise();
	static double GetValue(const Point& point) noexcept;
};
