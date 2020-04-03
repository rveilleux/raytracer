#pragma once
class Shape;

class Intersection {
public:
	Intersection(double t, const Shape* o) noexcept
		: t(t)
		, object(o)
	{}
	friend bool operator==(const Intersection& lhs, const Intersection& rhs) noexcept {
		return lhs.object == rhs.object && lhs.t == rhs.t;
	}
	friend bool operator<(const Intersection& lhs, const Intersection& rhs) noexcept {
		return lhs.t < rhs.t;
	}
	// These members can't be const because std::sort needs to modifying
	/*const */double t;
	const Shape* /*const */object;
};
