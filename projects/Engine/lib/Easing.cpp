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
	case EasingType::EaseInBack:
		return EaseInBack(t);
		break;
	case EasingType::EaseOutBack:
		return EaseOutBack(t);
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

float EaseInBack(float x)
{
	float c1 = 1.70158f;
	float c3 = c1 + 1.0f;
	return c3 * x * x * x - c1 * x * x;
}

float EaseOutBack(float x)
{
	float c1 = 1.70158f;
	float c3 = c1 + 1.0f;
	return 1.0f + c3 * std::pow(x - 1.0f, 3.0f) + c1 * std::pow(x - 1.0f, 2.0f);
}
