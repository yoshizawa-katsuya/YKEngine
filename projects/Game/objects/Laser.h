#pragma once
#include "My3dObject.h"
#include "collider/OBBCollider.h"

class Laser : public OBBCollider
{
public:

	/// <summary>
	/// 初期化。
	/// </summary>
	/// <param name="leftPlayerWorldTransform">左プレイヤーのワールド変換行列</param>
	/// <param name="rightPlayerWorldTransform">右プレイヤーのワールド変換行列</param>
	void Initialize(YKEngine::WorldTransform* leftPlayerWorldTransform, YKEngine::WorldTransform* rightPlayerWorldTransform);

	/// <summary>
	///	更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画。
	/// </summary>
	/// <param name="camera">カメラ</param>
	void Draw(YKEngine::Camera* camera);

private:

	/// <summary>
	/// 移動
	/// </summary>
	void Move();

	/// <summary>
	/// 回転
	/// </summary>
	void Rotate();

	/// <summary>
	/// 長さを設定
	/// </summary>
	void UpdateLength();

	/// <summary>
	/// エネルギー量を更新
	/// </summary>
	void UpdateEnergy();

	//3Dオブジェクト
	std::unique_ptr<YKEngine::My3dObject> object_;

	//プレイヤーのTransform
	YKEngine::WorldTransform* leftPlayerWorldTransform_ = nullptr;
	YKEngine::WorldTransform* rightPlayerWorldTransform_ = nullptr;

	float energy_ = 100.0f; // レーザーのエネルギー量
	const float kMaxEnergy = 100.0f; // 最大エネルギー量
	const float kEnergyConsumptionRate = 10.0f; // エネルギー消費の基準値
};

