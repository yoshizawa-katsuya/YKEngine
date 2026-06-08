#include "Lane.h"
#include "ModelPlatform.h"

using namespace YKEngine;

void Lane::Initialize(bool* isStart, WallModels* wallModels)
{
	// 流れ始めるかどうかのフラグのポインタを受け取る
	isStart_ = isStart;

	// 壁のモデルを受け取る
	wallModels_ = wallModels;

	worldTransform_.Initialize();

	//===== モデルの生成 =====
	ModelPlatform* modelPlatform = ModelPlatform::GetInstance();
	object_ = std::make_unique<My3dObject>();
	object_->Initialize(modelPlatform->CreateRigidModel("./resources/lane2", "lane2.obj").get());
}

void Lane::Update()
{
	worldTransform_.UpdateMatrix();
	object_->WorldTransformUpdate(worldTransform_);

	//壁の削除
	walls_.erase(std::remove_if(walls_.begin(), walls_.end(),
		[](const std::unique_ptr<Wall>& wall) { return wall->GetIsDead(); }),
		walls_.end());

	//壁の更新
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
	wall->Initialize(wallData, isStart_, &worldTransform_, wallModels_);

	walls_.push_back(std::move(wall));
}
