#include "PointLight.h"

bool operator==(const PointLight& lhs, const PointLight& rhs) noexcept {
	return lhs.GetIntensity() == rhs.GetIntensity() &&
		lhs.GetPosition() == rhs.GetPosition();
}
