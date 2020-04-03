#pragma once
#include <cstdint>
#include "Tuple.h"

class Color : public Tuple
{
public:
	static uint8_t DoubleToByte(double a) noexcept {
		return static_cast<uint8_t>(std::fmax(0, std::fmin(255, (std::round(a * 255)))));
	}
	constexpr __forceinline Color() noexcept : Tuple(0, 0, 0, 0) {}
	constexpr __forceinline Color(double r, double g, double b) noexcept : Tuple(r, g, b, 0) {}
	inline const double& r() const noexcept { return x; }
	inline double& r() noexcept { return x; }
	inline const double& g() const noexcept { return y; }
	inline double& g() noexcept { return y; }
	inline const double& b() const noexcept { return z; }
	inline double& b() noexcept { return z; }
	inline const double& a() const noexcept { return w; }
	inline double& a() noexcept { return w; }
	friend Color operator*(const Color& lhs, const Color& rhs) noexcept {
		return Color(
			lhs.r() * rhs.r(),
			lhs.g() * rhs.g(),
			lhs.b() * rhs.b()
		);
	}
	friend Color operator*(const Color& lhs, double scalar) noexcept {
		return { lhs.x * scalar, lhs.y * scalar, lhs.z * scalar };
	}
	friend Color operator+(const Color& lhs, const Color& rhs) noexcept {
		return { lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z };
	}
	friend Color operator-(const Color& lhs, const Color& rhs) noexcept {
		return { lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z };
	}
	static const Color White;
	static const Color Black;
};
