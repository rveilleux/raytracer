#pragma once
#include "Tuple.h"

class Color : public Tuple
{
public:
	static uint8_t DoubleToByte(double a) {
		return static_cast<uint8_t>(std::fmax(0, std::fmin(255, (std::round(a * 255)))));
	}
	__forceinline Color() : Tuple(0, 0, 0, 0) {}
	__forceinline Color(double r, double g, double b) : Tuple(r, g, b, 0) {}
	inline const double& r() const { return x; }
	inline double& r() { return x; }
	inline const double& g() const { return y; }
	inline double& g() { return y; }
	inline const double& b() const { return z; }
	inline double& b() { return z; }
	inline const double& a() const { return w; }
	inline double& a() { return w; }
	friend Color operator*(const Color& lhs, const Color& rhs) {
		return Color(
			lhs.r() * rhs.r(),
			lhs.g() * rhs.g(),
			lhs.b() * rhs.b()
		);
	}
	friend Color operator*(const Color& lhs, double scalar) {
		return { lhs.x * scalar, lhs.y * scalar, lhs.z * scalar };
	}
	friend Color operator+(const Color& lhs, const Color& rhs) {
		return { lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z };
	}
	friend Color operator-(const Color& lhs, const Color& rhs) {
		return { lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z };
	}
	std::string RedByteString() const { return std::to_string(Color::DoubleToByte(x)); }
	std::string GreenByteString() const { return std::to_string(Color::DoubleToByte(y)); }
	std::string BlueByteString() const { return std::to_string(Color::DoubleToByte(z)); }
	static const Color White;
	static const Color Black;
};
