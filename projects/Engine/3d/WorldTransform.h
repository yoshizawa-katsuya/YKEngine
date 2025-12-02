#pragma once
#include "Struct.h"

/// <summary>
/// ワールド変換データ。
/// 親子関係を持つことができる。
/// </summary>
class WorldTransform
{
public:

	// ローカルスケール
	Vector3 scale_ = { 1, 1, 1 };
	// X,Y,Z軸回りのローカル回転角
	Vector3 rotation_ = { 0, 0, 0 };
	// ローカル座標
	Vector3 translation_ = { 0, 0, 0 };
	// ローカル → ワールド変換行列
	Matrix4x4 worldMatrix_;
	// 親となるワールド変換へのポインタ
	WorldTransform* parent_ = nullptr;

	/// <summary>
	/// 初期化。
	/// </summary>
	void Initialize();
	/// <summary>
	/// 行列の更新。
	/// </summary>
	void UpdateMatrix();
};

