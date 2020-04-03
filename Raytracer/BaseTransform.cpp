#include "BaseTransform.h"

bool operator==(const BaseTransform& lhs, const BaseTransform& rhs) noexcept {
	return lhs.GetTransform() == rhs.GetTransform();
}
