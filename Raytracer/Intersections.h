#pragma once

#include <vector>
#include <optional>
#include <set>
#include "Intersection.h"

class Intersections {
public:
	// No need to use 'multiset' if Intersection struct contains a auto-increment 'id' field to differentiate two hits exactly at the same position
	//typedef std::multiset<Intersection> TIntersections;
	//typedef std::set<Intersection> TIntersections;
	typedef std::vector<Intersection> TIntersections;
	Intersections() = default;
	explicit Intersections(const TIntersections& vi);
	int GetCount() const { return _intersections.size(); }
	bool IsEmpty() const { return _intersections.empty(); }
	void Add(const Intersection& i);
	//const Intersection& operator[](int index) const { return _intersections[index]; }
	TIntersections GetSorted() const;
	std::optional<Intersection> Hit() const;
private:
	TIntersections _intersections;

	void Add(const TIntersections& vi);
};
