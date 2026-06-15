#pragma once
#include "Vector3.h"
#include "DirectXCommon.h"
#include "Easing.h"

namespace YKEngine
{

/// <summary>
/// SRTアニメーション。
/// SRTとはScale, Rotate, Translateの略。
/// SRTでの単純なアニメーションを行う。
/// </summary>
class SRTAnimator
{
public:

	/// <summary>
	/// アニメーションの設定。
	/// </summary>
	/// <param name="startVector">開始ベクトル</param>
	/// <param name="endVector">終了ベクトル</param>
	/// <param name="duration">所要時間（秒）</param>
	/// <param name="isPingPong">往復アニメーションするかどうか</param>
	void SetAnimation(const Vector3& startVector, const Vector3& endVector, float duration, bool isPingPong = false);

	/// <summary>
	/// 更新。経過時間を内部で管理する
	/// </summary>
	/// <returns>現在のベクトル</returns>
	Vector3 Update();

	//更新。経過時間を外部で管理する
	Vector3 Update(float& elapsedTime);

	bool GetIsEnd() const { return isEnd_; }

	void SetEasingType(EasingType easingType) { easingType_ = easingType; }

private:

	/// <summary>
	/// 経過時間の更新。経過時間が所要時間を超えないようにする。
	/// </summary>
	/// <param name="elapsedTime">経過時間</param>
	void UpdateElapsedTime(float& elapsedTime);

	Vector3 startVector_;
	Vector3 endVector_;

	//所要時間。単位は秒
	float duration_;

	//合計経過時間。単位は秒
	float elapsedTime_ = 0.0f;

	//アニメーションが終わっていたらtrue
	bool isEnd_;

	//アニメーションを往復させるならtrue
	bool isPingPong_ = false;

	EasingType easingType_ = EasingType::kLinear;

	DirectXCommon* dxCommon_ = DirectXCommon::GetInstance();
};

} // namespace YKEngine