#include "Quaternion.h"

Quaternion Add(const Quaternion& q1, const Quaternion& q2)
{
	
	Quaternion anser;

	anser.x = q1.x + q2.x;
	anser.y = q1.y + q2.y;
	anser.z = q1.z + q2.z;
	anser.w = q1.w + q2.w;

	return anser;

}

Quaternion Normalize(const Quaternion& q) {
	float magnitude = std::sqrt(q.w * q.w + q.x * q.x + q.y * q.y + q.z * q.z);
	return { q.x / magnitude, q.y / magnitude, q.z / magnitude, q.w / magnitude };
}

float Dot(const Quaternion& q1, const Quaternion& q2)
{
	float anser;
	anser = (q1.x * q2.x) + (q1.y * q2.y) + (q1.z * q2.z) + (q1.w * q2.w);

	return anser;
}

Quaternion Multiply(float scalar, const Quaternion& q)
{
	
	Quaternion anser;

	anser.x = q.x * scalar;
	anser.y = q.y * scalar;
	anser.z = q.z * scalar;
	anser.w = q.w * scalar;

	return anser;

}

float Length(const Quaternion& q)
{
	float anser;
	anser = sqrtf(Dot(q, q));

	return anser;

}

Quaternion operator+(const Quaternion& q1, const Quaternion& q2)
{
	return Add(q1, q2);
}

Quaternion operator*(float s, const Quaternion& q)
{
	return Multiply(s, q);
}

Quaternion operator*(const Quaternion& v, float s)
{
	return s * v;
}

Quaternion operator/(const Quaternion& q, float s)
{
	return Multiply(1.0f / s, q);
}
