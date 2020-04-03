#pragma once
#include <array>
#include "Math.h"
#include "Tuple.h"

#pragma warning(push)
#pragma warning(disable: 26482)
#pragma warning(disable: 26446)
template <int NR, int NC>
class Matrix {
public:
	typedef std::array<double, NC> TRow;
	typedef std::array<TRow, NR> TMatrix;

	//Matrix(double a1, double a2, double a3, double a4,
	//	double b1, double b2, double b3, double b4,
	//	double c1, double c2, double c3, double c4,
	//	double d1, double d2, double d3, double d4)
	//{
	//	_values[0][0] = a1;
	//	_values[0][1] = a2;
	//	_values[0][2] = a3;
	//	_values[0][3] = a4;
	//	_values[1][0] = b1;
	//	_values[1][1] = b2;
	//	_values[1][2] = b3;
	//	_values[1][3] = b4;
	//	_values[2][0] = c1;
	//	_values[2][1] = c2;
	//	_values[2][2] = c3;
	//	_values[2][3] = c4;
	//	_values[3][0] = d1;
	//	_values[3][1] = d2;
	//	_values[3][2] = d3;
	//	_values[3][3] = d4;
	//}
	TRow& operator[](int index) noexcept { return _values[index]; }
	constexpr const TRow& operator[](int index) const noexcept { return _values[index]; }
	template<int R, int C>
	double Get() const noexcept { return std::get<C>(std::get<R>(_values)); }
	template<int R, int C>
	void Set(double value) noexcept { std::get<C>(std::get<R>(_values)) = value; }
	template<int R>
	void SetRow(const TRow& row) noexcept { std::get<R>(_values) = row; }
	//void SetAll(const TMatrix& matrix) { _values = matrix; }
	void SetAll(const TRow& r0, const TRow& r1) noexcept {
		std::get<0>(_values) = r0;
		std::get<1>(_values) = r1;
	}
	void SetAll(const TRow& r0, const TRow& r1, const TRow& r2) noexcept {
		SetAll(r0, r1);
		std::get<2>(_values) = r2;
	}
	void SetAll(const TRow& r0, const TRow& r1, const TRow& r2, const TRow& r3) noexcept {
		SetAll(r0, r1, r2);
		std::get<3>(_values) = r3;
	}
	Matrix<NR, NC> Transpose() const noexcept {
		const Matrix<NR, NC> result {
			TRow{_values[0][0],_values[1][0],_values[2][0],_values[3][0]},
			TRow{_values[0][1],_values[1][1],_values[2][1],_values[3][1]},
			TRow{_values[0][2],_values[1][2],_values[2][2],_values[3][2]},
			TRow{_values[0][3],_values[1][3],_values[2][3],_values[3][3]}
		};
		//for (int r = 0; r < NR; r++) {
		//	for (int c = 0; c < NC; c++) {
		//		result[c][r] = _values[r][c];
		//	}
		//}
		return result;
	}
#pragma warning( push )
#pragma warning(disable:4701)
	Matrix<NR - 1, NC - 1> Submatrix(int removeR, int removeC) const noexcept {
		Matrix<NR - 1, NC - 1> result;
		int resultr = 0;
		for (int r = 0; r < NR; r++) {
			if (r == removeR) continue;
			int resultc = 0;
			for (int c = 0; c < NC; c++) {
				if (c == removeC) continue;
				result[resultr][resultc] = _values[r][c];
				resultc++;
			}
			resultr++;
		}
		return result;
	}
#pragma warning(pop)
	double Determinant() const noexcept {
		if constexpr(NR == 2) {
			return _values[0][0] * _values[1][1] - _values[0][1] * _values[1][0];
		}
		else {
			double determinant = 0;
			for (int c = 0; c < NC; c++) {
				determinant += _values[0][c] * Cofactor(0, c);
			}
			return determinant;
		}
	}
	double Minor(int removeR, int removeC) const noexcept {
		return Submatrix(removeR, removeC).Determinant();
	}
	double Cofactor(int removeR, int removeC) const noexcept {
		const double minor = Minor(removeR, removeC);
		return IsOdd(removeR + removeC) ? -minor : minor;
	}
	bool IsInvertible() const noexcept {
		return !(almostEqual(Determinant(), 0));
	}
	bool ApproximatelyEqual(const Matrix<NR, NC>& rhs) const noexcept {
		for (int r = 0; r < NR; r++) {
			for (int c = 0; c < NC; c++) {
				if (!almostEqual(_values[r][c], rhs._values[r][c], 0.00001)) {
					return false;
				}
			}
		}
		return true;
	}
	friend bool operator==(const Matrix<NR, NC>& lhs, const Matrix<NR, NC>& rhs) noexcept {
		return lhs.ApproximatelyEqual(rhs);
	}
	friend bool operator!=(const Matrix<NR, NC>& lhs, const Matrix<NR, NC>& rhs) noexcept {
		return !(lhs == rhs);
	}
	friend Matrix<NR, NC> operator*(const Matrix<NR, NC>& lhs, const Matrix<NR, NC>& rhs) noexcept {
		Matrix<NR, NC> result;
		for (int r = 0; r < NR; r++) {
			for (int c = 0; c < NC; c++) {
				result[r][c] =
					lhs[r][0] * rhs[0][c] +
					lhs[r][1] * rhs[1][c] +
					lhs[r][2] * rhs[2][c] +
					lhs[r][3] * rhs[3][c];
			}
		}
		return result;
	}
	Matrix<NR, NC> Inverse() const noexcept {
		_ASSERT_EXPR(IsInvertible(), L"Matrix is not invertible");
		double determinant = Determinant();

		Matrix<NR, NC> result;
		for (int r = 0; r < NR; r++) {
			for (int c = 0; c < NC; c++) {
				double cofactor = Cofactor(r, c);
				result[c][r] = cofactor / determinant;
			}
		}
		return result;
	}
	friend Tuple operator*(const Matrix<NR, NC>& lhs, const Tuple& rhs) noexcept {
		return Tuple{
		lhs[0][0] * rhs.x + lhs[0][1] * rhs.y + lhs[0][2] * rhs.z + lhs[0][3] * rhs.w,
		lhs[1][0] * rhs.x + lhs[1][1] * rhs.y + lhs[1][2] * rhs.z + lhs[1][3] * rhs.w,
		lhs[2][0] * rhs.x + lhs[2][1] * rhs.y + lhs[2][2] * rhs.z + lhs[2][3] * rhs.w,
		lhs[3][0] * rhs.x + lhs[3][1] * rhs.y + lhs[3][2] * rhs.z + lhs[3][3] * rhs.w
		};
	}
	Matrix<4, 4> Translation(const Vector& v) noexcept;
	Matrix<4, 4> Scaling(const Vector& v) noexcept;
	Matrix<4, 4> RotationX(double r) noexcept;
	Matrix<4, 4> RotationY(double r) noexcept;
	Matrix<4, 4> RotationZ(double r) noexcept;
	Matrix<4, 4> Shearing(double xy, double xz, double yx, double yz, double zx, double zy) noexcept;

	// Can't figure out how to initialize 'IdentityMatrix' if member is private
//private:
	TMatrix _values{};
	//double _values[4][4]{};
};
//
typedef std::array<double, 4> TRow;
typedef std::array<TRow, 4> TMatrix;
//static TRow row{ 1,2,3,4 };
static Matrix<4,4> IdentityMatrix{ TRow{1,0,0,0},TRow{0,1,0,0},TRow{0,0,1,0 }, TRow{ 0,0,0,1 } };

inline Matrix<4, 4> Matrix<4, 4>::Translation(const Vector& v) noexcept {
	Matrix<4, 4> m = IdentityMatrix;
	m.Set<0, 3>(v.x);
	m.Set<1, 3>(v.y);
	m.Set<2, 3>(v.z);
	return m * (*this);
}

inline Matrix<4, 4> Matrix<4, 4>::Scaling(const Vector& v) noexcept {
	Matrix<4, 4> m = IdentityMatrix;
	m.Set<0, 0>(v.x);
	m.Set<1, 1>(v.y);
	m.Set<2, 2>(v.z);
	return m * (*this);
}

inline Matrix<4, 4> Matrix<4, 4>::RotationX(double r) noexcept {
	Matrix<4, 4> m = IdentityMatrix;
	const double cos = std::cos(r);
	const double sin = std::sin(r);
	m.Set<1, 1>(cos);
	m.Set<1, 2>(-sin);
	m.Set<2, 1>(sin);
	m.Set<2, 2>(cos);
	return m * (*this);
}

inline Matrix<4, 4> Matrix<4, 4>::RotationY(double r) noexcept {
	Matrix<4, 4> m = IdentityMatrix;
	const double cos = std::cos(r);
	const double sin = std::sin(r);
	m.Set<0, 0>(cos);
	m.Set<0, 2>(sin);
	m.Set<2, 0>(-sin);
	m.Set<2, 2>(cos);
	return m * (*this);
}

inline Matrix<4, 4> Matrix<4, 4>::RotationZ(double r) noexcept {
	Matrix<4, 4> m = IdentityMatrix;
	const double cos = std::cos(r);
	const double sin = std::sin(r);
	m.Set<0, 0>(cos);
	m.Set<0, 1>(-sin);
	m.Set<1, 0>(sin);
	m.Set<1, 1>(cos);
	return m * (*this);
}

inline Matrix<4, 4> Matrix<4, 4>::Shearing(double xy, double xz, double yx, double yz, double zx, double zy) noexcept {
	Matrix<4, 4> m = IdentityMatrix;
	m.Set<0, 1>(xy);
	m.Set<0, 2>(xz);
	m.Set<1, 0>(yx);
	m.Set<1, 2>(yz);
	m.Set<2, 0>(zx);
	m.Set<2, 1>(zy);
	return m * (*this);
}

inline Matrix<4, 4> Translation(const Vector& v) noexcept {
	return IdentityMatrix.Translation(v);
}

inline Matrix<4, 4> Scaling(const Vector& v) noexcept {
	return IdentityMatrix.Scaling(v);
}

inline Matrix<4, 4> RotationX(double r) noexcept {
	return IdentityMatrix.RotationX(r);
}

inline Matrix<4, 4> RotationY(double r) noexcept {
	return IdentityMatrix.RotationY(r);
}

inline Matrix<4, 4> RotationZ(double r) noexcept {
	return IdentityMatrix.RotationZ(r);
}

inline Matrix<4, 4> Shearing(double xy, double xz, double yx, double yz, double zx, double zy) noexcept {
	return IdentityMatrix.Shearing(xy, xz, yx, yz, zx, zy);
}

Matrix<4, 4> ViewTransform(const Point& from, const Point& to, const Vector& up) noexcept;

//struct TS {
//private:
//	//double bla[4][4];
//	//TRow bl;// { 1, 2, 3, 4 };
//	TMatrix bla;// { TRow{ 1,2,3,4 }, TRow{ 2,3,4,5 }, TRow{ 1,2,3,2 }, TRow{ 1,2,1,2 } };
//};
//static double bla[4][4] { { 1,2,3,4 },{ 1,2,3,4 },{ 1,2,3,4 },{ 1,2,3,4 } };
//static Matrix<4, 4> bla2{ TMatrix{ { { 1,2,3,4 },{ 1,2,3,4 },{ 1,2,3,4 },{ 1,2,3,4 } } } };
//static TS bla2{ {TRow{2,4,3,4},TRow{3,4,1,2}, TRow{2,3,4,5}, TRow{6,7,8,9} } };
//static TS bla2{ TRow{2,4,3,4} };
//static TMatrix _identity{ TRow{1,0,0,0},TRow{0,1,0,0},TRow{0,0,1,0 }, TRow{ 0,0,0,1 } };
//static Matrix<4, 4> IdentityMatrix

#pragma warning(pop)
