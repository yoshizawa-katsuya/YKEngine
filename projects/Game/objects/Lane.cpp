#include "Lane.h"
#include "ModelPlatform.h"

using namespace YKEngine;

void Lane::Initialize(bool* isStart)
{
	isStart_ = isStart;

	worldTransform_.Initialize();

	//===== モデルの生成 =====
	ModelPlatform* modelPlatform = ModelPlatform::GetInstance();
	object_ = std::make_unique<My3dObject>();
	object_->Initialize(modelPlatform->CreateRigidModel("./resources/lane", "lane.obj").get());
}

void Lane::Update()
{
	worldTransform_.UpdateMatrix();
	object_->WorldTransformUpdate(worldTransform_);
	for (const std::unique_ptr<Wall>& wall : walls_)
	{
		wall->Update();
	}
}

void Lane::Draw(YKEngine::Camera* camera)
{
	object_->CameraUpdate(camera);
	object_->Draw();
	//壁の描画
	for (const std::unique_ptr<Wall>& wall : walls_)
	{
		wall->Draw(camera);
	}
}

void Lane::AddWall(const YKEngine::WallData& wallData)
{
	//壁の生成
	std::unique_ptr<Wall> wall = std::make_unique<Wall>();
	wall->Initialize(wallData, isStart_, &worldTransform_);

	walls_.push_back(std::move(wall));
}
