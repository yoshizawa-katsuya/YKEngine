#include "Collider.h"
#include "Rigid3dObject.h"

void Collider::Initialize(BaseModel* model)
{
	object_ = std::make_unique<Rigid3dObject>();
	object_->Initialize(model);

	worldTransform_.Initialize();
}

void Collider::Update()
{
	worldTransform_.UpdateMatrix();
	object_->WorldTransformUpdate(worldTransform_);
}

void Collider::Draw(Camera* camera)
{
	object_->CameraUpdate(camera);
	object_->Draw();
}

void Collider::Draw(Camera* camera, uint32_t textureHandle)
{
	object_->CameraUpdate(camera);
	object_->Draw(textureHandle);
}

Vector3 Collider::GetCenterPosition() const
{
	//ワールド座標を入れる変数
	Vector3 worldPos;
	//ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform_.worldMatrix_.m[3][0];
	worldPos.y = worldTransform_.worldMatrix_.m[3][1];
	worldPos.z = worldTransform_.worldMatrix_.m[3][2];

	return worldPos;
}

Vector2 Collider::Get2DCenterPosition() const
{
	//ワールド座標を入れる変数
	Vector2 worldPos;
	//ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform_.worldMatrix_.m[3][0];
	worldPos.y = worldTransform_.worldMatrix_.m[3][1];

	return worldPos;
}
