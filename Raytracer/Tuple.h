#pragma once
#include "Math.h"

struct Tuple {
	double x, y, z, w;
	Tuple(double x, double y, double z, double w)
		: x(x)
		, y(y)
		, z(z)
		, w(w)
	{}
	bool IsPoint() const { return w == 1.0; }
	bool IsVector() const { return w == 0.0; }

	double Magnitude() const { return std::sqrt(x*x + y * y + z * z + w * w); }
	Tuple Normalize() const { double norm = Magnitude(); return *this / norm; }
	double Dot(const Tuple& rhs) const { return x * rhs.x + y * rhs.y + z * rhs.z + w * rhs.w; }

	friend bool ApproximatelyEqual(const Tuple& lhs, const Tuple& rhs) {
		return almostEqual(lhs.x, rhs.x, 0.00001) &&
			almostEqual(lhs.y, rhs.y, 0.00001) &&
			almostEqual(lhs.z, rhs.z, 0.00001) &&
			almostEqual(lhs.w, rhs.w, 0.00001);
	}

	friend bool operator==(const Tuple& lhs, const Tuple& rhs) {
		return almostEqual(lhs.x, rhs.x) && almostEqual(lhs.y, rhs.y) && almostEqual(lhs.z, rhs.z) && almostEqual(lhs.w, rhs.w);
	}
	friend bool operator!=(const Tuple& lhs, const Tuple& rhs) {
		return !(lhs == rhs);
	}
	friend Tuple operator+(const Tuple& lhs, const Tuple& rhs) {
		return { lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w };
	}
	friend Tuple operator-(const Tuple& lhs, const Tuple& rhs) {
		return lhs + -rhs;
	}
	friend Tuple operator-(const Tuple& lhs) {
		return { -lhs.x, -lhs.y, -lhs.z, -lhs.w };
	}
	friend Tuple operator*(const Tuple& lhs, double scalar) {
		return { lhs.x * scalar, lhs.y * scalar, lhs.z * scalar, lhs.w * scalar };
	}
	friend Tuple operator*(double scalar, const Tuple& lhs) {
		return lhs * scalar;
	}
	friend Tuple operator/(const Tuple& lhs, double scalar) {
		return lhs * (1 / scalar);
	}
};

struct Point : Tuple {
	Point(double x, double y, double z) : Tuple{ x,y,z,1 } {};
	Point(const Tuple& tuple) : Tuple(tuple.x, tuple.y, tuple.z, 1) {}
};

struct Vector : Tuple {
	Vector(double x, double y, double z) : Tuple{ x,y,z,0 } {}
	Vector(const Tuple& tuple) : Tuple(tuple.x, tuple.y, tuple.z, 0) {}
	Vector Cross(const Vector& rhs) const { return Vector(y * rhs.z - z * rhs.y, z * rhs.x - x * rhs.z, x * rhs.y - y * rhs.x); }
	Vector Reflect(const Vector& normal) const {
		return *this - normal * 2 * Dot(normal);
	}
	friend Vector operator-(const Vector& lhs) {
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
