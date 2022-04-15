#include "Math.h"

#include <glm/gtc/type_ptr.hpp>

constexpr float Math::SMALL_NUMBER = 0.0001f;

bool Math::IsNearlyZero(float value)
{
	return IsNearlyZero(value, SMALL_NUMBER);
}

bool Math::IsNearlyZero(float value, float errorMargin)
{
	return (glm::abs(value - errorMargin) <= 0.0f);
}
