#pragma once

struct Vector3 {
	float x;
	float y;
	float z;

	Vector3& operator*=(float s) { x *= s;  y *= s; z *= s; return *this; }
	Vector3& operator-=(const Vector3& v) { x -= v.x; y -= v.y; z -= v.z; return *this; }
	Vector3& operator+=(const Vector3& v) { x += v.x; y += v.y; z += v.z; return *this; }
	Vector3& operator/=(float s) { x /= s;  y /= s; z /= s; return *this; }

};


//加算
Vector3 Add(const Vector3& v1, const Vector3& v2);

//減算
Vector3 Subtract(const Vector3& v1, const Vector3& v2);

//スカラー倍
Vector3 Multiply(float scalar, const Vector3& v);

//内積
float Dot(const Vector3& v1, const Vector3& v2);

//長さ(ノルム)
float Length(const Vector3& v);

//正規化
Vector3 Normalize(const Vector3& v);

//void VectorScreenPrintf(int x, int y, const Vector3& vector, const char* label);

//クロス積
Vector3 Cross(const Vector3& v1, const Vector3& v2);

//正射影ベクトル
Vector3 Project(const Vector3& v1, const Vector3& v2);

//最近接点
//Vector3 ClosestPoint(const Vector3& point, const Segment& segment);

//垂直なベクトルを求める
Vector3 Perpendicular(const Vector3& vector);

//スクリーン座標に変換
//Vector3 ConvertingToScreen(const Vector3& position, const ViewProjection& viewProjection);

Vector3 operator+(const Vector3& v1, const Vector3& v2);

Vector3 operator-(const Vector3& v1, const Vector3& v2);

Vector3 operator*(float s, const Vector3& v);

Vector3 operator*(const Vector3& v, float s);

Vector3 operator/(const Vector3& v, float s);

Vector3 operator-(const Vector3& v);

Vector3 operator+(const Vector3& v);