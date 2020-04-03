#include "Intersections.h"
#include <algorithm>
#pragma warning (disable: 26446)

Intersections::Intersections(const TIntersections& vi) {
	Add(vi);
}
//
//const Intersection& Intersections::operator[](int index) const {
//	auto iter = _intersections.cbegin();
//	while (index) {
//		iter++;
//		index--;
//	}
//	return *iter;
//}

Intersections::TIntersections Intersections::GetSorted() const {
	TIntersections result = _intersections;
	std::sort(result.begin(), result.end());
	return result;
}

std::optional<Intersection> Intersections::Hit() const noexcept {
	//const Intersection positive{ 0.0, nullptr };
	//auto findSmallestNonNegative = _intersections.lower_bound(positive);
	//if (findSmallestNonNegative == _intersections.end()) {
	//	return std::nullopt;
	//}
	int findSmallestNonNegative = -1;

	for (unsigned int i = 0; i < _intersections.size(); i++) {
		if (_intersections[i].t < 0.0) continue;
		if (findSmallestNonNegative != -1 && _intersections[i].t >= _intersections[findSmallestNonNegative].t) continue;
		findSmallestNonNegative = i;
	}
	if (findSmallestNonNegative == -1) return std::nullopt;

	return _intersections[findSmallestNonNegative];
}

void Intersections::Add(const Intersection& i) {
	_intersections.push_back(i);
	//static int highest = 0;
	//if (_intersections.size() > highest) {
	//	highest = _intersections.size();
	//	std::cout << "New highest: " << highest << std::endl;
	//}
}

void Intersections::Add(const TIntersections& vi) {
	//_intersections.insert(vi.cbegin(), vi.cend());
	for (const auto& i : vi) {
		Add(i);
	}
}
