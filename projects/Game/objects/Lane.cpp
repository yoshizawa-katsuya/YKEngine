#include "Lane.h"

using namespace YKEngine;

void Lane::Initialize(bool* isStart)
{
	isStart_ = isStart;

	worldTransform_.Initialize();

}

void Lane::Update()
{
	worldTransform_.UpdateMatrix();
	for (const std::unique_ptr<Wall>& wall : walls_)
	{
		wall->Update();
	}
}

void Lane::Draw(YKEngine::Camera* camera)
{
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
