#include "Lerp.h"
#include <numbers>
#include <cmath>

float Lerp(const float& p0, const float& p1, float t) {
	return (1 - t) * p0 + t * p1;
}

Vector2 Lerp(const Vector2& p0, const Vector2& p1, float t)
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