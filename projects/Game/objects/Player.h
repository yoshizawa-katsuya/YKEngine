#pragma once
#include "SRTAnimator.h"
#include "Animation.h"
#include "Input.h"
#include "Sprite.h"
#include "BaseCharacter.h"
#include "ReticleController.h"
class Camera;
class GameScene;
class Enemy;

class Player : public BaseCharacter
{
public:

	void Initialize(BaseModel* model, Matrix4x4* viewPortMatrix);

	void Update(Camera* railCamera);

	//旋回
	//void Rotate();

	//照準
	void ReticleUpdate(Camera* railCamera);

	//攻撃
	void Attack();

	//衝突時に呼ばれる関数
	void OnCollision([[maybe_unused]] Collider* other) override;

	//UI描画
	void DrawUI();

	void SetLockOnTarget(const std::list<std::unique_ptr<Enemy>>& enemies, Camera* railCamera);

	//親となるワールドトランスフォームをセット
	void SetParent(WorldTransform* parent);

	//ワールド座標を取得
	Vector3 GetWorldPosition();

	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

private:

	//キーボード入力
	Input* input_ = nullptr;

	//ゲームシーン
	GameScene* gameScene_ = nullptr;

	std::unique_ptr<ReticleController> reticleController_ = nullptr;

	int hitPoint_ = 5; // プレイヤーのヒットポイント
};

