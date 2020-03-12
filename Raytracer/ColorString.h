#pragma once

#include <string>
#include "Color.h"

std::string ColorToByteString(double v) { return std::to_string(Color::DoubleToByte(v)); }
