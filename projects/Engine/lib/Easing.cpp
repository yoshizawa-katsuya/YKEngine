#define _USE_MATH_DEFINES
#include <numbers>
#include <cmath>
#include "Easing.h"

float ApplyEasing(EasingType type, float t)
{
	switch (type)
	{
	case EasingType::Linear:
		return t;
		break;
	case EasingType::EaseInSine:
		return EaseInSine(t);
		break;
	case EasingType::EaseOutSine:
		return EaseOutSine(t);
		break;
	case EasingType::EaseInQuad:
		return EaseInQuad(t);
		break;
	case EasingType::EaseOutQuad:
		return EaseOutQuad(t);
		break;
	case EasingType::EaseInCubic:
		return EaseInCubic(t);
		break;
	case EasingType::EaseOutCubic:
		return EaseOutCubic(t);
		break;
	default:
		return t;
		break;
	}
	return t;
}

float EaseInSine(float x) {
	return (1 - std::cos((x * std::numbers::pi_v<float>) / 2));
}

float EaseOutSine(float x) {
	return std::sin((x * std::numbers::pi_v<float>) / 2);
}

float EaseInQuad(float x)
{
	return x * x;
}

float EaseOutQuad(float x)
{
	return 1.0f - (1.0f - x) * (1.0f - x);
}

float EaseInCubic(float x)
{
	return x * x * x;
}

float EaseOutCubic(float x)
{
	return 1.0f - std::pow(1.0f - x, 3.0f);
}
