#include "Matrix.h"


Matrix<4, 4> ViewTransform(const Point& from, const Point& to, const Vector& up) {
	Vector forward = (to - from).Normalize();
	Vector upn = up.Normalize();
	Vector left = forward.Cross(upn);
	Vector trueUp = left.Cross(forward);
	Matrix<4, 4> orientation;
	orientation.SetAll(
		{ left.x, left.y, left.z, 0 },
		{ trueUp.x, trueUp.y, trueUp.z, 0 },
		{ -forward.x, -forward.y, -forward.z, 0 },
		{ 0,0,0,1 });

	return orientation * Translation({ -from.x, -from.y, -from.z });
}
