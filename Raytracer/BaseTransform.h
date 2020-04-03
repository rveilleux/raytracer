#pragma once
#include "Matrix.h"

class BaseTransform {
public:
	virtual ~BaseTransform() = default;
	BaseTransform() = default;
	BaseTransform(const BaseTransform&) = default;
	BaseTransform(BaseTransform&&) = default;
	BaseTransform& operator=(const BaseTransform&) = default;
	BaseTransform& operator=(BaseTransform&&) = default;

	void SetTransform(const Matrix<4, 4>& m) noexcept {
		_transform = m;
		_inverseTransform = _transform.Inverse();
		OnTransformedChanged();
	}
	Matrix<4, 4> GetTransform() const noexcept {
		return _transform;
	}
	Matrix<4, 4> GetInverseTransform() const noexcept {
		return _inverseTransform;
	}
	friend bool operator==(const BaseTransform& lhs, const BaseTransform& rhs) noexcept;

protected:
	virtual void OnTransformedChanged() noexcept {}

private:
	Matrix<4, 4> _transform = IdentityMatrix;
	Matrix<4, 4> _inverseTransform = IdentityMatrix;
};
