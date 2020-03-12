#include "RunTests.h"
#include "UnitTest.h"
#include "Tuple.h"
#include "Color.h"
#include "Canvas.h"
#include "Matrix.h"
#include "Ray.h"
#include "Sphere.h"
#include "Intersection.h"
#include "PointLight.h"
#include "Material.h"
#include "Lighting.h"
#include "World.h"
#include "Camera.h"
#include "TestShape.h"
#include "Plane.h"
#include "StripePattern.h"
#include "TestPattern.h"
#include "GradientPattern.h"
#include "RingPattern.h"
#include "CheckerPattern.h"
#include "PPM.h"
#include "PerlinNoise.h"

void TestTuple() {
	UnitTest test("tuples");
	DOTEST(test, "A tuple with w=1.0 is a point",
		{ Tuple a { 4.3, -4.2, 3.1, 1.0}; return a.x == 4.3 && a.y == -4.2 && a.z == 3.1 && a.w == 1.0 && a.IsPoint() && !a.IsVector(); });
	DOTEST(test, "A tuple with w=0.0 is a vector",
		{ Tuple a { 4.3, -4.2, 3.1, 0.0}; return a.x == 4.3 && a.y == -4.2 && a.z == 3.1 && a.w == 0.0 && !a.IsPoint() && a.IsVector(); });
	DOTEST(test, "point() creates tuples with w=1",
		{ Point p(4, -4, 3); return p == Tuple{4,-4, 3, 1}; });
	DOTEST(test, "vector() creates tuples with w=0",
		{ Vector v(4, -4, 3); return v == Tuple{4,-4, 3, 0}; });
	DOTEST(test, "compare two different points",
		{ Point p1(1, 2, 3); auto p2 = Point(3,4,5);  return p1 != p2; });
	DOTEST(test, "Adding two tuples",
		{ Tuple a1 { 3, -2, 5, 1}; Tuple a2 {-2,3,1,0}; return a1 + a2 == Tuple{1,1,6,1}; });
	DOTEST(test, "Subtracting two points",
		{ Point p1(3,2,1); Point p2(5,6,7); return p1 - p2 == Vector(-2,-4,-6); });
	DOTEST(test, "Subtracting a vector from a point",
		{ Point p(3,2,1); Vector v(5,6,7); return p - v == Point(-2,-4,-6); });
	DOTEST(test, "Subtracting two vectors",
		{ Vector v1(3,2,1); Vector v2(5,6,7); return v1 - v2 == Vector(-2,-4,-6); });
	DOTEST(test, "Subtracting a vector from the zero-vector",
		{ Vector zero(0,0,0); Vector v(1,-2,3); return zero - v == Vector(-1,2,-3); });
	DOTEST(test, "Negating a tuple",
		{ Tuple a(1,-2,3,-4); return -a == Tuple(-1,2,-3,4); });
	DOTEST(test, "Multiplying a tuple by a scalar",
		{ Tuple a(1,-2,3,-4); return a * 3.5 == Tuple(3.5, -7, 10.5, -14); });
	DOTEST(test, "Multiplying a tuple by a fraction",
		{ Tuple a(1,-2,3,-4); return 0.5 * a == Tuple(0.5, -1, 1.5, -2); });
	DOTEST(test, "Dividing a tuple by a scalar",
		{ Tuple a(1,-2,3,-4); return a / 2 == Tuple(0.5, -1, 1.5, -2); });
	DOTEST(test, "Computing the magnitude of vector(1,0,0)",
		{ Vector v(1,0,0); return v.Magnitude() == 1; });
	DOTEST(test, "Computing the magnitude of vector(0,1,0)",
		{ Vector v(0,1,0); return v.Magnitude() == 1; });
	DOTEST(test, "Computing the magnitude of vector(0,0,1)",
		{ Vector v(0,0,1); return v.Magnitude() == 1; });
	DOTEST(test, "Computing the magnitude of vector(1,2,3)",
		{ Vector v(1,2,3); return v.Magnitude() == std::sqrt(14); });
	DOTEST(test, "Computing the magnitude of vector(-1,-2,-3)",
		{ Vector v(-1,-2,-3); return v.Magnitude() == std::sqrt(14); });
	DOTEST(test, "Normalizing vector(4,0,0) gives (1,0,0)",
		{ Vector v(4,0,0); return v.Normalize() == Vector(1,0,0); });
	DOTEST(test, "Normalizing vector(1,2,3) give expected result",
		{ Vector v(1,2,3); return ApproximatelyEqual(v.Normalize(), Vector(0.26726, 0.53452, 0.80178)); });
	DOTEST(test, "Normalizing vector(1,2,3) gives a magnitude of 1",
		{ Vector v(1,2,3); return v.Normalize().Magnitude() == 1; });
	DOTEST(test, "The dot product of two vectors",
		{ Vector a(1,2,3); Vector b(2,3,4); return a.Dot(b) == 20; });
	DOTEST(test, "The cross product of two vectors",
		{ Vector a(1,2,3); Vector b(2,3,4); return a.Cross(b) == Vector(-1,2,-1) && b.Cross(a) == Vector(1, -2, 1); });
	DOTEST(test, "Reflecting a vector approaching at 45deg",
		{ Vector v(1,-1,0); Vector n(0,1,0); Vector r = v.Reflect(n); return r == Vector(1, 1, 0); });
	DOTEST(test, "Reflecting a vector off a slanted surface",
		{ Vector v(0,-1,0); double sq2 = std::sqrt(2) / 2; Vector n(sq2,sq2,0); Vector r = v.Reflect(n); return r == Vector(1, 0, 0); });
}

void TestColor() {
	UnitTest test("colors");
	DOTEST(test, "Colors are (r, g, b) tuples",
		{ Color c(-0.5, 0.4, 1.7); return c.r() == -0.5 && c.g() == 0.4 && c.b() == 1.7; });
	DOTEST(test, "Adding colors",
		{ Color c1(0.9, 0.6, 0.75); Color c2(0.7, 0.1, 0.25); return c1 + c2 == Color(1.6, 0.7, 1.0); });
	DOTEST(test, "Subtracting colors",
		{ Color c1(0.9, 0.6, 0.75); Color c2(0.7, 0.1, 0.25); return c1 - c2 == Color(0.2, 0.5, 0.5); });
	DOTEST(test, "Multiplying a color by a scalar",
		{ Color c(0.2, 0.3, 0.4); return c * 2 == Color(0.4, 0.6, 0.8); });
	DOTEST(test, "Multiplying colors",
		{ Color c1(1, 0.2, 0.4); Color c2(0.9, 1, 0.1); return c1 * c2 == Color(0.9, 0.2, 0.04); });
}

void TestCanvas() {
	UnitTest test("canvas");
	DOTEST(test, "Creating a canvas",
		{ Canvas c(10, 20); return c.GetWidth() == 10 && c.GetHeight() == 20 && c.AllPixelColorAre(Color(0,0,0)); });
	DOTEST(test, "Writing pixels to a a canvas",
		{ Canvas c(10, 20); Color red(1,0,0); c.WritePixel(2, 3, red); return c.PixelAt(2,3) == red; });
	DOTEST(test, "Constructing the PPM header",
		{ Canvas c(5, 3); PPM ppm = c.ToPPM(); const auto& lines = ppm.GetLines(); return lines[0] == "P3" && lines[1] == "5 3" && lines[2] == "255"; });

	DOTEST(test, "Constructing the PPM pixel data", {
	Canvas c(5, 3);
	Color c1(1.5, 0, 0);
	Color c2(0, 0.5, 0);
	Color c3(-0.5, 0, 1);
	c.WritePixel(0, 0, c1);
	c.WritePixel(2, 1, c2);
	c.WritePixel(4, 2, c3);
	PPM ppm = c.ToPPM();
	const auto& lines = ppm.GetLines();
	return lines[3] == "255 0 0 0 0 0 0 0 0 0 0 0 0 0 0"
		&& lines[4] == "0 0 0 0 0 0 0 128 0 0 0 0 0 0 0"
		&& lines[5] == "0 0 0 0 0 0 0 0 0 0 0 0 0 0 255";
		});

	DOTEST(test, "Splitting long lines in PPM files", {
	Canvas c(10, 2);
	c.SetAllPixels(Color(1, 0.8, 0.6));
	PPM ppm = c.ToPPM();
	const auto& lines = ppm.GetLines();
	return lines[3] == "255 204 153 255 204 153 255 204 153 255 204 153 255 204 153 255 204"
		&& lines[4] == "153 255 204 153 255 204 153 255 204 153 255 204 153"
		&& lines[5] == "255 204 153 255 204 153 255 204 153 255 204 153 255 204 153 255 204"
		&& lines[6] == "153 255 204 153 255 204 153 255 204 153 255 204 153";
		});

	DOTEST(test, "PPM files are terminated by a newline character", {
	Canvas c(5, 3);
	PPM ppm = c.ToPPM();
	const auto& lines = ppm.GetLines();
	return lines.back().empty();
		});
}

void TestMatrix() {
	UnitTest test("matrix");
	DOTEST(test, "Constructing and inspecting a 4x4 matrix",
		{ Matrix<4,4> m; m.SetAll({1, 2, 3, 4}, {5.5, 6.5, 7.5, 8.5}, {9,10,11,12},{13.5,14.5,15.5,16.5});
	return m.Get<0,0>() == 1 && m.Get<0, 3>() == 4 && m.Get<1, 0>() == 5.5 &&
		m.Get<1, 2>() == 7.5 && m.Get<2, 2>() == 11 && m.Get<3, 0>() == 13.5 &&
		m.Get<3, 2>() == 15.5; });
	DOTEST(test, "A 2x2 matrix ought to be representable",
		{ Matrix<2,2> m; m.SetAll({-3, 5}, {1, -2});
	return m.Get<0, 0>() == -3 && m.Get<0, 1>() == 5 &&
		m.Get<1, 0>() == 1 && m.Get<1, 1>() == -2; });
	DOTEST(test, "A 3x3 matrix ought to be representable",
		{ Matrix<3,3> m; m.SetAll({-3, 5, 0}, {1, -2, -7}, {0, 1, 1});
	return m.Get<0, 0>() == -3 && m.Get<1, 1>() == -2 && m.Get<2, 2>() == 1; });
	DOTEST(test, "Matrix equality with identical matrices",
		{ Matrix<4,4> A; A.SetAll({1,2,3,4},{5,6,7,8},{9,8,7,6},{5,4,3,2});
		  Matrix<4, 4> B; B.SetAll({ 1,2,3,4 }, { 5,6,7,8 }, { 9,8,7,6 }, { 5,4,3,2 });
		  return A == B;
		});
	DOTEST(test, "Matrix equality with different matrices",
		{ Matrix<4,4> A; A.SetAll({1,2,3,4},{5,6,7,8},{9,8,7,6},{5,4,3,2});
		  Matrix<4, 4> B; B.SetAll({ 2,3,4,5 }, { 6,7,8,9 }, { 8,7,6,5 }, { 4,3,2,1 });
		  return A != B;
		});
	DOTEST(test, "Multiplying two matrices",
		{ Matrix<4,4> A; A.SetAll({1,2,3,4},{5,6,7,8},{9,8,7,6},{5,4,3,2});
		  Matrix<4,4> B; B.SetAll({-2,1,2,3},{3,2,1,-1},{4,3,6,5},{1,2,7,8});
		  Matrix<4,4> R; R.SetAll({20,22,50,48},{44,54,114,108},{40,58,110,102},{16,26,46,42});
		  return A * B == R;
		});
	DOTEST(test, "A matrix multiplied by a tuple",
		{ Matrix<4,4> A; A.SetAll({1,2,3,4},{2,4,4,2},{8,6,4,1},{0,0,0,1});
	Tuple b(1, 2, 3, 1);
	return A * b == Tuple(18, 24, 33, 1);
		});
	DOTEST(test, "Multiplying a matrix by the identify matrix",
		{ Matrix<4,4> A; A.SetAll({0,1,2,4},{1,2,4,8},{2,4,8,16},{4,8,16,32});
	return A * IdentityMatrix == A;
		});
	DOTEST(test, "Multiplying the identity matrix by a tuple",
		{ Tuple a(1,2,3,4);
	return IdentityMatrix * a == a;
		});
	DOTEST(test, "Transposing a matrix",
		{ Matrix<4,4> A; A.SetAll({0,9,3,0},{9,8,0,8},{1,8,5,3},{0,0,5,8});
		Matrix<4, 4> R; R.SetAll({ 0,9,1,0 }, { 9,8,8,0 }, { 3,0,5,5 }, { 0,8,3,8 });
	return A.Transpose() == R;
		});
	DOTEST(test, "Transposing the identify matrix",
		{ Matrix<4,4> A = IdentityMatrix;
	return A.Transpose() == IdentityMatrix;
		});
	DOTEST(test, "Calculating the determinant of a 2x2 matrix",
		{ Matrix<2,2> A; A.SetAll({1,5},{-3,2});
	return A.Determinant() == 17;
		});
	DOTEST(test, "A submatrix of a 3x3 matrix is a 2x2 matrix",
		{ Matrix<3,3> A; A.SetAll({1,5,0},{-3,2,7},{0,6,-3});
	Matrix<2, 2> R; R.SetAll({-3,2}, {0,6});
	return A.Submatrix(0, 2) == R;
		});
	DOTEST(test, "A submatrix of a 4x4 matrix is a 3x3 matrix",
		{ Matrix<4,4> A; A.SetAll({-6,1,1,6},{-8,5,8,6},{-1,0,8,2},{-7,1,-1,1});
	Matrix<3, 3> R; R.SetAll({ -6,1,6 }, { -8,8,6 }, { -7,-1,1 });
	return A.Submatrix(2, 1) == R;
		});
	DOTEST(test, "Calculating a minor of a 3x3 matrix",
		{ Matrix<3,3> A; A.SetAll({3,5,0},{2,-1,-7},{6,-1,5});
	Matrix<2, 2> B = A.Submatrix(1, 0);
	return B.Determinant() == 25 && A.Minor(1, 0) == 25;
		});
	DOTEST(test, "Calculating a cofactor of a 3x3 matrix",
		{ Matrix<3,3> A; A.SetAll({3,5,0},{2,-1,-7},{6,-1,5});
	return A.Minor(0, 0) == -12 && A.Cofactor(0, 0) == -12 && A.Minor(1, 0) == 25 && A.Cofactor(1, 0) == -25;
		});
	DOTEST(test, "Calculating the determinant of a 3x3 matrix",
		{ Matrix<3,3> A; A.SetAll({1,2,6},{-5,8,-4},{2,6,4});
	return A.Cofactor(0, 0) == 56 && A.Cofactor(0, 1) == 12 && A.Cofactor(0, 2) == -46 && A.Determinant() == -196;
		});
	DOTEST(test, "Calculating the determinant of a 4x4 matrix",
		{ Matrix<4,4> A; A.SetAll({-2,-8,3,5},{-3,1,7,3},{1,2,-9,6},{-6,7,7,-9});
	return A.Cofactor(0, 0) == 690 && A.Cofactor(0, 1) == 447 && A.Cofactor(0, 2) == 210 && A.Cofactor(0, 3) == 51 && A.Determinant() == -4071;
		});
	DOTEST(test, "Testing an invertible matrix for invertibility",
		{ Matrix<4,4> A; A.SetAll({6,4,4,4},{5,5,7,6},{4,-9,3,-7},{9,1,7,-6});
	return A.Determinant() == -2120 && A.IsInvertible();
		});
	DOTEST(test, "Testing an noninvertible matrix for invertibility",
		{ Matrix<4,4> A; A.SetAll({-4,2,-2,-3},{9,6,2,6},{0,-5,1,-5},{0,0,0,0});
	return A.Determinant() == 0 && !A.IsInvertible();
		});
	DOTEST(test, "Testing an noninvertible matrix for invertibility",
		{ Matrix<4,4> A; A.SetAll({-5,2,6,-8},{1,-5,1,8},{7,7,-6,-7},{1,-3,7,4});
	Matrix<4, 4> B = A.Inverse();
	Matrix<4, 4> R; R.SetAll(
		{ 0.21805,0.45113,0.24060,-0.04511 },
		{ -0.80827, -1.45677, -0.44361, 0.52068 },
		{ -0.07895,-0.22368,-0.05263,0.19737 },
		{ -0.52256,-0.81391,-0.30075,0.30639 }
	);
		return A.Determinant() == 532 && A.Cofactor(2, 3) == -160 &&
		almostEqual(B[3][2], -160.0 / 532) && A.Cofactor(3, 2) == 105 &&
		almostEqual(B[2][3], 105.0 / 532) && B.ApproximatelyEqual(R);
		});
	DOTEST(test, "Calculating the inverse of another matrix",
		{ Matrix<4,4> A; A.SetAll({8,-5,9,2},{7,5,6,1},{-6,0,9,6},{-3,0,-9,-4});
	Matrix<4, 4> B = A.Inverse();
	Matrix<4, 4> R; R.SetAll(
		{ -0.15385, -0.15385, -0.28205, -0.53846},
		{ -0.07692,0.12308,0.02564,0.03077},
		{ 0.35897,0.35897,0.43590,0.92308},
		{ -0.69231,-0.69231,-0.76923,-1.92308}
	);
		return B.ApproximatelyEqual(R);
		});
	DOTEST(test, "Calculating the inverse of third matrix",
		{ Matrix<4,4> A; A.SetAll({9,3,0,9},{-5,-2,-6,-3},{-4,9,6,4},{-7,6,6,2});
	Matrix<4, 4> B = A.Inverse();
	Matrix<4, 4> R; R.SetAll(
		{ -0.04074,-0.07778,0.14444,-0.22222},
		{ -0.07778,0.03333, 0.36667, -0.33333},
		{ -0.02901,-0.14630,-0.10926,0.12963 },
		{ 0.17778,0.06667,-0.26667,0.33333 }
	);
		return B.ApproximatelyEqual(R);
		});
	DOTEST(test, "Multiplying a product by its inverse",
		{ Matrix<4,4> A; A.SetAll({3,-9,7,3},{3,-8,2,-9},{-4,4,4,1},{-6,5,-1,1});
	Matrix<4, 4> B; B.SetAll({ 8,2,2,2 }, { 3,-1,7,0 }, { 7,0,5,4 }, { 6,-2,0,5 });
		return A * B * B.Inverse() == A;
		});
}

void TestTransformation() {
	UnitTest test("transformation");
	DOTEST(test, "Multiplying by a translation matrix",
		{
	Matrix<4,4> transform = Translation({5, -3, 2});
	Point p(-3, 4, 5);
	return transform * p == Point(2, 1, 7);
		});
	DOTEST(test, "Multiplying by the inverse of a translation matrix",
		{
	Matrix<4,4> transform = Translation({5, -3, 2});
	Matrix<4, 4> inv = transform.Inverse();
	Point p(-3, 4, 5);
	return inv * p == Point(-8, 7, 3);
		});
	DOTEST(test, "Translation does not affect vectors", {
	Matrix<4,4> transform = Translation({5, -3, 2});
	Vector v(-3, 4, 5);
	return transform * v == v;
		});
	DOTEST(test, "A scaling matrix applied to a point", {
	auto transform = Scaling({2,3,4});
	Point p(-4, 6, 8);
	return transform * p == Point(-8, 18, 32);
		});
	DOTEST(test, "A scaling matrix applied to a vector", {
	auto transform = Scaling({2,3,4});
	Vector v(-4, 6, 8);
	return transform * v == Vector(-8, 18, 32);
		});
	DOTEST(test, "Multiplying by the inverse of scaling matrix", {
	auto transform = Scaling({2,3,4 });
	auto inv = transform.Inverse();
	Vector v(-4, 6, 8);
	return inv * v == Vector(-2, 2, 2);
		});
	DOTEST(test, "Reflecting is scaling by a negative value", {
	auto transform = Scaling({-1,1,1});
	Point p(2, 3, 4);
	return transform * p == Point(-2, 3, 4);
		});
	DOTEST(test, "Rotating a point around the x axis", {
	Point p(0,1,0);
	auto halfQuarter = RotationX(M_PI / 4);
	auto fullQuarter = RotationX(M_PI / 2);
	const double sqrt2 = std::sqrt(2) / 2;
	return halfQuarter * p == Point(0, sqrt2, sqrt2)
		&& fullQuarter * p == Point(0,0,1);
		});
	DOTEST(test, "The inverse of an x-rotation rotates in the opposite direction", {
	Point p(0,1,0);
	auto halfQuarter = RotationX(M_PI / 4);
	auto inv = halfQuarter.Inverse();
	const double sqrt2 = std::sqrt(2) / 2;
	return inv * p == Point(0, sqrt2, -sqrt2);
		});
	DOTEST(test, "Rotating a point around the y axis", {
	Point p(0,0,1);
	auto halfQuarter = RotationY(M_PI / 4);
	auto fullQuarter = RotationY(M_PI / 2);
	const double sqrt2 = std::sqrt(2) / 2;
	return halfQuarter * p == Point(sqrt2, 0, sqrt2)
		&& fullQuarter * p == Point(1,0,0);
		});
	DOTEST(test, "Rotating a point around the z axis", {
	Point p(0,1,0);
	auto halfQuarter = RotationZ(M_PI / 4);
	auto fullQuarter = RotationZ(M_PI / 2);
	const double sqrt2 = std::sqrt(2) / 2;
	return halfQuarter * p == Point(-sqrt2, sqrt2, 0)
		&& fullQuarter * p == Point(-1,0,0);
		});
	DOTEST(test, "A shearing transformation moves x in proportion to y", {
	auto transform = Shearing(1, 0, 0, 0, 0, 0);
	Point p(2,3,4);
	return transform * p == Point(5,3,4);
		});
	DOTEST(test, "A shearing transformation moves x in proportion to z", {
	auto transform = Shearing(0, 1, 0, 0, 0, 0);
	Point p(2,3,4);
	return transform * p == Point(6,3,4);
		});
	DOTEST(test, "A shearing transformation moves y in proportion to x", {
	auto transform = Shearing(0, 0, 1, 0, 0, 0);
	Point p(2,3,4);
	return transform * p == Point(2,5,4);
		});
	DOTEST(test, "A shearing transformation moves y in proportion to z", {
	auto transform = Shearing(0, 0, 0, 1, 0, 0);
	Point p(2,3,4);
	return transform * p == Point(2,7,4);
		});
	DOTEST(test, "A shearing transformation moves z in proportion to x", {
	auto transform = Shearing(0, 0, 0, 0, 1, 0);
	Point p(2,3,4);
	return transform * p == Point(2,3,6);
		});
	DOTEST(test, "A shearing transformation moves z in proportion to y", {
	auto transform = Shearing(0, 0, 0, 0, 0, 1);
	Point p(2,3,4);
	return transform * p == Point(2,3,7);
		});
	DOTEST(test, "Individual transformations are applied in sequence", {
	Point p(1,0,1);
	auto A = RotationX(M_PI / 2);
	auto B = Scaling({ 5, 5, 5 });
	auto C = Translation({ 10, 5, 7 });
	// Apply rotation first
	auto p2 = A * p;
	if (p2 != Point(1, -1, 0)) return false;
	// then apply scaling
	auto p3 = B * p2;
	if (p3 != Point(5, -5, 0)) return false;
	// then apply translation
	auto p4 = C * p3;
	return p4 == Point(15,0,7);
		});
	DOTEST(test, "Chained transformations must be applied in reverse order", {
	Point p(1,0,1);
	auto A = RotationX(M_PI / 2);
	auto B = Scaling({ 5, 5, 5 });
	auto C = Translation({ 10, 5, 7 });
	auto T = C * B * A;
	return T * p == Point(15, 0, 7);
		});
	DOTEST(test, "Fluent chained transformations", {
	Point p(1,0,1);
	auto transform = IdentityMatrix.RotationX(M_PI / 2).Scaling({ 5, 5, 5 }).Translation({ 10, 5, 7 });
	return transform * p == Point(15, 0, 7);
		});
}

void TestRay() {
	UnitTest test("ray");
	DOTEST(test, "Creating and querying a ray", {
	Point origin(1,2,3);
	Vector direction(4, 5, 6);
	auto r = Ray(origin, direction);
	return r.origin == origin && r.direction == direction;
		});
	DOTEST(test, "Computing a point from a distance", {
	auto r = Ray(Point(2,3,4), Vector(1,0,0));
	return r.Position(0) == Point(2,3,4) && r.Position(1) == Point(3,3,4) &&
		r.Position(-1) == Point(1,3,4) && r.Position(2.5) == Point(4.5,3,4);
		});
	DOTEST(test, "A ray intersects a sphere at two points", {
	auto r = Ray(Point(0,0,-5), Vector(0,0,1));
	auto s = Sphere();
	const auto& xs = s.Intersect(r);
	const auto& sorted = xs.GetSorted();
	return xs.GetCount() == 2 && sorted[0].t == 4.0 && sorted[1].t == 6.0;
		});
	DOTEST(test, "A ray intersects a sphere at a tangent", {
	auto r = Ray(Point(0,1,-5), Vector(0,0,1));
	auto s = Sphere();
	const auto& xs = s.Intersect(r);
	const auto& sorted = xs.GetSorted();
	return xs.GetCount() == 2 && sorted[0].t == 5.0 && sorted[1].t == 5.0;
		});
	DOTEST(test, "A ray misses a sphere", {
	auto r = Ray(Point(0,2,-5), Vector(0,0,1));
	auto s = Sphere();
	const auto& xs = s.Intersect(r);
	return xs.GetCount() == 0;
		});
	DOTEST(test, "A ray originates inside a sphere", {
	auto r = Ray(Point(0,0,0), Vector(0,0,1));
	auto s = Sphere();
	const auto& xs = s.Intersect(r);
	const auto& sorted = xs.GetSorted();
	return xs.GetCount() == 2 && sorted[0].t == -1.0 && sorted[1].t == 1.0;
		});
	DOTEST(test, "A sphere is behind a ray", {
	auto r = Ray(Point(0,0,5), Vector(0,0,1));
	auto s = Sphere();
	const auto& xs = s.Intersect(r);
	const auto& sorted = xs.GetSorted();
	return xs.GetCount() == 2 && sorted[0].t == -6.0 && sorted[1].t == -4.0;
		});
	DOTEST(test, "Translating a ray", {
	auto r = Ray(Point(1,2,3), Vector(0,1,0));
	auto m = Translation({ 3,4,5 });
	auto r2 = r.Transform(m);
	return r2.origin == Point(4,6,8) && r2.direction == Vector(0,1,0);
		});
	DOTEST(test, "Scaling a ray", {
	auto r = Ray(Point(1,2,3), Vector(0,1,0));
	auto m = Scaling({ 2,3,4 });
	auto r2 = r.Transform(m);
	return r2.origin == Point(2,6,12) && r2.direction == Vector(0,3,0);
		});
}

void TestSphere() {
	MaterialManager materials;

	UnitTest test("sphere");
	DOTEST(test, "A sphere's default transformation", {
	auto s = Sphere();
	return s.GetTransform() == IdentityMatrix;
		});
	DOTEST(test, "Changing a sphere's transformation", {
	auto s = Sphere();
	auto t = Translation({ 2,3,4 });
	s.SetTransform(t);
	return s.GetTransform() == t;
		});
	DOTEST(test, "Intersecting a scaled sphere with a ray", {
	auto r = Ray(Point(0,0,-5), Vector(0,0,1));
	auto s = Sphere();
	s.SetTransform(Scaling({ 2,2,2 }));
	const auto& xs = s.Intersect(r);
	const auto& sorted = xs.GetSorted();
	return xs.GetCount() == 2 && sorted[0].t == 3 && sorted[1].t == 7;
		});
	DOTEST(test, "Intersecting a translated sphere with a ray", {
	auto r = Ray(Point(0,0,-5), Vector(0,0,1));
	auto s = Sphere();
	s.SetTransform(Translation({ 5,0,0 }));
	const auto& xs = s.Intersect(r);
	return xs.GetCount() == 0;
		});
	DOTEST(test, "The normal on a sphere at a point on the x axis", {
	auto s = Sphere();
	auto n = s.NormalAt(Point(1, 0, 0));
	return n == Vector(1,0,0);
		});
	DOTEST(test, "The normal on a sphere at a point on the y axis", {
	auto s = Sphere();
	auto n = s.NormalAt(Point(0, 1, 0));
	return n == Vector(0,1,0);
		});
	DOTEST(test, "The normal on a sphere at a point on the z axis", {
	auto s = Sphere();
	auto n = s.NormalAt(Point(0, 0, 1));
	return n == Vector(0,0,1);
		});
	DOTEST(test, "The normal on a sphere at a nonaxial point", {
	auto s = Sphere();
	double sq3 = std::sqrt(3) / 3;
	auto n = s.NormalAt(Point(sq3, sq3, sq3));
	return n == Vector(sq3, sq3, sq3);
		});
	DOTEST(test, "The normal is a normalized vector", {
	auto s = Sphere();
	double sq3 = std::sqrt(3) / 3;
	auto n = s.NormalAt(Point(sq3, sq3, sq3));
	return n == n.Normalize();
		});
	DOTEST(test, "Computing the normal on a translated sphere", {
	auto s = Sphere();
	s.SetTransform(Translation({ 0,1,0 }));
	auto n = s.NormalAt(Point(0, 1.70711, -0.70711));
	return ApproximatelyEqual(n, Vector(0, 0.70711, -0.70711));
		});
	DOTEST(test, "Computing the normal on a transformed sphere", {
	auto s = Sphere();
	s.SetTransform(Scaling({ 1,0.5,1 }) * RotationZ(M_PI / 5));
	auto n = s.NormalAt(Point(0, std::sqrt(2) / 2, -std::sqrt(2) / 2));
	return ApproximatelyEqual(n, Vector(0, 0.97014, -0.24254));
		});
	DOTEST(test, "A sphere has a default material", {
	auto s = Sphere();
	s.SetMaterial(materials.GetDefaultMaterial());
	return s.GetMaterial() == materials.GetDefaultMaterial();
		});
	DOTEST(test, "A sphere may be assigned a material", {
	auto s = Sphere();
	auto m = materials.CreateMaterial();
	m->ambient = 1;
	s.SetMaterial(m);
	return s.GetMaterial() == m;
		});
}

void TestLight() {
	UnitTest test("light");

	DOTEST(test, "A point light has a position and intensity", {
	Color intensity = Color(1,1,1);
	Point position = Point(0, 0, 0);
	PointLight light = PointLight(position, intensity);
	return light.GetPosition() == position && light.GetIntensity() == intensity;
		});
}

void TestMaterial() {
	UnitTest test("material");

	// Shared across all tests:
	const Material m;
	const Point position = Point(0, 0, 0);

	DOTEST(test, "The default material", {
	return m.color == Color(1,1,1) && m.ambient == 0.1 && m.diffuse == 0.9 && m.specular == 0.9 && m.shininess == 200.0;
		});
	DOTEST(test, "Background", {
	return m.ambient == 0.1 && m.diffuse == 0.9 && m.specular == 0.9 && m.shininess == 200.0;
		});
	DOTEST(test, "Lighting with the eye between the light and the surface", {
	auto eyev = Vector(0,0,-1);
	auto normalv = Vector(0, 0, -1);
	auto light = PointLight(Point(0, 0, -10), Color(1, 1, 1));
	auto result = Lighting(m, light, position, eyev, normalv, false);
	return result == Color(1.9, 1.9, 1.9);
		});
	DOTEST(test, "Lighting with the eye between the light and surface, eye offset 45deg", {
	double sq2 = std::sqrt(2) / 2;
	auto eyev = Vector(0,sq2,-sq2);
	auto normalv = Vector(0, 0, -1);
	auto light = PointLight(Point(0, 0, -10), Color(1, 1, 1));
	auto result = Lighting(m, light, position, eyev, normalv, false);
	return result == Color(1.0, 1.0, 1.0);
		});
	DOTEST(test, "Lighting with eye opposite surface, light offset 45deg", {
	auto eyev = Vector(0,0,-1);
	auto normalv = Vector(0, 0, -1);
	auto light = PointLight(Point(0, 10, -10), Color(1, 1, 1));
	auto result = Lighting(m, light, position, eyev, normalv, false);
	return ApproximatelyEqual(result, Color(0.7364, 0.7364, 0.7364));
		});
	DOTEST(test, "Lighting with eye in the path of the reflection vector", {
	double sq2 = std::sqrt(2) / 2;
	auto eyev = Vector(0,-sq2,-sq2);
	auto normalv = Vector(0, 0, -1);
	auto light = PointLight(Point(0, 10, -10), Color(1, 1, 1));
	auto result = Lighting(m, light, position, eyev, normalv, false);
	return ApproximatelyEqual(result, Color(1.6364, 1.6364, 1.6364));
		});
	DOTEST(test, "Lighting with the lght behind the surface", {
	auto eyev = Vector(0,0,-1);
	auto normalv = Vector(0, 0, -1);
	auto light = PointLight(Point(0, 0, 10), Color(1, 1, 1));
	auto result = Lighting(m, light, position, eyev, normalv, false);
	return result == Color(0.1, 0.1, 0.1);
		});
	DOTEST(test, "Lighting with the surface in the shadow", {
	auto eyev = Vector(0,0,-1);
	auto normalv = Vector(0, 0, -1);
	auto light = PointLight(Point(0, 0, -10), Color(1, 1, 1));
	bool inShadow = true;
	auto result = Lighting(m, light, position, eyev, normalv, inShadow);
	return result == Color(0.1, 0.1, 0.1);
		});
	DOTEST(test, "Lighting with a pattern applied", {
	Material m;
	m.pattern = std::make_unique<StripePattern>(Color(1, 1, 1), Color(0, 0, 0));
	m.ambient = 1;
	m.diffuse = 0;
	m.specular = 0;
	auto eyev = Vector(0, 0, -1);
	auto normalv = Vector(0, 0, -1);
	auto light = PointLight(Point(0, 0, -10), Color(1, 1, 1));
	auto c1 = Lighting(m, light, Point(0.9, 0, 0), eyev, normalv, false);
	auto c2 = Lighting(m, light, Point(1.1, 0, 0), eyev, normalv, false);
	return c1 == Color(1,1,1) && c2 == Color(0,0,0);
		});
}

void TestIntersection() {
	UnitTest test("intersection");
	DOTEST(test, "An intersection encapsulates t and object", {
	auto s = Sphere();
	auto i = Intersection(3.5, &s);
	return i.t == 3.5 && i.object == &s;
		});
	DOTEST(test, "Aggregatting intersections", {
	auto s = Sphere();
	auto i1 = Intersection(1, &s);
	auto i2 = Intersection(2, &s);
	auto xs = Intersections({ i1, i2 });
	const auto& sorted = xs.GetSorted();
	return xs.GetCount() == 2 && sorted[0].t == 1 && sorted[1].t == 2;
		});
	DOTEST(test, "Intersect sets the object on the intersection", {
	auto r = Ray(Point(0,0,-5),Vector(0,0,1));
	auto s = Sphere();
	const auto& xs = s.Intersect(r);
	const auto& sorted = xs.GetSorted();
	return xs.GetCount() == 2 && sorted[0].object == &s && sorted[1].object == &s;
		});
	DOTEST(test, "The hit, when all intersections have positive t", {
	auto s = Sphere();
	auto i1 = Intersection(1, &s);
	auto i2 = Intersection(2, &s);
	auto xs = Intersections({ i2, i1 });
	auto i = xs.Hit();
	return i == i1;
		});
	DOTEST(test, "The hit, when some intersections have negative t", {
	auto s = Sphere();
	auto i1 = Intersection(-1, &s);
	auto i2 = Intersection(2, &s);
	auto xs = Intersections({ i2, i1 });
	auto i = xs.Hit();
	return i == i2;
		});
	DOTEST(test, "The hit, when all intersections have negative t", {
	auto s = Sphere();
	auto i1 = Intersection(-2, &s);
	auto i2 = Intersection(-1, &s);
	auto xs = Intersections({ i2, i1 });
	auto i = xs.Hit();
	return !i;
		});
	DOTEST(test, "The hit is always the lowest nonnegative intersection", {
	auto s = Sphere();
	auto i1 = Intersection(5, &s);
	auto i2 = Intersection(7, &s);
	auto i3 = Intersection(-3, &s);
	auto i4 = Intersection(2, &s);
	auto xs = Intersections({ i1, i2, i3, i4 });
	auto i = xs.Hit();
	return i == i4;
		});
	DOTEST(test, "Precomputing the state of an intersection", {
	Ray r = Ray(Point(0,0,-5),Vector(0,0,1));
	Sphere shape = Sphere();
	auto i = Intersection(4, &shape);
	auto comps = PrepareComputations(i, r);
	return comps.object == i.object &&
		comps.point == Point(0,0,-1) &&
		comps.eyev == Vector(0,0,-1) &&
		comps.normalv == Vector(0,0,-1);
		});
	DOTEST(test, "The hit, when an intersection occurs on the outside", {
	Ray r = Ray(Point(0,0,-5),Vector(0,0,1));
	Sphere shape = Sphere();
	auto i = Intersection(4, &shape);
	auto comps = PrepareComputations(i, r);
	return comps.inside == false;
		});
	DOTEST(test, "The hit, when an intersection occurs on the inside", {
	Ray r = Ray(Point(0,0,0),Vector(0,0,1));
	Sphere shape = Sphere();
	auto i = Intersection(1, &shape);
	auto comps = PrepareComputations(i, r);
	return comps.point == Point(0,0,1) &&
		comps.eyev == Vector(0,0,-1) &&
		comps.inside == true &&
		comps.normalv == Vector(0,0,-1);
		});
	DOTEST(test, "The transformation matrix for the default orientation", {
	Point from = Point(0,0,0);
	Point to = Point(0, 0, -1);
	Vector up = Vector(0, 1, 0);
	auto t = ViewTransform(from, to, up);
	return t == IdentityMatrix;
		});
	DOTEST(test, "A view transformation matrix looking in positive z", {
	Point from = Point(0,0,0);
	Point to = Point(0, 0, 1);
	Vector up = Vector(0, 1, 0);
	auto t = ViewTransform(from, to, up);
	return t == Scaling({ -1,1,-1 });
		});
	DOTEST(test, "The view transformation moves the world", {
	Point from = Point(0,0,8);
	Point to = Point(0, 0, 0);
	Vector up = Vector(0, 1, 0);
	auto t = ViewTransform(from, to, up);
	return t == Translation({ 0,0,-8 });
		});
	DOTEST(test, "An arbitrary view transformation", {
	Point from = Point(1,3,2);
	Point to = Point(4, -2, 8);
	Vector up = Vector(1, 1, 0);
	auto t = ViewTransform(from, to, up);
	Matrix<4, 4> M;
	M.SetAll(
		{ -0.50709, 0.50709, 0.67612, -2.36643 },
		{ 0.76772, 0.60609, 0.12122, -2.82843 },
		{ -0.35857, 0.59761, -0.71714, 0.0 },
		{ 0.0, 0.0, 0.0, 1.0 });
	return t == M;
		});
	DOTEST(test, "The hit should offset the point", {
	Ray r = Ray(Point(0,0,-5),Vector(0,0,1));
	Sphere shape = Sphere();
	shape.SetTransform(Translation({ 0,0,1 }));
	auto i = Intersection(5, &shape);
	auto comps = PrepareComputations(i, r);
	return comps.overPoint.z < -myEpsilon / 2 && comps.point.z > comps.overPoint.z;
		});
}

void TestWorld() {
	UnitTest test("world");

	DOTEST(test, "Creating a world", {
	World w;
	return w.GetObjectCount() == 0 && !w.GetLightSource();
		});
	DOTEST(test, "The default world", {
	World w = World::Default();
	PointLight light = PointLight(Point(-10,10,-10), Color(1,1,1));
	Sphere s1;
	auto m1 = w._materialManager.CreateMaterial();
	m1->color = Color(0.8, 1.0, 0.6);
	m1->diffuse = 0.7;
	m1->specular = 0.2;
	s1.SetMaterial(m1);
	Sphere s2;
	s2.SetMaterial(w._materialManager.GetDefaultMaterial());
	s2.SetTransform(Scaling({ 0.5,0.5,0.5 }));
	return *w.GetLightSource() == light && *w.GetObject(0) == s1 && *w.GetObject(1) == s2;
		});
	DOTEST(test, "Intersection a world with a ray", {
	World w = World::Default();
	Ray r = Ray(Point(0, 0, -5), Vector(0, 0, 1));
	auto xs = w.Intersect(r);
	const auto& sorted = xs.GetSorted();
	return xs.GetCount() == 4 &&
		sorted[0].t == 4 &&
		sorted[1].t == 4.5 &&
		sorted[2].t == 5.5 &&
		sorted[3].t == 6;
		});
	DOTEST(test, "Shading an intersection", {
	World w = World::Default();
	Ray r = Ray(Point(0, 0, -5), Vector(0, 0, 1));
	Shape* shape = w.GetObject(0);
	auto i = Intersection(4, shape);
	auto comps = PrepareComputations(i, r);
	auto c = w.ShadeHit(comps);
	return ApproximatelyEqual(c, Color(0.38066, 0.47583, 0.2855));
		});
	DOTEST(test, "Shading an intersection from the inside", {
	World w = World::Default();
	w.SetLightSource(std::make_unique<PointLight>(Point(0, 0.25, 0), Color(1, 1, 1)));
	Ray r = Ray(Point(0, 0, 0), Vector(0, 0, 1));
	Shape* shape = w.GetObject(1);
	auto i = Intersection(0.5, shape);
	auto comps = PrepareComputations(i, r);
	auto c = w.ShadeHit(comps);
	return ApproximatelyEqual(c, Color(0.90498, 0.90498, 0.90498));
		});
	DOTEST(test, "The color when a ray misses", {
	World w = World::Default();
	Ray r = Ray(Point(0, 0, -5), Vector(0, 1, 0));
	Color c = w.ColorAt(r);
	return c == Color(0,0,0);
		});
	DOTEST(test, "The color when a ray hits", {
	World w = World::Default();
	Ray r = Ray(Point(0, 0, -5), Vector(0, 0, 1));
	Color c = w.ColorAt(r);
	return ApproximatelyEqual(c, Color(0.38066,0.47583,0.2855));
		});
	DOTEST(test, "The color with an intersection behind the ray", {
	World w = World::Default();
	Shape* outer = w.GetObject(0);
	auto m = w._materialManager.CreateMaterial();
	m->ambient = 1;
	outer->SetMaterial(m);
	Shape* inner = w.GetObject(1);
	inner->SetMaterial(m);
	Ray r = Ray(Point(0, 0, 0.75), Vector(0, 0, -1));
	Color c = w.ColorAt(r);
	return c == inner->GetMaterial()->color;
		});
	DOTEST(test, "There is no shadow when nothing is collinear with point and light", {
	World w = World::Default();
	Point p = Point(0, 10, 0);
	return !w.IsShadowed(p);
		});
	DOTEST(test, "The shadow when an object is between the point and the light", {
	World w = World::Default();
	Point p = Point(10, -10, 10);
	return w.IsShadowed(p);
		});
	DOTEST(test, "There is no shadow when an object is behind the light", {
	World w = World::Default();
	Point p = Point(-20, 20, -20);
	return !w.IsShadowed(p);
		});
	DOTEST(test, "There is no shadow when an object is behind the point", {
	World w = World::Default();
	Point p = Point(-2, 2, -2);
	return !w.IsShadowed(p);
		});
	DOTEST(test, "ShadeHit() is given an intersection in shadow", {
	World w;
	w.SetLightSource(std::make_unique<PointLight>(Point(0, 0, -10), Color(1, 1, 1)));
	auto s1 = std::make_unique<Sphere>();
	s1->SetMaterial(w._materialManager.GetDefaultMaterial());
	w.AddObject(std::move(s1));
	auto s2 = std::make_unique<Sphere>();
	s2->SetMaterial(w._materialManager.GetDefaultMaterial());
	s2->SetTransform(Translation({ 0,0,10 }));
	w.AddObject(std::move(s2));
	Ray r = Ray(Point(0, 0, 5), Vector(0, 0, 1));
	auto i = Intersection(4, w.GetObject(1));
	auto comps = PrepareComputations(i, r);
	auto c = w.ShadeHit(comps);
	return ApproximatelyEqual(c, Color(0.1, 0.1, 0.1));
		});
}

void TestCamera() {
	UnitTest test("camera");
	World w;
	DOTEST(test, "Constructing a camera", {
	Camera c = Camera(160, 120, M_PI / 2);
	return c.GetHSize() == 160 &&
		c.GetVSize() == 120 &&
		c.GetFOV() == (M_PI / 2) &&
		c.GetTransform() == IdentityMatrix;
		});
	DOTEST(test, "The pixel size for a horizontal canvas", {
	Camera c = Camera(200, 125, M_PI / 2);
	return almostEqual(c.GetPixelSize(), 0.01);
		});
	DOTEST(test, "The pixel size for a vertical canvas", {
	Camera c = Camera(125, 200, M_PI / 2);
	return almostEqual(c.GetPixelSize(), 0.01);
		});
	DOTEST(test, "Constructing a ray through the center of the canvas", {
	Camera c = Camera(201, 101, M_PI / 2);
	Ray r = c.RayForPixel(100, 50);
	return r.origin == Point(0,0,0) && r.direction == Vector(0,0,-1);
		});
	DOTEST(test, "Constructing a ray through a corner of the canvas", {
	Camera c = Camera(201, 101, M_PI / 2);
	Ray r = c.RayForPixel(0, 0);
	return r.origin == Point(0,0,0) && ApproximatelyEqual(r.direction, Vector(0.66519,0.33259,-0.66851));
		});
	DOTEST(test, "Constructing a ray when the camera is transformed", {
	Camera c = Camera(201, 101, M_PI / 2);
	c.SetTransform(RotationY(M_PI / 4)*Translation({ 0, -2, 5 }));
	Ray r = c.RayForPixel(100, 50);
	double hsq2 = std::sqrt(2) / 2;
	return r.origin == Point(0, 2, -5) && r.direction == Vector(hsq2, 0, -hsq2);
		});
	DOTEST(test, "Rendering a world with a camera", {
	World w = World::Default();
	Camera c(11, 11, M_PI / 2);
	Point from = Point(0, 0, -5);
	Point to = Point(0, 0, 0);
	Vector up = Vector(0, 1, 0);
	c.SetTransform(ViewTransform(from, to, up));
	Canvas image = c.Render(w);
	return ApproximatelyEqual(image.PixelAt(5, 5), Color(0.38066, 0.47583, 0.2855));
		});
}

void TestShapeFunc() {
	MaterialManager materials;

	UnitTest test("shape");
	DOTEST(test, "The default material", {
	TestShape s;
	s.SetMaterial(materials.GetDefaultMaterial());
	return s.GetMaterial() == materials.GetDefaultMaterial(); });
	DOTEST(test, "Assigning a material", {
	TestShape s;
	auto m = materials.CreateMaterial();
	m->ambient = 1;
	s.SetMaterial(m);
	return s.GetMaterial()->ambient == 1; });
	DOTEST(test, "Intersecting a scaled shape with a ray", {
	Ray r = Ray(Point(0,0,-5), Vector(0,0,1));
	TestShape s;
	s.SetTransform(Scaling({ 2,2,2 }));
	auto xs = s.Intersect(r);
	return s.savedRay->origin == Point(0,0,-2.5) && s.savedRay->direction == Vector(0,0,0.5); });
	DOTEST(test, "Intersecting a translated shape with a ray", {
	Ray r = Ray(Point(0,0,-5), Vector(0,0,1));
	TestShape s;
	s.SetTransform(Translation({ 5,0,0 }));
	auto xs = s.Intersect(r);
	return s.savedRay->origin == Point(-5,0,-5) && s.savedRay->direction == Vector(0,0,1); });
	DOTEST(test, "Computing the normal on a translated shape", {
	TestShape s;
	s.SetTransform(Translation({ 0,1,0 }));
	auto n = s.NormalAt(Point(0, 1.70711, -0.70711));
	return ApproximatelyEqual(n,Vector(0, 0.70711, -0.70711)); });
	DOTEST(test, "Computing the normal on a transformed shape", {
	TestShape s;
	s.SetTransform(Scaling({ 1,0.5,1 }) * RotationZ(M_PI / 5));
	double sq2 = std::sqrt(2) / 2;
	auto n = s.NormalAt(Point(0, sq2, -sq2));
	return ApproximatelyEqual(n, Vector(0, 0.97014, -0.24254)); });
}

void TestPlane() {
	UnitTest test("plane");
	DOTEST(test, "The normal of a plane is constant everywhere", {
	Plane p;
	auto n1 = p.NormalAt(Point(0, 0, 0));
	auto n2 = p.NormalAt(Point(10, 0, -10));
	auto n3 = p.NormalAt(Point(-5, 0, 150));
	return n1 == Vector(0,1,0) && n2 == n1 && n3 == n1; });
	DOTEST(test, "The normal of a plane is constant everywhere", {
	Plane p;
	auto r = Ray(Point(0, 10, 0), Vector(0, 0, 1));
	Intersections xs;
	p.LocalIntersect(r, xs);
	return xs.GetCount() == 0; });
	DOTEST(test, "The normal of a plane is constant everywhere", {
	Plane p;
	auto r = Ray(Point(0, 0, 0), Vector(0, 0, 1));
	Intersections xs;
	p.LocalIntersect(r, xs);
	return xs.GetCount() == 0; });
	DOTEST(test, "A ray intersecting a plane from above", {
	Plane p;
	auto r = Ray(Point(0, 1, 0), Vector(0, -1, 0));
	Intersections xs;
	p.LocalIntersect(r, xs);
	const auto& sorted = xs.GetSorted();
	return xs.GetCount() == 1 && sorted[0].t == 1 && sorted[0].object == &p; });
	DOTEST(test, "A ray intersecting a plane from below", {
	Plane p;
	auto r = Ray(Point(0, -1, 0), Vector(0, 1, 0));
	Intersections xs;
	p.LocalIntersect(r, xs);
	const auto& sorted = xs.GetSorted();
	return xs.GetCount() == 1 && sorted[0].t == 1 && sorted[0].object == &p; });
}

void TestPatterns() {
	UnitTest test("pattern");
	Color white = Color::White;
	Color black = Color::Black;

	DOTEST(test, "Creating a stripe pattern", {
	auto pattern = StripePattern(white, black);
	return pattern.GetColorA() == white && pattern.GetColorB() == black; });
	DOTEST(test, "A stripe pattern is constant in y", {
	auto pattern = StripePattern(white, black);
	return pattern.PatternAt(Point(0,0,0)) == white &&
		pattern.PatternAt(Point(0, 1, 0)) == white &&
		pattern.PatternAt(Point(0, 2, 0)) == white; });
	DOTEST(test, "A stripe pattern is constant in z", {
	auto pattern = StripePattern(white, black);
	return pattern.PatternAt(Point(0,0,0)) == white &&
		pattern.PatternAt(Point(0, 0, 1)) == white &&
		pattern.PatternAt(Point(0, 0, 2)) == white; });
	DOTEST(test, "A stripe pattern is alternates in x", {
	auto pattern = StripePattern(white, black);
	return pattern.PatternAt(Point(0,0,0)) == white &&
		pattern.PatternAt(Point(0.9, 0, 0)) == white &&
		pattern.PatternAt(Point(1, 0, 0)) == black &&
		pattern.PatternAt(Point(-0.1, 0, 0)) == black &&
		pattern.PatternAt(Point(-1, 0, 0)) == black &&
		pattern.PatternAt(Point(-1.1, 0, 0)) == white; });
	DOTEST(test, "Stripes with an object transformation", {
	auto object = Sphere();
	object.SetTransform(Scaling({ 2,2,2 }));
	auto pattern = StripePattern(white, black);
	Color c = pattern.PatternAtShape(&object, Point(1.5, 0, 0));
	return c == white; });
	DOTEST(test, "Stripes with a pattern transformation", {
	auto object = Sphere();
	auto pattern = StripePattern(white, black);
	pattern.SetTransform(Scaling({ 2,2,2 }));
	Color c = pattern.PatternAtShape(&object, Point(1.5, 0, 0));
	return c == white; });
	DOTEST(test, "Stripes with both an object and a pattern transformation", {
	auto object = Sphere();
	object.SetTransform(Scaling({ 2,2,2 }));
	auto pattern = StripePattern(white, black);
	pattern.SetTransform(Translation({ 0.5,0,0 }));
	Color c = pattern.PatternAtShape(&object, Point(2.5, 0, 0));
	return c == white; });
	DOTEST(test, "The default pattern transformation", {
	auto pattern = TestPattern();
	return pattern.GetTransform() == IdentityMatrix; });
	DOTEST(test, "Assigning a transformation", {
	auto pattern = TestPattern();
	pattern.SetTransform(Translation({ 1, 2, 3 }));
	return pattern.GetTransform() == Translation({ 1, 2, 3 }); });
	DOTEST(test, "A pattern with an object transformation", {
	auto shape = Sphere();
	shape.SetTransform(Scaling({ 2,2,2, }));
	auto pattern = TestPattern();
	Color c = pattern.PatternAtShape(&shape, Point(2, 3, 4));
	return c == Color(1,1.5,2); });
	DOTEST(test, "A pattern with both an object and a pattern transformation", {
	auto shape = Sphere();
	shape.SetTransform(Scaling({ 2,2,2, }));
	auto pattern = TestPattern();
	pattern.SetTransform(Translation({ 0.5, 1, 1.5 }));
	Color c = pattern.PatternAtShape(&shape, Point(2.5, 3, 3.5));
	return c == Color(0.75, 0.5, 0.25); });
	DOTEST(test, "A gradient linearly interpolates between colors", {
	auto pattern = GradientPattern(Color::White, Color::Black);
	return pattern.PatternAt(Point(0, 0, 0)) == Color::White &&
		pattern.PatternAt(Point(0.25, 0, 0)) == Color(0.75, 0.75, 0.75) &&
		pattern.PatternAt(Point(0.5, 0, 0)) == Color(0.5, 0.5, 0.5) &&
		pattern.PatternAt(Point(0.75, 0, 0)) == Color(0.25, 0.25, 0.25); });
	DOTEST(test, "A ring should extend in both x and z", {
	auto pattern = RingPattern(Color::White, Color::Black);
	return pattern.PatternAt(Point(0, 0, 0)) == Color::White &&
		pattern.PatternAt(Point(1, 0, 0)) == Color::Black &&
		pattern.PatternAt(Point(0, 0, 1)) == Color::Black &&
		pattern.PatternAt(Point(0.708, 0, 0.708)) == Color::Black; });
	DOTEST(test, "Checkers should repeat in x", {
	auto pattern = CheckerPattern(Color::White, Color::Black);
	return pattern.PatternAt(Point(0, 0, 0)) == Color::White &&
		pattern.PatternAt(Point(0.99, 0, 0)) == Color::White &&
		pattern.PatternAt(Point(1.01, 0, 0)) == Color::Black; });
}

//void TestPlane() {
//	UnitTest test("template");
//	DOTEST(test, "template", {
//	return true; });
//}

void RunTests() {
	TestTuple();
	TestColor();
	TestCanvas();
	TestMatrix();
	TestTransformation();
	TestRay();
	TestSphere();
	TestLight();
	TestIntersection();
	TestMaterial();
	TestWorld();
	TestCamera();
	TestShapeFunc();
	TestPlane();
	TestPatterns();

	//PerlinNoise p;
	//for (double x = 0; x < 5; x += 0.1) {
	//	std::cout << p.GetValue({ 0,x,0 }) << std::endl;
	//}
}
