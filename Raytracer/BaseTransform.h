#pragma once
#include "Matrix.h"

class BaseTransform {
public:
	virtual ~BaseTransform() {};

	void SetTransform(const Matrix<4, 4>& m) {
		_transform = m;
		_inverseTransform = _transform.Inverse();
		OnTransformedChanged();
	}
	Matrix<4, 4> GetTransform() const {
		return _transform;
	}
	Matrix<4, 4> GetInverseTransform() const {
		return _inverseTransform;
	}
	friend bool operator==(const BaseTransform& lhs, const BaseTransform& rhs);

protected:
	virtual void OnTransformedChanged() {}

private:
	Matrix<4, 4> _transform = IdentityMatrix;
	Matrix<4, 4> _inverseTransform = IdentityMatrix;
};
