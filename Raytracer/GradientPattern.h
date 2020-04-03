#pragma once

#include "Pattern.h"

class GradientPattern: public Pattern {
public:
	GradientPattern(const Color& c1, const Color& c2) noexcept;

	Color PatternAt(const Point& p) const noexcept override;

private:
	Color a;
	Color b;
};
