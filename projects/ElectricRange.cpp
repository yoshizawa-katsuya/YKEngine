#include "ElectricRange.h"
#include "Input.h"
#include "imgui/imgui.h"

void ElectricRange::Initialize(BaseModel* model, const Vector3& position)
{
    BaseCharacter::Initialize(model, position);
    scale_ = { 0.7f, 0.7f, 0.7f };
}

void ElectricRange::Update()
{
    //プレイヤーの位置に追従
    worldTransform_.translation_ = *targetPosition_;
    worldTransform_.scale_ = scale_;

    //拡大縮小操作
    if (Input::GetInstance()->PushKey(DIK_E)) {
        scale_ += Vector3{ step_, step_, step_ } *(1.0f / 60.0f);
    }
    if (Input::GetInstance()->PushKey(DIK_Q)) {
        scale_ -= Vector3{ step_, step_, step_ } *(1.0f / 60.0f);
    }
    ClampScale();
    worldTransform_.UpdateMatrix();
    object_->WorldTransformUpdate(worldTransform_);
}

void ElectricRange::Draw(Camera* camera)
{
    BaseCharacter::Draw(camera);
}
void ElectricRange::DebugImGui()
{
    if (ImGui::TreeNode("Electric Range"))
    {
        if (ImGui::ColorEdit4("Color", &color_.x))
        {
            SetColor(color_); 
        }
        ImGui::TreePop();
    }
}
AABB ElectricRange::GetAABB() {
    AABB aabb;
    Vector3 pos = GetWorldPosition();
    Vector3 scale = worldTransform_.scale_;

    float halfX = (kWidth_ * scale.x) / 2.0f;
    float halfY = (kWidth_ * scale.y) / 2.0f;
    float halfZ = (kWidth_ * scale.z) / 2.0f;

    aabb.min = { pos.x - halfX, pos.y - halfY, pos.z - halfZ };
    aabb.max = { pos.x + halfX, pos.y + halfY, pos.z + halfZ };

    return aabb;
}
void ElectricRange::SetColor(const Vector4& color)
{
    color_ = color;
    object_->GetModel().GetMaterialDataAddress().color = color_;
}
void ElectricRange::SetFollowTarget(const Vector3* targetPos)
{
    targetPosition_ = targetPos;
}

void ElectricRange::ClampScale()
{
    if (scale_.x > maxScale_) scale_ = { maxScale_, maxScale_, maxScale_ };
    if (scale_.x < minScale_) scale_ = { minScale_, minScale_, minScale_ };
}
