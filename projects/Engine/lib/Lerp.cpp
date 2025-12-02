#include "Lerp.h"
#include <numbers>
#include <cmath>
#include "TransformHelpers.h"

float Lerp(float p0, float p1, float t)
{
	return (1 - t) * p0 + t * p1;
}

Vector2 Lerp(Vector2 p0, Vector2 p1, float t) 
{

	Vector2 anser;

	anser.x = (1 - t) * p0.x + t * p1.x;
	anser.y = (1 - t) * p0.y + t * p1.y;

	return anser;
}

Vector3 Lerp(const Vector3& p0, const Vector3& p1, float t)
{

	Vector3 anser;

	anser.x = (1 - t) * p0.x + t * p1.x;
	anser.y = (1 - t) * p0.y + t * p1.y;
	anser.z = (1 - t) * p0.z + t * p1.z;

	return anser;
}

Vector4 Lerp(const Vector4& p0, const Vector4& p1, float t)
{
	Vector4 anser;

	anser.x = (1 - t) * p0.x + t * p1.x;
	anser.y = (1 - t) * p0.y + t * p1.y;
	anser.z = (1 - t) * p0.z + t * p1.z;
	anser.w = (1 - t) * p0.w + t * p1.w;

	return anser;
}

float LerpAngle(float a0, float a1, float t)
{
	float diff = TransformHelpers::NormalizeAngle(a1 - a0);	//最短差分を求める
	return a0 + diff * t;
}

Vector3 LerpAngle(const Vector3& a0, const Vector3& a1, float t)
{
	return Vector3(
		LerpAngle(a0.x, a1.x, t),
		LerpAngle(a0.y, a1.y, t),
		LerpAngle(a0.z, a1.z, t)
	);
}

Quaternion Lerp(const Quaternion& p0, const Quaternion& p1, float t)
{

	Quaternion anser;

	anser.x = (1 - t) * p0.x + t * p1.x;
	anser.y = (1 - t) * p0.y + t * p1.y;
	anser.z = (1 - t) * p0.z + t * p1.z;
	anser.w = (1 - t) * p0.w + t * p1.w;

	return anser;

}

float LeapShortAngle(float a, float b, float t)
{

	//角度差分を求める
	float diff = b - a;
	float mpi = static_cast<float>(std::numbers::pi_v<float>);

	diff = std::fmod(diff, 2.0f * mpi);

	if (diff > mpi) {
		diff -= 2.0f * mpi;
	}
	else if (diff < -mpi) {
		diff += 2.0f * mpi;
	}

	return a + diff * t;
}