#pragma once

struct Vector2 {
	float x;
	float y;

	Vector2& operator*=(float s) { x *= s;  y *= s; return *this; }
	Vector2& operator-=(const Vector3& v) { x -= v.x; y -= v.y; return *this; }
	Vector2& operator+=(const Vector3& v) { x += v.x; y += v.y; return *this; }
	Vector2& operator/=(float s) { x /= s;  y /= s; return *this; }
};

//加算
Vector2 Add(const Vector2& v1, const Vector2& v2);

//減算
Vector2 Subtract(const Vector2& v1, const Vector2& v2);

//スカラー倍
Vector2 Multiply(const Vector2& v, float s);

//正規化
Vector2 Normalize(const Vector2& v);

//長さ
float Length(const Vector2& v);

//内積
float Dot(const Vector2& v1, const Vector2& v2);

Vector2 operator+(const Vector2& v1, const Vector2& v2);

Vector2 operator-(const Vector2& v1, const Vector2& v2);

Vector2 operator*(float s, const Vector2& v);

Vector2 operator*(const Vector2& v, float s);

Vector2 operator/(const Vector2& v, float s);

Vector2 operator-(const Vector2& v);

Vector2 operator+(const Vector2& v);