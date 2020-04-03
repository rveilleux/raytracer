#pragma once
#include "Pattern.h"

class StripePattern: public Pattern {
public:
	StripePattern(const Color& c1, const Color& c2);
	Color PatternAt(const Point& p) const noexcept override;
	Color GetColorA() const noexcept { return a; }
	Color GetColorB() const noexcept { return b; }

private:
	const Color a;
	const Color b;
};
