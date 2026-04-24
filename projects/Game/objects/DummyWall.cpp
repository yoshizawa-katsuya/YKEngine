#include "DummyWall.h"
#ifdef USE_IMGUI
#include "imgui/imgui.h"
#endif // USE_IMGUI

using namespace YKEngine;

void DummyWall::Initialize(YKEngine::BaseModel* model) {
    object_ = std::make_unique<My3dObject>();
    object_->Initialize(model);

	worldTransform_.Initialize();

    worldTransform_.translation_.y = 1.5f;
	worldTransform_.translation_.z = 10.0f;

	state_ = { PlayerPose::A,PlayerDirection::Front };
}

void DummyWall::Update() {

#ifdef USE_IMGUI

    ImGui::Begin("DummyWall");
    ImGui::DragFloat3("Position", &worldTransform_.translation_.x, 0.01f);
    ImGui::Checkbox("Start Move", &isStart_);
    if (ImGui::Button("Reset Position"))
    {
        worldTransform_.translation_.z = 10.0f;
    }
    int pose = static_cast<int>(state_.pose);
    if (ImGui::Combo("Pose", &pose, "Base\0Squat\0A\0B\0C\0D\0"))
    {
        state_.pose = static_cast<PlayerPose>(pose);
    }
    int dir = static_cast<int>(state_.dir);
    if (ImGui::Combo("Direction", &dir, "Left\0Front\0Right\0"))
    {
        state_.dir = static_cast<PlayerDirection>(dir);
    }
    ImGui::End();

#endif // USE_IMGUI	

    // ===== 流す処理 =====
    if (isStart_)
    {
        worldTransform_.translation_.z -= 0.1f;
    }

    UpdateColorForDebug();
    worldTransform_.UpdateMatrix();
    object_->WorldTransformUpdate(worldTransform_);
}

void DummyWall::Draw(Camera* camera) {

    object_->CameraUpdate(camera);
    object_->Draw();

}

void DummyWall::UpdateColorForDebug()
{
    static const Vector4 kPoseColors[] =
    {
        {1,1,1,1}, // Base
        {0,0,0,1}, // Squat
        {1,0,0,1}, // A
        {0,1,0,1}, // B
        {0,0,1,1}, // C
        {1,1,0,1}, // D
    };

    object_->SetColor(kPoseColors[static_cast<int>(state_.pose)]);
}
