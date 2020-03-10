#include "BaseTransform.h"

bool operator==(const BaseTransform& lhs, const BaseTransform& rhs) {
	return lhs.GetTransform() == rhs.GetTransform();
}
