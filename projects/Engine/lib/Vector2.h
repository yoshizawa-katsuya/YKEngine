#pragma once

namespace YKEngine
{

/// <summary>
/// 2次元ベクトル
/// </summary>
struct Vector2 {
	float x;
	float y;

	//スカラー倍
	Vector2& operator*=(float s) { x *= s;  y *= s; return *this; }
	//加算
	Vector2& operator-=(const Vector2& v) { x -= v.x; y -= v.y; return *this; }
	//減算
	Vector2& operator+=(const Vector2& v) { x += v.x; y += v.y; return *this; }
	//スカラー除算
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

//加算
Vector2 operator+(const Vector2& v1, const Vector2& v2);

//減算
Vector2 operator-(const Vector2& v1, const Vector2& v2);

//スカラー倍
Vector2 operator*(float s, const Vector2& v);

//スカラー倍
Vector2 operator*(const Vector2& v, float s);

//スカラー除算
Vector2 operator/(const Vector2& v, float s);

//単項マイナス
Vector2 operator-(const Vector2& v);

//単項プラス
Vector2 operator+(const Vector2& v);

}	// namespace YKEngine