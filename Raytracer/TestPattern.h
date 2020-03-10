#pragma once
#include "Pattern.h"

class TestPattern: public Pattern {
public:
	TestPattern() {};

	Color PatternAt(const Point& p) const override;
};
