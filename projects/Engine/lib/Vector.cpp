#include "Vector.h"
#include "Matrix.h"
#include "WinApp.h"

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

Vector3 Add(const Vector3& v1, const Vector3& v2) {

	Vector3 anser;
	anser.x = v1.x + v2.x;
	anser.y = v1.y + v2.y;
	anser.z = v1.z + v2.z;

	return anser;

}

Vector3 Subtract(const Vector3& v1, const Vector3& v2) {

	Vector3 anser;
	anser.x = v1.x - v2.x;
	anser.y = v1.y - v2.y;
	anser.z = v1.z - v2.z;

	return anser;

}

Vector3 Multiply(float scalar, const Vector3& v) {

	Vector3 anser;
	anser.x = scalar * v.x;
	anser.y = scalar * v.y;
	anser.z = scalar * v.z;

	return anser;

}

float Dot(const Vector3& v1, const Vector3& v2) {

	float anser;
	anser = (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);

	return anser;

}

float Length(const Vector3& v) {

	float anser;
	anser = sqrtf(Dot(v, v));

	return anser;

}

Vector3 Normalize(const Vector3& v) {

	Vector3 anser;
	float length = Length(v);
	anser.x = v.x / length;
	anser.y = v.y / length;
	anser.z = v.z / length;

	return anser;

}
/*
void VectorScreenPrintf(int x, int y, const Vector3& vector, const char* label) {
	Novice::ScreenPrintf(x, y, "%.02f", vector.x);
	Novice::ScreenPrintf(x + kColumnWidth, y, "%.02f", vector.y);
	Novice::ScreenPrintf(x + kColumnWidth * 2, y, "%.02f", vector.z);
	Novice::ScreenPrintf(x + kColumnWidth * 3, y, "%s", label);
}
*/
Vector3 Cross(const Vector3& v1, const Vector3& v2) {

	Vector3 v;

	v.x = v1.y * v2.z - v1.z * v2.y;
	v.y = v1.z * v2.x - v1.x * v2.z;
	v.z = v1.x * v2.y - v1.y * v2.x;

	return v;

}

Vector3 Project(const Vector3& v1, const Vector3& v2) {

	Vector3 b = Normalize(v2);
	float ab = Dot(v1, b);
	Vector3 c = Multiply(ab, b);

	return c;

}
/*
Vector3 ClosestPoint(const Vector3& point, const Segment& segment) {

	Vector3 o = segment.origin;
	Vector3 a = Subtract(point, o);
	Vector3 b = segment.diff;
	float bLength = Length(b);

	float t = Dot(a, b) / (bLength * bLength);

	Vector3 tb = Multiply(t, b);

	Vector3 cp = Add(o, tb);

	return cp;

}
*/
Vector3 Perpendicular(const Vector3& vector) {
	if (vector.x != 0.0f || vector.y != 0.0f) {
		return { -vector.y, vector.x, 0.0f };
	}
	return { 0.0f, -vector.z, vector.y };
}

Vector3 ConvertingToScreen(const Vector3& position, Camera* camera) {

	//ビューポート行列
	Matrix4x4 matViewport = MakeViewportMatrix(0, 0, WinApp::kClientWidth, WinApp::kClientHeight, 0, 1);

	Matrix4x4 matViewProjectionViewport = camera->GetViewMatrix() * camera->GetProjection() * matViewport;

	Vector3 ScreenPosition = Transform(position, matViewProjectionViewport);

	return ScreenPosition;

}

Vector3 operator+(const Vector3& v1, const Vector3& v2) { return Add(v1, v2); }

Vector3 operator-(const Vector3& v1, const Vector3& v2) { return Subtract(v1, v2); }

Vector3 operator*(float s, const Vector3& v) { return Multiply(s, v); }

Vector3 operator*(const Vector3& v, float s) { return s * v; }

Vector3 operator/(const Vector3& v, float s) { return Multiply(1.0f / s, v); }

Vector3 operator-(const Vector3& v) { return { -v.x, -v.y, -v.z }; }

Vector3 operator+(const Vector3& v) { return v; }