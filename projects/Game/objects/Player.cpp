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

	/// <summary>
	/// 体のパーツの座標設定
	/// </summary>

	// 体及び全体
	worldTransforms_[0].scale_ = { 0.8f,0.8f,0.8f };
	worldTransforms_[0].rotation_.y = -1.56f;
	worldTransforms_[0].translation_.y = -0.2f;

	// 頭部
	worldTransforms_[1].translation_.y = 0.7f;

	// 右腕
	worldTransforms_[2].rotation_.z = 1.58f;
	worldTransforms_[2].translation_.y = 0.33f;
	worldTransforms_[2].translation_.z = -0.68f;

	// 左腕
	worldTransforms_[3].translation_.y = 0.34f;
	worldTransforms_[3].translation_.z = 0.68f;

	// 右足
	worldTransforms_[4].translation_.y = -0.4f;
	worldTransforms_[4].translation_.z = -0.18f;

	// 左足
	worldTransforms_[5].translation_.y = -0.4f;
	worldTransforms_[5].translation_.z = 0.22f;

	// ブースター
	worldTransforms_[6].translation_.x = -0.37f;
	worldTransforms_[6].translation_.y = 0.25f;

	// ジェット
	worldTransforms_[7].translation_.x = -0.1f;
	worldTransforms_[7].translation_.y = -0.05f;

	// 銃
	worldTransforms_[8].translation_.y = -1.72f;
	worldTransforms_[8].rotation_.z = -1.59f;

	// ミサイルランチャー
	worldTransforms_[9].translation_.y = 0.81f;
	worldTransforms_[9].translation_.z = -0.54f;

	// ペアレント設定
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