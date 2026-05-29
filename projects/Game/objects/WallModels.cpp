#include "WallModels.h"
#include "ModelPlatform.h"

using namespace YKEngine;

void WallModels::Initialize()
{
	//壁のモデルの生成
	ModelPlatform* modelPlatform = ModelPlatform::GetInstance();
	wallModels_[PlayerPose::A] = modelPlatform->CreateRigidModel("./Resources/walls", "PoseAWall.obj");
	wallModels_[PlayerPose::B] = modelPlatform->CreateRigidModel("./Resources/walls", "PoseBWall.obj");
	wallModels_[PlayerPose::C] = modelPlatform->CreateRigidModel("./Resources/walls", "PoseCWall.obj");
	wallModels_[PlayerPose::D] = modelPlatform->CreateRigidModel("./Resources/walls", "PoseDWall.obj");
	wallModels_[PlayerPose::Squat] = modelPlatform->CreateRigidModel("./Resources/walls", "SquatWall.obj");
	wallModels_[PlayerPose::Base] = modelPlatform->CreateRigidModel("./Resources/wall", "wall.obj");
}
