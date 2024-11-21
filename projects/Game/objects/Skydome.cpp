#include "Skydome.h"
#include "Rigid3dObject.h"

void Skydome::Initialzie(BaseModel* model)
{
	object_ = std::make_unique<Rigid3dObject>();
	object_->Initialize(model);

	worldTransform_.Initialize();
	worldTransform_.scale_ = { 1.0f,1.0f,1.0f };
}

void Skydome::Update(Camera* camera)
{
	worldTransform_.UpdateMatrix();
	object_->Update(worldTransform_,camera);
}

void Skydome::Draw()
{
	object_->Draw();
}
