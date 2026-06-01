#include "SkinCharacter.h"

using namespace YKEngine;

void SkinCharacter::Initialize(BaseModel* model, Animation* animation)
{
	// NULLポインタチェック
	assert(model);

	SphereCollider::Initialize();

	object_ = std::make_unique<Skin3dObject>();
	object_->Initialize(model);

	characterWorldTransform_.Initialize();
	characterWorldTransform_.parent_ = &worldTransform_;

	animation_ = animation;
}

void SkinCharacter::Update()
{
	SphereCollider::Update();

	characterWorldTransform_.UpdateMatrix();
	object_->WorldTransformUpdate(characterWorldTransform_);
}

void SkinCharacter::Draw(Camera * camera)
{
	object_->CameraUpdate(camera);
	object_->Draw();
}
