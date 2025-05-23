#include "TransformHelpers.h"
#include "cmath"
#include "Struct.h"
#include "Matrix.h"
#include "numbers"

Vector3 TransformHelpers::FaceToVelocityDirection(const Vector3& rotate, const Vector3& velocity)
{
	if (Length(velocity) == 0.0f)
	{
		return rotate;
	}
	// Y軸回り角度(θy)
	Vector3 newRotate = rotate;
	newRotate.y = std::atan2(velocity.x, velocity.z) + std::numbers::pi_v<float>;
	Matrix4x4 minusThetaY = MakeRotateYMatrix(-newRotate.y);
	Vector3 velocityZ = Transform(velocity, minusThetaY);
	//X軸回り角度(θx)
	newRotate.x = std::atan2(-velocityZ.y, -velocityZ.z);

	return newRotate;
}
