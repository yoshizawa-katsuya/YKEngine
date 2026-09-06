#pragma once
#include "My3dObject.h"
#include "Input.h"
#include "collider/SphereCollider.h"

class BasePlayer;

/// <summary>
/// 基底エネミークラス
/// </summary>
class BaseEnemy : public SphereCollider
{
public:

	/// <summary>
	/// 初期化。
	/// </summary>
	/// <param name="model">モデル</param>
	void Initialize();

	/// <summary>
	/// 更新。
	/// </summary>
	void Update();

	/// <summary>
	/// 描画。
	/// </summary>
	/// <param name="camera">カメラ</param>
	void Draw(YKEngine::Camera* camera);

	void SetTargets(const BasePlayer* target1, const BasePlayer* target2)
	{
		target1_ = target1;
		target2_ = target2;
	}

	/// <summary>
	/// 
	/// </summary>
	/// <param name="position">ターゲットのワールド変換</param>
	/// <param name="angle">角度(度数)</param>
	/// <returns></returns>
	bool IsWithinAngle(const YKEngine::Vector3& position, float angle);

	/// <summary>
	/// ターゲットとの内積を取得する。
	/// </summary>
	/// <param name="target">ターゲットのワールド変換</param>
	/// <returns>内積の値</returns>
	float GetDotProduct(const YKEngine::Vector3& position);

	/// <summary>
	/// 衝突時の処理
	/// </summary>
	/// <param name="other">他のコライダー</param>
	void OnCollision([[maybe_unused]] BaseCollider* other) override;
	
	/// <summary>
	/// 敵が生存しているかどうかを取得する。
	/// </summary>
	/// <returns>生存している場合はtrue、死亡している場合はfalse</returns>
	bool IsAlive() const { return isAlive_; }

protected:

	/// <summary>
	/// 移動。
	///	</summary>
	virtual void Move() = 0;

protected:

	// 3Dオブジェクト
	std::unique_ptr<YKEngine::My3dObject> object_;

	const BasePlayer* target1_ = nullptr;
	const BasePlayer* target2_ = nullptr;

	// 最大速度
	const float kMaxSpeed_ = 4.0f / 60.0f;

	// 速度
	YKEngine::Vector3 velocity_ = { 0.0f, 0.0f, 0.0f };

	//生存しているならtrue、死亡しているならfalse
	bool isAlive_ = true;
};