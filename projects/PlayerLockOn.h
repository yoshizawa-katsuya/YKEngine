#pragma once
#include "Camera.h"
#include "Player.h"
#include <memory>

#define PI  3.141592

class Camera;

class PlayerLockOn
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Camera* camera);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="player"></param>
	void Update(const std::unique_ptr<Player>& player);

	/// <summary>
	/// ターゲット検索
	/// </summary>
	/// <param name="player"></param>
	void Search(const std::unique_ptr<Player>& player);

	/// <summary>
	/// ワールドからスクリーン座標に変換
	/// </summary>
	/// <param name="worldPosition"></param>
	/// <returns></returns>
	Vector3 WorldToScreen(Vector3& worldPosition);

	/// <summary>
	/// 範囲外選択判定
	/// </summary>
	/// <returns></returns>
	bool OutsideSelectionRange();

	/// <summary>
	/// 中心座標取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetTargetPosition() const;

	/// <summary>
	/// ロックオン中かどうか確認
	/// </summary>
	/// <returns></returns>
	bool ExistTarget() const { return target_ ? true : false; }

private:

	Matrix4x4 viewProjection_;
	Matrix4x4 matView_;
	Matrix4x4 matProjection_;

	const Player* target_ = nullptr;
	Camera* camera_ = nullptr;

	// 最小距離
	float minDistance_ = 10.0f;
	// 最大距離
	float maxDistance_ = 30.0f;
	// 角度範囲
	float angleRange_ = 20.0f * static_cast<float>(PI) / 180.0f;

private:
	Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);
};

