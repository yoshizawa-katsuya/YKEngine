#include "Collider.h"

void Collider::Initialize(RigidModel* model) {

	object_ = std::make_unique<Rigid3dObject>();
	object_->Initialize(model);

	worldTransform_.Initialize();
	worldTransform_.scale_ *= radius_;
}

void Collider::UpdateWorldTransform() {

	//ワールド座標をワールドトランスフォームに適用
	worldTransform_.translation_ = GetCenterPosition();
	worldTransform_.scale_ = {radius_, radius_, radius_};
	worldTransform_.UpdateMatrix();
	object_->WorldTransformUpdate(worldTransform_);

}

void Collider::Draw(Camera* camera) {

	//モデルの描画
	object_->CameraUpdate(camera);
	object_->Draw();

}
