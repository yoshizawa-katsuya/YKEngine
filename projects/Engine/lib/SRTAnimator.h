#pragma once
#include "Vector3.h"
#include "DirectXCommon.h"

/// <summary>
/// SRTアニメーション。
/// SRTとはScale, Rotate, Translateの略。
/// SRTでの単純なアニメーションを行う。
/// </summary>
class SRTAnimator
{
public:

	//アニメーションを設定
	void SetAnimation(const Vector3& startVector, const Vector3& endVector, float duration);

	//更新
	Vector3 Update();

	//更新。経過時間を外部で管理する
	Vector3 Update(float& elapsedTime);

private:

	Vector3 startVector_;
	Vector3 endVector_;

	//所要時間。単位は秒
	float duration_;

	//合計経過時間。単位は秒
	float elapsedTime_ = 0.0f;

	DirectXCommon* dxCommon_ = DirectXCommon::GetInstance();
};

