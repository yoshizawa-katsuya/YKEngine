#pragma once
#include "WorldTransform.h"
#include "Camera.h"

//レールカメラ
class RailCamera {
public:

	//初期化
	void Initialize(const Vector3 worldPos, const Vector3 rotate, Camera* camera);

	//更新
	void Update();

	//描画
	void Draw(Camera* camera);

	//曲線の作成
	void CreateSplineCurve(const std::vector<Vector3>& controlPoints);

	Camera* GetCamera() { return camera_; }

	WorldTransform* GetWorldTransform() { return &worldTransform_; }

private:
	
	//ワールド変換データ
	WorldTransform worldTransform_;

	//ビュープロジェクション
	Camera* camera_;

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

	uint32_t difference_ = 5;
	uint32_t moveCount_ = 0;

};