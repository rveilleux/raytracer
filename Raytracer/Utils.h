#pragma once
#include <vector>
#include <string>
#define _USE_MATH_DEFINES
#include <math.h>
#include <limits>
#include <fstream>
#include "Dynarray.h"

typedef std::vector<std::string> TStrings;
typedef Dynarray<uint8_t> TBytes;

void SaveToFile(const std::string& filename, const TStrings& strings);

class File {
public:
	File(const std::string& filename, bool binary);
	~File();
	void SaveStrings(const TStrings& strings);
	void SaveBytes(const TBytes& bytes);
private:
	std::ofstream _stream;
};

inline bool IsOdd(int number) { return (number & 1) == 1; }

//RV: std::numeric_limits<double>::epsilon() is too small for some test to succeed
//because epsilon() only works for values at 1.0.
//const double myEpsilon = std::numeric_limits<double>::epsilon() * 100;
const double myEpsilon = 0.000001;
inline bool almostEqual(double a, double b, double delta = myEpsilon) {
	return std::abs(a - b) <= delta;
}
