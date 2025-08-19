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

	void Initialize(BaseModel* model, Matrix4x4* viewPortMatrix, WorldTransform* parent, uint32_t heartTextureHandle, uint32_t heartEmptyTexturehandle);

	void Update(Camera* railCamera);

	//旋回
	//void Rotate();

	//衝突時に呼ばれる関数
	void OnCollision([[maybe_unused]] Collider* other) override;

	//UI描画
	void DrawUI();

	void SetLockOnTarget(const std::list<std::unique_ptr<Enemy>>& enemies, Camera* railCamera);

	//ワールド座標を取得
	Vector3 GetWorldPosition();

	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

private:

	//HUD初期化
	void HUDInitialize(uint32_t heartTextureHandle, uint32_t heartEmptyTexturehandle);

	//移動入力
	void HandleMoveInput();

	//照準
	void ReticleUpdate(Camera* railCamera);

	//攻撃
	void Attack();

	//キーボード入力
	Input* input_ = nullptr;

	//ゲームシーン
	GameScene* gameScene_ = nullptr;

	std::unique_ptr<ReticleController> reticleController_ = nullptr;

	Vector3 direction_{};	//方向

	const int maxHitPoint_ = 5; // 最大ヒットポイント
	int hitPoint_ = maxHitPoint_; // プレイヤーのヒットポイント

	std::vector<std::unique_ptr<Sprite>> heratSprites_; // ヒットポイントのスプライト
	std::vector<std::unique_ptr<Sprite>> heratEmptySprites_; // ヒットポイントがないときのスプライト
};

