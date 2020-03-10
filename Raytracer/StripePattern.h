#pragma once
#include "Pattern.h"

class StripePattern: public Pattern {
public:
	StripePattern(const Color& c1, const Color& c2);
	Color PatternAt(const Point& p) const override;
	Color GetColorA() const { return a; }
	Color GetColorB() const { return b; }

private:
	const Color a;
	const Color b;
};
