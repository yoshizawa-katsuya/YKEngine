#pragma once
#include "Collider.h"
#include "SRTAnimator.h"
class YKEngine::Camera;
class EnemySpawnManager;

/// <summary>
/// レールに沿って移動するオブジェクト。
/// プレイヤーとレールカメラのWorldTransformの親に設定して使用する。 
/// </summary>
class RailMover : public Collider
{
public:

	/// <summary>
	/// 初期化。
	/// </summary>
	/// <param name="controlPoints">スプライン曲線の制御点のリスト</param>
	/// <param name="enemySpawnManager">敵出現管理クラス</param>
	/// <param name="isLoop">ループするかどうか</param>
	void Initialize(const std::vector<YKEngine::Vector3>& controlPoints, EnemySpawnManager* enemySpawnManager, bool isLoop);

	/// <summary>
	/// 更新。
	/// </summary>
	void Update();

	/// <summary>
	/// レールの描画。
	/// </summary>
	/// <param name="camera">描画に使用するカメラ</param>
	void DrawRail(YKEngine::Camera* camera);

	/// <summary>
	/// 衝突時の処理。
	/// </summary>
	/// <param name="other">衝突した相手のコライダー</param>
	void OnCollision([[maybe_unused]] Collider* other) override;

	YKEngine::WorldTransform* GetWorldTransform() { return &worldTransform_; }

	bool IsEnd() const { return moveCount_ >= pointsDrawing_.size(); }

	uint32_t GetNextEnemyWaveNumber() const { return nextEnemySpawnEventNumber_; }

	uint32_t GetNextSpeedWaveNumber() const { return nextSpeedEventNumber_; }

	uint32_t GetNextRotateWaveNumber() const { return nextRotateEventNumber_; }

	uint32_t GetNextRotateResetWaveNumber() const { return nextRotateResetEventNumber_; }

	const YKEngine::Vector3& GetForward() const { return forward_; }

private:

	/// <summary>
	/// スプライン曲線の生成。
	/// </summary>
	/// <param name="controlPoints">スプライン曲線の制御点のリスト</param>
	void CreateSplineCurve(const std::vector<YKEngine::Vector3>& controlPoints);

	/// <summary>
	/// 向きの初期化。
	/// </summary>
	void InitializeRotate();

	/// <summary>
	/// 回転の更新。
	/// </summary>
	void UpdateRotate();

	// スプライン曲線制御点(通過点)Add commentMore actions
	std::vector<YKEngine::Vector3> controlPoints_;

	// 線分で描画する用の頂点リスト
	std::vector<YKEngine::Vector3> pointsDrawing_;
	// 線分の数
	const uint32_t kSegmentCount_ = 100;

	YKEngine::Vector3 target_;
	YKEngine::Vector3 forward_;

	float speed_ = 0.1f;

	uint32_t difference_ = 5;
	uint32_t moveCount_ = 0;

	uint32_t nextEnemySpawnEventNumber_ = 1;
	EnemySpawnManager* enemySpawnManager_ = nullptr;

	uint32_t nextSpeedEventNumber_ = 1;

	uint32_t nextRotateEventNumber_ = 1;
	//回転イベント中
	bool isInRotateEvent_ = false;
	std::unique_ptr<YKEngine::SRTAnimator> srtAnimator_;

	//回転リセットイベント用
	uint32_t nextRotateResetEventNumber_ = 1;

	bool isDrawRail_ = false;
	bool isLoop_ = false;
};