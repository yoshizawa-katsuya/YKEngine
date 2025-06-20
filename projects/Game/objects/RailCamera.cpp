#include "RailCamera.h"
#include "Matrix.h"
#include "ImGuiManager.h"
#include "imgui/imgui.h"
#include "Curve.h"
#include "TransformHelpers.h"
#include "ModelPlatform.h"

void RailCamera::Initialize(const Vector3 worldPos, const Vector3 rotate, Camera* camera) {

	//ワールドトランスフォームの初期設定
	worldTransform_.Initialize();
	worldTransform_.translation_ = worldPos;
	worldTransform_.rotation_ = rotate;
	
	camera_ = camera;
	camera_->SetTranslate(worldPos);
	camera_->SetRotate(rotate);
}

void RailCamera::Update() {

	if (pointsDrawing_.size() > moveCount_) {
		// カメラの位置を更新する処理
		eye_ = pointsDrawing_[moveCount_];
		worldTransform_.translation_ = eye_;

		if (pointsDrawing_.size() > moveCount_ + difference_) {
			target_ = pointsDrawing_[moveCount_ + difference_];
			forward_ = Subtract(target_, eye_);

			worldTransform_.rotation_ = TransformHelpers::FaceToVelocityDirection(worldTransform_.rotation_, forward_);
			/*if (worldTransform_.rotation_.y < 0.0f) {
				worldTransform_.rotation_.y = -worldTransform_.rotation_.y;
			}*/
			
		}

		// worldTransform_.translation_.z += 0.1f;
		// worldTransform_.rotation_.y += 0.1f;
		worldTransform_.UpdateMatrix();

		offset_ = { 0.0f, 1.0f, 0.0f };
		offset_ = TransformNormal(offset_, worldTransform_.worldMatrix_);
		worldTransform_.translation_ += offset_;

		moveCount_++;
	}

	worldTransform_.UpdateMatrix();

	//カメラオブジェクトのワールド行列からビュー行列を計算する
	camera_->SetTranslate(worldTransform_.translation_);
	camera_->SetRotate(worldTransform_.rotation_);
	camera_->Update();

#ifdef _DEBUG

	//カメラの座標を画面表示する処理
	ImGui::Begin("Camera");
	ImGui::SliderFloat3("translate", &worldTransform_.translation_.x, -20.0f, 20.0f);
	ImGui::SliderFloat3("rotation", &worldTransform_.rotation_.x, -20.0f, 20.0f);
	ImGui::End();

#endif // _DEBUG	
}

void RailCamera::Draw(Camera* camera) 
{
	//線の描画
	for (size_t i = 0; i < pointsDrawing_.size() - 1; i++) {
		Matrix4x4 point1 = MakeTranslateMatrix(pointsDrawing_[i]);
		Matrix4x4 point2 = MakeTranslateMatrix(pointsDrawing_[i + 1]);
		ModelPlatform::GetInstance()->LineDraw(point1, point2, camera);
	}
}

void RailCamera::CreateSplineCurve(const std::vector<Vector3>& controlPoints)
{
	// レベルデータから制御点を取得
	for (Vector3 controlPoint : controlPoints) {
		controlPoints_.push_back(controlPoint);
	}
	// Catmull-Romスプラインのポイントを生成
	pointsDrawing_ = GenerateCatmullRomSplinePoints(controlPoints_, segmentCount_);
}
