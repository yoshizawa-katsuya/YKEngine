#include "TransformHelpers.h"
#include "cmath"
#include "Struct.h"
#include "Matrix.h"

Vector3 TransformHelpers::FaceToVelocityDirection(const Vector3& rotate, const Vector3& velocity)
{
	// Y軸回り角度(θy)
	Vector3 newRotate = rotate;
	newRotate.y = std::atan2(velocity.x, velocity.z);
	Matrix4x4 minusThetaY = MakeRotateYMatrix(-newRotate.y);
	Vector3 velocityZ = Transform(velocity, minusThetaY);
	//X軸回り角度(θx)
	newRotate.x = std::atan2(-velocityZ.y, velocityZ.z);

	return newRotate;
}
