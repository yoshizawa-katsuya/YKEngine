#pragma once
#include "Input.h"
#include "Sprite.h"
#include "Enemy.h"
#include <memory>
#include <numbers>

/// <summary>
/// ロックオン
/// </summary>
class LockOn {
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="enemies">敵リスト</param>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Update(const std::list<std::unique_ptr<Enemy>>& enemies, const ViewProjection& viewProjection);

	//検索
	void Search(const std::list<std::unique_ptr<Enemy>>& enemies, const ViewProjection& viewProjection);

	//範囲外判定
	bool IsOutsideRange(const ViewProjection& viewProjection);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	//ロックオン対象の座標取得
	Vector3 GetTargetPosition() const;

	//ロックオン中か確認
	bool ExistTarget() const { return target_ ? true : false; }

private:

	XINPUT_STATE prejoyState_;

	//ロックオンマーク用スプライト
	std::unique_ptr<Sprite> lockOnMark_;

	//ロックオン対象
	const Enemy* target_ = nullptr;

	//度からラジアンへの変換
	const float kDegreeToRadian = (1.0f / 180.0f) * std::numbers::pi_v<float>;

	// 最小距離
	float minDistance_ = 10.0f;
	//最大距離
	float maxDistance_ = 60.0f;
	//角度範囲
	float angleRange_ = 20.0f * kDegreeToRadian;

};
