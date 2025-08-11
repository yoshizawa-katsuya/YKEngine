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

	void Initialize(BaseModel* model, Matrix4x4* viewPortMatrix, const std::vector<Vector3>& controlPoints);

	void Update(Camera* railCamera);

	//旋回
	//void Rotate();

	//衝突時に呼ばれる関数
	void OnCollision([[maybe_unused]] Collider* other) override;

	//UI描画
	void DrawUI();

	//レール描画
	void DrawRail(Camera* camera);

	void SetLockOnTarget(const std::list<std::unique_ptr<Enemy>>& enemies, Camera* railCamera);

	//ワールド座標を取得
	Vector3 GetWorldPosition();

	WorldTransform* GetbasePointWorldTransform() { return &basePointWorldTransform_; }

	bool IsEnd() const { return moveCount_ >= pointsDrawing_.size(); }

	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

private:

	//移動入力
	void HandleMoveInput();

	//照準
	void ReticleUpdate(Camera* railCamera);

	//攻撃
	void Attack();

	//曲線の作成
	void CreateSplineCurve(const std::vector<Vector3>& controlPoints);

	//向きを更新
	void UpdateRotate();

	//オフセットを更新
	void UpdateOffset();

	//キーボード入力
	Input* input_ = nullptr;

	//ゲームシーン
	GameScene* gameScene_ = nullptr;

	std::unique_ptr<ReticleController> reticleController_ = nullptr;

	int hitPoint_ = 5; // プレイヤーのヒットポイント

	// スプライン曲線制御点(通過点)Add commentMore actions
	std::vector<Vector3> controlPoints_;

	// 線分で描画する用の頂点リスト
	std::vector<Vector3> pointsDrawing_;
	// 線分の数
	const uint32_t segmentCount_ = 100;

	Vector3 eye_;
	Vector3 target_;
	Vector3 forward_;
	Vector3 offset_ = { 0.0f, 1.0f, 0.0f };

	float speed_ = 0.1f;

	uint32_t difference_ = 5;
	uint32_t moveCount_ = 0;

	WorldTransform basePointWorldTransform_;
};

