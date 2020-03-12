#pragma once

#include <string>
#include "Tuple.h"

std::string TupleToString(const Tuple& tuple) { 
	return "(" + std::to_string(tuple.x) + "," + std::to_string(tuple.y) + "," + std::to_string(tuple.z) + "," + std::to_string(tuple.w) + ")";
}
