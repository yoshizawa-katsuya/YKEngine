#include "BaseCharacter.h"
#include "cassert"

void BaseCharacter::Initialize(const std::vector<Model*>& models, ViewProjection* viewProjection) {

	Collider::Initialize();

	// NULLポインタチェック
	for (Model* model : models) {
		assert(model);
	}

	models_ = models;
	worldTransform_.Initialize();
	viewProjection_ = viewProjection;

}

void BaseCharacter::Update() {

	worldTransform_.UpdateMatrix();

}

void BaseCharacter::Draw() {

	//モデル描画
	for (Model* model : models_) {
	
		model->Draw(worldTransform_, *viewProjection_);
	
	}

}

Vector3 BaseCharacter::GetCenterPosition() const { 
	
	//ワールド座標を入れる変数
	Vector3 worldPos;
	//ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;

}
