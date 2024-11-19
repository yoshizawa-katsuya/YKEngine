#pragma once
#include "Vector.h"

#define PI 3.141592f

// 前方宣言
class Camera;

class LockOnSystem
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// 描画
	/// </summary>
	//virtual void Draw() = 0;

	/// <summary>
	/// ターゲット検索
	/// </summary>
	virtual void Search() = 0;

	/// <summary>
	/// ワールドからスクリーン座標に変換
	/// </summary>
	/// <returns></returns>
	virtual Vector3 WorldToScreen() = 0;

	/// <summary>
	/// 範囲外選択判定
	/// </summary>
	/// <returns></returns>
	virtual bool OutsideSelectionRange() = 0;

	/// <summary>
	/// 中心座標取得
	/// </summary>
	/// <returns></returns>
	virtual Vector3 GetTargetPosition() const= 0;

protected:

	Matrix4x4 viewProjection_;
	Matrix4x4 matView_;

	const Camera* camera_ = nullptr;

	// 最小距離
	float minDistance_ = 0.0f;
	// 最大距離
	float maxDistance_ = 0.0f;
	// 角度範囲
	float angleRange_ = 0.0f;

};

