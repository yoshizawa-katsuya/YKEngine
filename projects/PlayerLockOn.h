#pragma once
#include "LockOnSystem.h"

// 前方宣言
class Player;

class PlayerLockOn : public LockOnSystem
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// ターゲット検索
	/// </summary>
	void Search() override;

	/// <summary>
	/// ワールドからスクリーン座標に変換
	/// </summary>
	/// <returns></returns>
	Vector3 WorldToScreen() override;

	/// <summary>
	/// 範囲外選択判定
	/// </summary>
	/// <returns></returns>
	bool OutsideSelectionRange() override;

	/// <summary>
	/// 中心座標取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetTargetPosition() const override;

	/// <summary>
	/// ロックオン中かどうか確認
	/// </summary>
	/// <returns></returns>
	bool ExistTarget() const { return target_ ? true : false; }

	Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);

private:

	const Player* target_ = nullptr;

};

