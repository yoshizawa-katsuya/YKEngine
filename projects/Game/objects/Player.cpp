#include "Player.h"
#include "imgui/imgui.h"

void Player::Initialize(const std::vector<BaseModel*>& models) {

	models_ = models;
	
	// 各部位のWorldTransformを初期化
	worldTransforms_.resize(models.size());
	for (auto& transform : worldTransforms_) {
		transform.Initialize();
		transform.UpdateMatrix();
	}
	worldTransforms_[0].scale_ = { 0.8f,0.8f,0.8f };
	worldTransforms_[0].rotation_.y = -1.55f;
	worldTransforms_[0].translation_.y = -2.0f;
	worldTransforms_[1].parent_ = &worldTransforms_[0];
	worldTransforms_[2].parent_ = &worldTransforms_[0];
	worldTransforms_[3].parent_ = &worldTransforms_[0];
	worldTransforms_[4].parent_ = &worldTransforms_[0];
	worldTransforms_[5].parent_ = &worldTransforms_[0];
	worldTransforms_[6].parent_ = &worldTransforms_[0];
	worldTransforms_[7].parent_ = &worldTransforms_[6];
	worldTransforms_[8].parent_ = &worldTransforms_[2];
	worldTransforms_[9].parent_ = &worldTransforms_[0];
}

void Player::Update() {

	for (auto& transform : worldTransforms_) {
		transform.UpdateMatrix();
	}

#ifdef _DEBUG

	ImGui::Begin("Player");
	for (size_t i = 0; i < models_.size(); ++i) {
		if (ImGui::TreeNode(("Model " + std::to_string(i)).c_str())) {
			ImGui::ColorEdit4("color", &models_[i]->GetMaterialDataAddress().color.x);
			ImGui::DragFloat3("translate", &worldTransforms_[i].translation_.x, 0.01f);
			ImGui::DragFloat3("rotate", &worldTransforms_[i].rotation_.x, 0.01f);
			ImGui::DragFloat3("scale", &worldTransforms_[i].scale_.x, 0.01f);
			ImGui::TreePop();
		}
	}
	ImGui::End();

#endif // _DEBUG	

	
}

void Player::Draw(Camera* camera) {

	
	// 各部位のモデルを描画
	for (size_t i = 0; i < models_.size(); ++i) {
		models_[i]->Draw(worldTransforms_[i], camera);
	}
	
}