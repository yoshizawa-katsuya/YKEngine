#include "Collider.h"

void Collider::Initialize() {

	worldTransform_.Initialize();
	worldTransform_.scale_ *= radius_;
}

void Collider::UpdateWorldTransform() {

	//ワールド座標をワールドトランスフォームに適用
	worldTransform_.translation_ = GetCenterPosition();
	worldTransform_.scale_ = {radius_, radius_, radius_};
	worldTransform_.UpdateMatrix();

}

void Collider::Draw(Model* model, const ViewProjection& viewProjection) {

	//モデルの描画
	model->Draw(worldTransform_, viewProjection);

}
