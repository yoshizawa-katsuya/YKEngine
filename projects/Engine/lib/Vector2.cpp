#include "Vector2.h"
#include <optional>

namespace YKEngine
{

Vector2 Add(const Vector2& v1, const Vector2& v2)
{
	Vector2 anser;
	anser.x = v1.x + v2.x;
	anser.y = v1.y + v2.y;

	return anser;
}

Vector2 Subtract(const Vector2& v1, const Vector2& v2)
{
	Vector2 anser;
	anser.x = v1.x - v2.x;
	anser.y = v1.y - v2.y;

	return anser;
}

Vector2 Multiply(const Vector2& v, float s)
{
	Vector2 anser;
	anser.x = v.x * s;
	anser.y = v.y * s;

	return anser;
}

Vector2 Normalize(const Vector2& v)
{
	Vector2 anser;
	float length = Length(v);
	if (length == 0.0f) {
		return { 0.0f, 0.0f };
	}
	anser.x = v.x / length;
	anser.y = v.y / length;

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

Vector2 operator+(const Vector2& v1, const Vector2& v2)
{
	return Add(v1, v2);
}

Vector2 operator-(const Vector2& v1, const Vector2& v2)
{
	return Subtract(v1, v2);
}

Vector2 operator*(float s, const Vector2& v)
{
	return Multiply(v, s);
}

Vector2 operator*(const Vector2& v, float s)
{
	return s * v;
}

Vector2 operator/(const Vector2& v, float s)
{
	return Multiply(v, 1.0f / s);
}

Vector2 operator-(const Vector2& v)
{
	return {-v.x, -v.y};
}

Vector2 operator+(const Vector2& v)
{
	return v;
}

} // namespace YKEngine