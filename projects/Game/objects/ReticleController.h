#pragma once
#include "WorldTransform.h"
#include <memory>
#include "Sprite.h"

namespace YKEngine
{
	class Input;
	class Camera;
}

class BaseEnemy;

/// <summary>
/// プレイヤーのレティクルを管理するクラス。
/// 2Dと3Dのレティクルを持ち、ロックオン機能を備えている。
/// 2Dレティクルは画面上に表示され、3Dレティクルはゲーム空間内でターゲットを示す。
/// ロックオン時にはターゲットに追従し、チャージマックス時には色が変化する。
/// </summary>
class ReticleController
{
public:

	/// <summary>
	/// 初期化処理。
	/// </summary>
	/// <param name="viewPortMatrix">ビューポート行列へのポインタ。ワールド座標への変換に使用される。</param>
	void Initialize();

	/// <summary>
	/// 更新処理。
	/// </summary>
	/// <param name="railCamera">レールカメラへのポインタ。ワールド座標への変換に使用される。</param>
	void Update(YKEngine::Camera* railCamera);

	/// <summary>
	/// 描画処理。
	/// </summary>
	void Draw();

	/// <summary>
	/// ロックオン対象の設定。
	/// </summary>
	/// <param name="enemies">敵キャラクターのリスト</param>
	/// <param name="railCamera">レールカメラへのポインタ。敵の位置をスクリーン座標に変換するために使用される。</param>
	void SetLockOnTarget(const std::list<std::unique_ptr<BaseEnemy>>& enemies, YKEngine::Camera* railCamera);

	/// <summary>
	/// チャージマックス時の処理。
	/// </summary>
	void ChargeMax();

	/// <summary>
	/// チャージリセット時の処理。
	/// </summary>
	void ChargeReset();

	bool IsLockOn() const { return isLockOn_; }

	const YKEngine::Vector3& GetTargetPosition() const { return target_; }

	YKEngine::Vector3 Get3DReticlePosition() { return worldTransform3DReticle_.GetWorldPosition(); }

private:

	/// <summary>
	/// チャージ更新処理。
	/// </summary>
	void ChargeUpdate();

	/// <summary>
	/// ロックオン処理。
	/// </summary>
	/// <param name="position">ターゲットのスクリーン座標上の位置</param>
	/// <param name="targetPosition">ターゲットのワールド座標上の位置</param>
	void LockOn(YKEngine::Vector2 position, const YKEngine::Vector3& targetPosition);

	//3Dレティクル用ワールドトランスフォーム
	YKEngine::WorldTransform worldTransform3DReticle_;

	//2Dレティクル用スプライト
	std::unique_ptr<YKEngine::Sprite> spriteLargeReticle_;
	std::unique_ptr<YKEngine::Sprite> spriteSmallReticle_;

	bool isLockOn_ = false;

	YKEngine::Vector3 target_;

	//ビューポート行列
	YKEngine::Matrix4x4* viewPortMatrix_ = nullptr;

	YKEngine::Input* input_ = nullptr;

	YKEngine::Vector4 defaultColor_ = { 1.0f, 1.0f, 0.1f, 1.0f };
	YKEngine::Vector4 chargeMaxColor_ = { 0.0f, 0.0f, 1.0f, 1.0f };
	bool isChargeMax_ = false;
	float chargeMaxTimer_ = 0.0f;
	//チャージマックス時に色に変化し終わるまでの時間
	const float kChargeMaxColorChangeTime_ = 0.1f;
};

