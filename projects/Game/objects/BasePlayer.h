#pragma once
#include "My3dObject.h"
#include "WorldTransform.h"
#include "Input.h"

/// <summary>
/// プレイヤークラス
/// </summary>
class BasePlayer
{
public:

	/// <summary>
	/// 初期化。
	/// </summary>
	/// <param name="model">モデル</param>
	void Initialize(YKEngine::BaseModel* model);

	/// <summary>
	/// 更新。
	/// </summary>
	void Update();

	/// <summary>
	/// 描画。
	/// </summary>
	/// <param name="camera">カメラ</param>
	void Draw(YKEngine::Camera* camera);

	/// <summary>
	/// 自分ではない方の自機のワールド変換行列を設定する。
	/// </summary>
	/// <param name="otherPlayerWorldTransform">自分ではない方の自機のワールド変換行列</param>
	void SetOtherPlayerWorldTransform(YKEngine::WorldTransform* otherPlayerWorldTransform) { otherPlayerWorldTransform_ = otherPlayerWorldTransform; }

	/// <summary>
	/// 自機のワールド変換行列を取得する。
	/// </summary>
	/// <returns>自機のワールド変換行列</returns>
	YKEngine::WorldTransform* GetWorldTransform() { return &worldTransform_; }

protected:

	/// <summary>
	/// 移動。
	/// </summary>
	virtual void Move() = 0;

	/// <summary>
	/// 回転。
	/// </summary>
	void Rotate();

	//入力
	YKEngine::Input* input_ = nullptr;

	//Transform変数を作る
	YKEngine::WorldTransform worldTransform_;

	//3Dオブジェクト
	std::unique_ptr<YKEngine::My3dObject> object_;

	//自分ではない方の自機のワールド変換行列を取得するためのポインタ
	YKEngine::WorldTransform* otherPlayerWorldTransform_ = nullptr;
};

