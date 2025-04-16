#include "Vector2.h"
#include <optional>

Vector2 Subtract(const Vector2& v1, const Vector2& v2)
{
	Vector2 anser;
	anser.x = v1.x - v2.x;
	anser.y = v1.y - v2.y;

	return anser;
}

float Length(const Vector2& v)
{
	float anser;
	anser = sqrtf(Dot(v, v));

	return anser;
}

float Dot(const Vector2& v1, const Vector2& v2)
{
	float anser;
	anser = (v1.x * v2.x) + (v1.y * v2.y);

	return anser;
}