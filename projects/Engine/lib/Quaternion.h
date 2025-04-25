#pragma once

struct  Quaternion
{
	float x;
	float y;
	float z;
	float w;
};

//加算
Quaternion Add(const Quaternion& q1, const Quaternion& q2);

Quaternion Normalize(const Quaternion& q);

//内積
float Dot(const Quaternion& q1, const Quaternion& q2);

//スカラー倍
Quaternion Multiply(float scalar, const Quaternion& q);

//長さ(ノルム)
float Length(const Quaternion& q);

Quaternion operator+(const Quaternion& q1, const Quaternion& q2);

Quaternion operator*(float s, const Quaternion& q);

Quaternion operator*(const Quaternion& q, float s);

Quaternion operator/(const Quaternion& q, float s);

bool operator==(const Quaternion& q1, const Quaternion& q2);
