#pragma once
#include "Vector3.h"
#include "DirectXCommon.h"
#include "Easing.h"

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
	void SetAnimation(const Vector3& startVector, const Vector3& endVector, float duration);

	/// <summary>
	/// 更新。経過時間を内部で管理する
	/// </summary>
	/// <returns>現在のベクトル</returns>
	Vector3 Update();

	//更新。経過時間を外部で管理する
	Vector3 Update(float& elapsedTime);

	bool GetIsEnd() { return isEnd_; }

	void SetEasingType(EasingType easingType) { easingType_ = easingType; }

private:

	Vector3 startVector_;
	Vector3 endVector_;

	//所要時間。単位は秒
	float duration_;

	//合計経過時間。単位は秒
	float elapsedTime_ = 0.0f;

	//アニメーションが終わっていたらtrue
	bool isEnd_;

	EasingType easingType_ = EasingType::Linear;

	DirectXCommon* dxCommon_ = DirectXCommon::GetInstance();
};

