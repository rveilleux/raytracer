#pragma once
#include "Pattern.h"

class TestPattern: public Pattern {
public:
	TestPattern() = default;

	Color PatternAt(const Point& p) const noexcept override;
};
