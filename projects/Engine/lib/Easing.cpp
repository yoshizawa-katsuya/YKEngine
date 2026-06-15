#define _USE_MATH_DEFINES
#include <numbers>
#include <cmath>
#include "Easing.h"

namespace YKEngine
{

float ApplyEasing(EasingType type, float t)
{
	//各イージング関数を適用
	switch (type)
	{
	case EasingType::kLinear:
		return t;
		break;
	case EasingType::kEaseInSine:
		return EaseInSine(t);
		break;
	case EasingType::kEaseOutSine:
		return EaseOutSine(t);
		break;
	case EasingType::kEaseInOutSine:
		return EaseInOutSine(t);
		break;
	case EasingType::kEaseInQuad:
		return EaseInQuad(t);
		break;
	case EasingType::kEaseOutQuad:
		return EaseOutQuad(t);
		break;
	case EasingType::kEaseInOutQuad:
		return EaseInOutQuad(t);
		break;
	case EasingType::kEaseInCubic:
		return EaseInCubic(t);
		break;
	case EasingType::kEaseOutCubic:
		return EaseOutCubic(t);
		break;
	case EasingType::kEaseInOutCubic:
		return EaseInOutCubic(t);
		break;
	case EasingType::kEaseInBack:
		return EaseInBack(t);
		break;
	case EasingType::kEaseOutBack:
		return EaseOutBack(t);
		break;
	case EasingType::kEaseInOutBack:
		return EaseInOutBack(t);
		break;
	default:
		return t;
		break;
	}
	return t;
}

float EaseInSine(float x) 
{
	return (1 - std::cos((x * std::numbers::pi_v<float>) / 2));
}

float EaseOutSine(float x) 
{
	return std::sin((x * std::numbers::pi_v<float>) / 2);
}

float EaseInOutSine(float x)
{
	return -(std::cos(std::numbers::pi_v<float> * x) - 1) / 2;
}

float EaseInQuad(float x)
{
	return x * x;
}

float EaseOutQuad(float x)
{
	return 1.0f - (1.0f - x) * (1.0f - x);
}

float EaseInOutQuad(float x)
{
	return x < 0.5f ? 2.0f * x * x : 1.0f - std::pow(-2.0f * x + 2.0f, 2.0f) / 2.0f;
}

float EaseInCubic(float x)
{
	return x * x * x;
}

float EaseOutCubic(float x)
{
	return 1.0f - std::pow(1.0f - x, 3.0f);
}

float EaseInOutCubic(float x)
{
	return x < 0.5f ? 4.0f * x * x * x : 1.0f - std::pow(-2.0f * x + 2.0f, 3.0f) / 2.0f;
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

float EaseInOutBack(float x)
{
	const float c1 = 1.70158f;
	const float c2 = c1 * 1.525f;

	return x < 0.5f
		? (std::pow(2.0f * x, 2.0f) * ((c2 + 1.0f) * 2.0f * x - c2)) / 2.0f
		: (std::pow(2.0f * x - 2.0f, 2.0f) * ((c2 + 1.0f) * (x * 2.0f - 2.0f) + c2) + 2.0f) / 2.0f;
}

} // namespace YKEngine