#include "CameraManager.h"
#include "ModelPlatform.h"
#include "Camera.h"

#ifdef USE_IMGUI
#include "imgui/imgui.h"
#endif // USE_IMGUI

using namespace YKEngine;

void CameraManager::Initialize(WorldTransform* railMoverWorldTransform, WorldTransform* playerWorldTransform)
{
	//デバッグカメラの生成
	debugCamera_ = std::make_unique<DebugCamera>();
	debugCamera_->Initialize();

	// レールカメラの生成
	railCamera_ = std::make_unique<RailCamera>();
	// レールカメラの初期化
	railCamera_->Initialize(railMoverWorldTransform, playerWorldTransform);

	//メインカメラの設定
	mainCamera_ = GetRailCameraInner();

	//モデルプラットフォームにカメラを設定
	modelPlatform_ = ModelPlatform::GetInstance();
	modelPlatform_->SetCamera(mainCamera_);

}

void CameraManager::InitializeForClearScene(WorldTransform* railMoverWorldTransform)
{
	Initialize(railMoverWorldTransform, railMoverWorldTransform);

	railCamera_->SetClearScene();
}

void CameraManager::InitializeBeforeGameStart(YKEngine::WorldTransform* railMoverWorldTransform, YKEngine::WorldTransform* playerWorldTransform)
{
	Initialize(railMoverWorldTransform, playerWorldTransform);

	railCamera_->SetStart();
}

void CameraManager::Update()
{

#ifdef USE_IMGUI

	ImGui::Begin("CameraManager");
	if (ImGui::TreeNode("camera"))
	{
		ImGui::DragFloat3("translate", &GetRailCameraInner()->GetTranslate().x, 0.01f);
		ImGui::DragFloat3("rotate", &GetRailCameraInner()->GetRotate().x, 0.01f);

		ImGui::TreePop();
	}

	//メインカメラの切り替え
	if (ImGui::RadioButton("gameCamera", !isActiveDebugCamera_))
	{
		isActiveDebugCamera_ = false;

		mainCamera_ = GetRailCameraInner();
		modelPlatform_->SetCamera(mainCamera_);

	}
	if (ImGui::RadioButton("DebugCamera", isActiveDebugCamera_))
	{
		isActiveDebugCamera_ = true;

		mainCamera_ = debugCamera_->GetCamera();
		modelPlatform_->SetCamera(mainCamera_);

	}

	ImGui::End();

#endif // USE_IMGUI

	if (isActiveDebugCamera_)
	{
		debugCamera_->Update();
	}
}

void CameraManager::UpdateRailCamera()
{
	railCamera_->Update();
}

void CameraManager::ProcessGameOver()
{
	railCamera_->SetGameOver();
}
