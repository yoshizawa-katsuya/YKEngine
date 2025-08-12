#pragma once
#include "Collider.h"
class Camera;

class RailMover : public Collider
{
public:

	void Initialize(const std::vector<Vector3>& controlPoints);

	void Update();

	//レール描画
	void DrawRail(Camera* camera);

	WorldTransform* GetWorldTransform() { return &worldTransform_; }

	bool IsEnd() const { return moveCount_ >= pointsDrawing_.size(); }

private:

	//曲線の作成
	void CreateSplineCurve(const std::vector<Vector3>& controlPoints);

	//向きを更新
	void UpdateRotate();

	//オフセットを更新
	void UpdateOffset();

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

};

