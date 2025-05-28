#define _USE_MATH_DEFINES
#include <math.h>
#include "Easing.h"

float ApplyEasing(EasingType type, float t)
{
	switch (type)
	{
	case EasingType::Linear:
		return t;
		break;
	case EasingType::EaseIn:
		return EaseIn(t);
		break;
	case EasingType::EaseOut:
		return EaseOut(t);
		break;
	default:
		return t;
		break;
	}
	return t;
}

float EaseIn(float x) {
	return (1 - cosf((x * float(M_PI)) / 2));
}

float EaseOut(float x) {
	return sinf((x * float(M_PI)) / 2);
}