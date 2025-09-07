#include "ElectricRange.h"
#include "Input.h"
#include "imgui/imgui.h"

void ElectricRange::Initialize(BaseModel* model, const Vector3& position)
{
    BaseCharacter::Initialize(model, position);
    scale_ = { 0.7f,0.7f,0.7f };
}

void ElectricRange::Update()
{
    //プレイヤーの位置に追従
    electricPosition = *targetPosition_ + offset;
    worldTransform_.translation_ = electricPosition;

    //拡大縮小
    if (Input::GetInstance()->PushKey(DIK_E)) {
        scale_ += Vector3{ step_, step_, step_ } *(1.0f / 60.0f);
    }
    if (Input::GetInstance()->PushKey(DIK_Q)) {
        scale_ -= Vector3{ step_, step_, step_ } *(1.0f / 60.0f);
    }
    ClampScale();

    worldTransform_.scale_ = scale_;

    //行列更新
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
