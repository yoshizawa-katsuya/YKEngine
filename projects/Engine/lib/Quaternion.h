#pragma once

/// <summary>
/// クォータニオン
/// x, y, zはベクトル成分、wはスカラー成分
/// </summary>
struct  Quaternion
{
	float x;
	float y;
	float z;
	float w;
};

//加算
Quaternion Add(const Quaternion& q1, const Quaternion& q2);

//正規化
Quaternion Normalize(const Quaternion& q);

//内積
float Dot(const Quaternion& q1, const Quaternion& q2);

//スカラー倍
Quaternion Multiply(float scalar, const Quaternion& q);

//長さ(ノルム)
float Length(const Quaternion& q);

//加算
Quaternion operator+(const Quaternion& q1, const Quaternion& q2);

//乗算
Quaternion operator*(float s, const Quaternion& q);

//乗算
Quaternion operator*(const Quaternion& q, float s);

//除算
Quaternion operator/(const Quaternion& q, float s);

//等価比較
bool operator==(const Quaternion& q1, const Quaternion& q2);
