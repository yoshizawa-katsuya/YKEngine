#include "BaseCharacter.h"

void BaseCharacter::Initialize(BaseModel* model)
{
	// NULLポインタチェック
	assert(model);

	Collider::Initialize();

	object_ = std::make_unique<Rigid3dObject>();
	object_->Initialize(model);

	characterWorldTransform_.Initialize();
	characterWorldTransform_.parent_ = &worldTransform_;
}

void BaseCharacter::Update()
{
	Collider::Update();

	characterWorldTransform_.UpdateMatrix();
	object_->WorldTransformUpdate(characterWorldTransform_);
}

void BaseCharacter::Draw(Camera* camera)
{
	object_->CameraUpdate(camera);
	object_->Draw();
}
