#include "RailCamera.h"
#include "Matrix.h"
#include "ImGuiManager.h"
#include "imgui/imgui.h"
#include "Curve.h"
#include "TransformHelpers.h"
#include "ModelPlatform.h"

void RailCamera::Initialize(Camera* camera, WorldTransform* parent) {

	//ワールドトランスフォームの初期設定
	worldTransform_.Initialize();
	worldTransform_.translation_.z = -13.0f; // Z軸方向に-13.0fの位置にカメラを配置
	worldTransform_.parent_ = parent; // 親ワールドトランスフォームを設定

	camera_ = camera;

	worldTransform_.UpdateMatrix();

	UpdateCamera();
}

void RailCamera::Update() {

	worldTransform_.UpdateMatrix();

	UpdateCamera();

#ifdef _DEBUG

	//カメラの座標を画面表示する処理
	ImGui::Begin("Camera");
	ImGui::SliderFloat3("translate", &worldTransform_.translation_.x, -20.0f, 20.0f);
	ImGui::SliderFloat3("rotation", &worldTransform_.rotation_.x, -20.0f, 20.0f);
	ImGui::End();

#endif // _DEBUG	
}

void RailCamera::UpdateCamera()
{
	camera_->SetTranslate(worldTransform_.GetWorldPosition());
	camera_->SetRotate(worldTransform_.parent_->rotation_);
	camera_->Update();
}
