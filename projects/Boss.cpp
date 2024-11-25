#include "Boss.h"
#include "imgui/imgui.h"
#include "PlayerLockOn.h"
#include "Vector.h"
#include "Rigid3dObject.h"

void Boss::Initialize(BaseModel* model)
{

	object_ = std::make_unique<Rigid3dObject>();
	object_->Initialize(model);

	worldTransform_.Initialize();

	worldTransform_.translation_ = { 0.0f,0.0f,65.0f };
	worldTransform_.rotation_ = { 0.0f,1.5f,0.0f };

	worldTransform_.UpdateMatrix();

}

void Boss::Update()
{

	// ロックオン
	if (lockOn_ && lockOn_->ExistTarget()) {
		// ロックオン座標
		Vector3 lockOnPosition = lockOn_->GetTargetPosition();
		// 追従対象からロックオン対象へのベクトル
		Vector3 sub = lockOnPosition - worldTransform_.translation_;

		// Y軸周り角度
		worldTransform_.rotation_.y = std::atan2(sub.x, sub.z);
	}

	worldTransform_.UpdateMatrix();

#ifdef _DEBUG

	ImGui::Begin("Boss");
	if (ImGui::TreeNode("Model1")) {
		ImGui::ColorEdit4("color", &object_->GetModel().GetMaterialDataAddress().color.x);
		ImGui::DragFloat3("translate", &worldTransform_.translation_.x, 0.01f);
		ImGui::DragFloat3("rotate", &worldTransform_.rotation_.x, 0.01f);
		ImGui::DragFloat3("scale", &worldTransform_.scale_.x, 0.01f);

		ImGui::TreePop();
	}
	ImGui::End();

#endif // _DEBUG	

}

void Boss::Draw(Camera* camera)
{


	object_->Update(worldTransform_, camera);
	object_->Draw();

}
