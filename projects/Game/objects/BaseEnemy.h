#pragma once
#include "My3dObject.h"
#include "WorldTransform.h"
#include "Input.h"

class BasePlayer;

/// <summary>
/// 基底エネミークラス
/// </summary>
class BaseEnemy
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

protected:

	/// <summary>
	/// 移動。
	///	</summary>
	virtual void Move() = 0;

protected:

	//Transform変数を作る
	YKEngine::WorldTransform worldTransform_;

	// 3Dオブジェクト
	std::unique_ptr<YKEngine::My3dObject> object_;

	const BasePlayer* target1_ = nullptr;
	const BasePlayer* target2_ = nullptr;

	// 最大速度
	const float kMaxSpeed_ = 4.0f / 60.0f;

	// 速度
	YKEngine::Vector3 velocity_ = { 0.0f, 0.0f, 0.0f };
};