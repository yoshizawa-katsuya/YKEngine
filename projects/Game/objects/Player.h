#pragma once
#include "SRTAnimator.h"
#include "Animation.h"
#include "Input.h"
#include "Sprite.h"
#include "BaseCharacter.h"
class Camera;
class GameScene;

class Player : public BaseCharacter
{
public:

	void Initialize(BaseModel* model) override;

	void Update(Camera* railCamera);

	//旋回
	void Rotate();

	//攻撃
	void Attack();

	//衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	//UI描画
	void DrawUI();

	void LockOn(const Vector2& position, const Vector3& targetPosition);

	//親となるワールドトランスフォームをセット
	void SetParent(WorldTransform* parent);

	void SetIsLockOn(bool isLockOn) { isLockOn_ = isLockOn; }

	//ワールド座標を取得
	Vector3 GetWorldPosition();

	//2Dレティクルの座標を取得
	Vector2 Get2DReticlePosition() { return sprite2DReticle_->GetPosition(); }

	Vector2 Get2DReticleSize() { return sprite2DReticle_->GetSize(); }

	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

private:

	//キーボード入力
	Input* input_ = nullptr;

	//ゲームシーン
	GameScene* gameScene_ = nullptr;

	//3Dレティクル用ワールドトランスフォーム
	WorldTransform worldTransform3DReticle_;

	//2Dレティクル用スプライト
	std::unique_ptr<Sprite> sprite2DReticle_;

	bool isLockOn_ = false;

	Vector3 target_;
};

