#include "BaseCharacter.h"
#include "cassert"

void BaseCharacter::Initialize(const std::vector<RigidModel*>& models, RigidModel* colliderModel) {

	Collider::Initialize(colliderModel);

	// NULLポインタチェック
	for (RigidModel* model : models) {
		assert(model);
	}

	for (RigidModel* model : models) {
		Rigid3dObject* newObject = new Rigid3dObject();
		newObject->Initialize(model);
		objects_.emplace_back();
		std::unique_ptr<Rigid3dObject>& object = objects_.back();
		object.reset(newObject);
	}
	worldTransform_.Initialize();

}

void BaseCharacter::Update() {

	worldTransform_.UpdateMatrix();
	for (std::unique_ptr<Rigid3dObject>& object : objects_) {
		object->WorldTransformUpdate(worldTransform_);
	}
}

void BaseCharacter::Draw(Camera* camera) {

	//モデル描画
	for (std::unique_ptr<Rigid3dObject>& object : objects_) {
		object->CameraUpdate(camera);
		object->Draw();
	
	}

}

Vector3 BaseCharacter::GetCenterPosition() const { 
	
	//ワールド座標を入れる変数
	Vector3 worldPos;
	//ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform_.worldMatrix_.m[3][0];
	worldPos.y = worldTransform_.worldMatrix_.m[3][1];
	worldPos.z = worldTransform_.worldMatrix_.m[3][2];

	return worldPos;

}
