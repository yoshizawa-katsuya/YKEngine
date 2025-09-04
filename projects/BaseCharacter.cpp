#include "BaseCharacter.h"

void BaseCharacter::Initialize(BaseModel* model)
{
	object_ = std::make_unique<Rigid3dObject>();
	object_->Initialize(model);

	worldTransform_.Initialize();
}

void BaseCharacter::Update()
{
	worldTransform_.UpdateMatrix();
	object_->WorldTransformUpdate(worldTransform_);
}

void BaseCharacter::Draw(Camera* camera)
{
	object_->CameraUpdate(camera);
	object_->Draw();
}
