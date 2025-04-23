#pragma once

struct Vector2 {
	float x;
	float y;
};

//減算
Vector2 Subtract(const Vector2& v1, const Vector2& v2);

//長さ
float Length(const Vector2& v);

//内積
float Dot(const Vector2& v1, const Vector2& v2);
