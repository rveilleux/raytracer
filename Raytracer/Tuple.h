#pragma once
#include "Math.h"

struct Tuple {
	double x, y, z, w;
	constexpr Tuple(double x, double y, double z, double w) noexcept
		: x(x)
		, y(y)
		, z(z)
		, w(w)
	{}
	constexpr bool IsPoint() const noexcept { return w == 1.0; }
	constexpr bool IsVector() const noexcept { return w == 0.0; }

	double Magnitude() const noexcept { return std::sqrt(x*x + y * y + z * z + w * w); }
	Tuple Normalize() const noexcept { const double norm = Magnitude(); return *this / norm; }
	constexpr double Dot(const Tuple& rhs) const noexcept { return x * rhs.x + y * rhs.y + z * rhs.z + w * rhs.w; }

	friend bool ApproximatelyEqual(const Tuple& lhs, const Tuple& rhs) noexcept {
		return almostEqual(lhs.x, rhs.x, 0.00001) &&
			almostEqual(lhs.y, rhs.y, 0.00001) &&
			almostEqual(lhs.z, rhs.z, 0.00001) &&
			almostEqual(lhs.w, rhs.w, 0.00001);
	}

	friend bool operator==(const Tuple& lhs, const Tuple& rhs) noexcept {
		return almostEqual(lhs.x, rhs.x) && almostEqual(lhs.y, rhs.y) && almostEqual(lhs.z, rhs.z) && almostEqual(lhs.w, rhs.w);
	}
	friend bool operator!=(const Tuple& lhs, const Tuple& rhs) noexcept {
		return !(lhs == rhs);
	}
	friend constexpr Tuple operator+(const Tuple& lhs, const Tuple& rhs) noexcept {
		return { lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w };
	}
	friend constexpr Tuple operator-(const Tuple& lhs, const Tuple& rhs) noexcept {
		return lhs + -rhs;
	}
	friend constexpr Tuple operator-(const Tuple& lhs) noexcept {
		return { -lhs.x, -lhs.y, -lhs.z, -lhs.w };
	}
	friend constexpr Tuple operator*(const Tuple& lhs, double scalar) noexcept {
		return { lhs.x * scalar, lhs.y * scalar, lhs.z * scalar, lhs.w * scalar };
	}
	friend constexpr Tuple operator*(double scalar, const Tuple& lhs) noexcept {
		return lhs * scalar;
	}
	friend constexpr Tuple operator/(const Tuple& lhs, double scalar) {
		return lhs * (1 / scalar);
	}
};

struct Point : Tuple {
	constexpr Point(double x, double y, double z) noexcept : Tuple{ x,y,z,1 } {};
	constexpr Point(const Tuple& tuple) noexcept : Tuple(tuple.x, tuple.y, tuple.z, 1) {}
};

struct Vector : Tuple {
	constexpr Vector(double x, double y, double z) noexcept : Tuple{ x,y,z,0 } {}
	constexpr Vector(const Tuple& tuple) noexcept : Tuple(tuple.x, tuple.y, tuple.z, 0) {}
	constexpr Vector Cross(const Vector& rhs) const noexcept { return Vector(y * rhs.z - z * rhs.y, z * rhs.x - x * rhs.z, x * rhs.y - y * rhs.x); }
	constexpr Vector Reflect(const Vector& normal) const noexcept {
		return *this - normal * 2 * Dot(normal);
	}
	friend constexpr Vector operator-(const Vector& lhs) noexcept {
		return { -lhs.x, -lhs.y, -lhs.z };
	}
};

//
//inline tuple point(double x, double y, double z) {
//	return tuple{ x, y, z, 1 };
//}
//
//inline tuple vector(double x, double y, double z) {
//	return tuple{ x, y, z, 0 };
//}
