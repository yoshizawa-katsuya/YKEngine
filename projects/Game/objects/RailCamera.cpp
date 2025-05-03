#include "RailCamera.h"
#include "Matrix.h"
#include "ImGuiManager.h"
#include "imgui/imgui.h"

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

	worldTransform_.translation_.z += 0.1f;
	//worldTransform_.rotation_.y += 0.1f;
	worldTransform_.UpdateMatrix();

	//カメラオブジェクトのワールド行列からビュー行列を計算する
	camera_->SetTranslate(worldTransform_.translation_);
	camera_->Update();

#ifdef _DEBUG

	//カメラの座標を画面表示する処理
	ImGui::Begin("Camera");
	ImGui::SliderFloat3("translate", &worldTransform_.translation_.x, -20.0f, 20.0f);
	ImGui::SliderFloat3("rotation", &worldTransform_.rotation_.x, -20.0f, 20.0f);
	ImGui::End();

#endif // _DEBUG	
}