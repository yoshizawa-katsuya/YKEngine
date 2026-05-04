#include "BaseCharacter.h"

using namespace YKEngine;

void BaseCharacter::Initialize(BaseModel* model)
{
	// NULLポインタチェック
	assert(model);

	SphereCollider::Initialize();

	object_ = std::make_unique<My3dObject>();
	object_->Initialize(model);

	characterWorldTransform_.Initialize();
	characterWorldTransform_.parent_ = &worldTransform_;
}

void BaseCharacter::Update()
{
	BaseCollider::Update();

	characterWorldTransform_.UpdateMatrix();
	object_->WorldTransformUpdate(characterWorldTransform_);
}

void BaseCharacter::Draw(Camera* camera)
{
	object_->CameraUpdate(camera);
	object_->Draw();
}
