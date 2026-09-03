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

	//3Dオブジェクト
	std::unique_ptr<YKEngine::My3dObject> object_;

	//プレイヤーのTransform
	YKEngine::WorldTransform* leftPlayerWorldTransform_ = nullptr;
	YKEngine::WorldTransform* rightPlayerWorldTransform_ = nullptr;
};

