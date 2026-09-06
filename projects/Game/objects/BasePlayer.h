#pragma once
#include "My3dObject.h"
#include "Input.h"
#include "collider/SphereCollider.h"

/// <summary>
/// プレイヤークラス
/// </summary>
class BasePlayer : public SphereCollider
{
public:

	/// <summary>
	/// 初期化。
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="hp">体力</param>
	void Initialize(YKEngine::BaseModel* model, int32_t* hp);

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

	/// <summary>
	/// プレイヤーの位置を取得します。
	/// </summary>
	/// <returns>プレイヤーの位置（Vector3）</returns>
	const YKEngine::Vector3 GetPosition() const { return worldTransform_.translation_; }

	/// <summary>
	/// 衝突時の処理
	/// </summary>
	/// <param name="other">衝突したコライダー</param>
	void OnCollision([[maybe_unused]] BaseCollider* other) override;

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

	//3Dオブジェクト
	std::unique_ptr<YKEngine::My3dObject> object_;

	//自分ではない方の自機のワールド変換行列を取得するためのポインタ
	YKEngine::WorldTransform* otherPlayerWorldTransform_ = nullptr;

	//プレイヤーの体力
	int32_t* hp_ = nullptr;

};

