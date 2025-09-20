#pragma once
#include "Collider.h"
#include "SRTAnimator.h"
class Camera;
class EnemySpawnManager;

class RailMover : public Collider
{
public:

	void Initialize(const std::vector<Vector3>& controlPoints, EnemySpawnManager* enemySpawnManager);

	void Update();

	//レール描画
	void DrawRail(Camera* camera);

	//衝突時に呼ばれる関数
	void OnCollision([[maybe_unused]] Collider* other) override;

	WorldTransform* GetWorldTransform() { return &worldTransform_; }

	bool IsEnd() const { return moveCount_ >= pointsDrawing_.size(); }

	uint32_t GetNextEnemyWaveNumber() const { return nextEnemySpawnEventNumber_; }

	uint32_t GetNextSpeedWaveNumber() const { return nextSpeedEventNumber_; }

	uint32_t GetNextRotateWaveNumber() const { return nextRotateEventNumber_; }

	uint32_t GetNextRotateResetWaveNumber() const { return nextRotateResetEventNumber_; }

private:

	//曲線の作成
	void CreateSplineCurve(const std::vector<Vector3>& controlPoints);

	//向きを更新
	void UpdateRotate();

	// スプライン曲線制御点(通過点)Add commentMore actions
	std::vector<Vector3> controlPoints_;

	// 線分で描画する用の頂点リスト
	std::vector<Vector3> pointsDrawing_;
	// 線分の数
	const uint32_t segmentCount_ = 100;

	Vector3 target_;
	Vector3 forward_;

	float speed_ = 0.1f;

	uint32_t difference_ = 5;
	uint32_t moveCount_ = 0;

	uint32_t nextEnemySpawnEventNumber_ = 1;
	EnemySpawnManager* enemySpawnManager_ = nullptr;

	uint32_t nextSpeedEventNumber_ = 1;

	uint32_t nextRotateEventNumber_ = 1;
	//回転イベント中
	bool isInRotateEvent_ = false;
	std::unique_ptr<SRTAnimator> srtAnimator_;

	//回転リセットイベント用
	uint32_t nextRotateResetEventNumber_ = 1;

	bool isDrawRail_ = false;
};