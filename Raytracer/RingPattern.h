#pragma once
#include "Pattern.h"

class RingPattern: public Pattern {
public:
	RingPattern(const Color& c1, const Color& c2) noexcept;
	Color PatternAt(const Point& p) const noexcept override;
	Color GetColorA() const noexcept { return a; }
	Color GetColorB() const noexcept { return b; }

private:
	const Color a;
	const Color b;
};
