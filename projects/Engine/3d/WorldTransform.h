#pragma once
#include "Struct.h"

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
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 行列を計算する
	/// </summary>
	void UpdateMatrix();
	/// <summary>
	/// ワールド座標の取得
	/// </summary>
	Vector3 GetWorldPosition();
};

