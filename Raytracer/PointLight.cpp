#include "PointLight.h"

bool operator==(const PointLight& lhs, const PointLight& rhs) {
	return lhs.GetIntensity() == rhs.GetIntensity() &&
		lhs.GetPosition() == rhs.GetPosition();
}
