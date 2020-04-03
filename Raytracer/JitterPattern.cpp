#include "JitterPattern.h"
#include <cmath>
#include "PerlinNoise.h"

JitterPattern::JitterPattern(std::unique_ptr<Pattern>&& pattern) noexcept
	: _pattern(std::move(pattern))
{
}

Color JitterPattern::PatternAt(const Point& p) const {
	//double perlin = PerlinNoise::GetValue(p);
	//Point p2 = Point(p.x + perlin, p.y + perlin, p.z + perlin);
	const Point p2 = p + Point(PerlinNoise::GetValue({ p.x, 0, 0 }),
		PerlinNoise::GetValue({ 0, p.y, 0 }),
		PerlinNoise::GetValue({ 0,0,p.z }));
	return _pattern->PatternAt(p2);
}
