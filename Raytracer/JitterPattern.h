#pragma once
#include "Pattern.h"

class JitterPattern: public Pattern
{
public:
	JitterPattern(std::unique_ptr<Pattern>&& pattern) noexcept;
	Color PatternAt(const Point& p) const override;

private:
	std::unique_ptr<Pattern> _pattern;
};
