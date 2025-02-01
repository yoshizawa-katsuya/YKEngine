#include "Hammer.h"
#include "cassert"
#include "Matrix.h"
#include "CollisionTypeIdDef.h"
#include "Enemy.h"

void Hammer::Initialize(RigidModel* model, RigidModel* colliderModel) {

	Collider::Initialize(colliderModel);
	Collider::SetRadius(3.0f);
	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeIdDef::kPlayerWeapon));

	assert(model);

	object_ = std::make_unique<Rigid3dObject>();
	object_->Initialize(model);
	worldTransform_.Initialize();

}

void Hammer::UpdateWorldTransform() {

	worldTransform_.UpdateMatrix();
	object_->WorldTransformUpdate(worldTransform_);
}

void Hammer::Draw(Camera* camera) {

	object_->CameraUpdate(camera);
	object_->Draw();

}

void Hammer::OnCollision([[maybe_unused]] Collider* other) {

	//衝突相手の種別IDを取得
	uint32_t typeID = other->GetTypeID();
	//衝突相手が敵なら
	if (typeID == static_cast<uint32_t>(CollisionTypeIdDef::kEnemy)) {
	
		Enemy* enemy = static_cast<Enemy*>(other);
		uint32_t serialNumber = enemy->GetSerialNumber();

		//接触記録があれば何もせず抜ける
		if (hittengRecord_.HistoryCheck(serialNumber)) {
			return;
		}

		//接触記録に登録
		hittengRecord_.HistoryAdd(serialNumber);

		//敵の位置にエフェクトを発生
		enemy->Hit();
	}

}

void Hammer::HittingRecordClear() {

	hittengRecord_.Clear();

}

Vector3 Hammer::GetCenterPosition() const { 

	// ローカル座標でのオフセット
	const Vector3 offset = {0.0f, 10.5f, 0.0f};
	// ワールド座標に変換
	Vector3 worldPos = Transform(offset, worldTransform_.worldMatrix_);
	return worldPos;

}
