#include "WallModels.h"
#include "ModelPlatform.h"

using namespace YKEngine;

void WallModels::Initialize()
{
	//壁のモデルの生成
	ModelPlatform* modelPlatform = ModelPlatform::GetInstance();
	wallModels_[PlayerPose::A] = modelPlatform->CreateRigidModel("./resources/wall", "APoseWall.obj");
	wallModels_[PlayerPose::B] = modelPlatform->CreateRigidModel("./resources/wall", "BPoseWall.obj");
	wallModels_[PlayerPose::C] = modelPlatform->CreateRigidModel("./resources/wall", "CPoseWall.obj");
	wallModels_[PlayerPose::D] = modelPlatform->CreateRigidModel("./resources/wall", "DPoseWall.obj");
	wallModels_[PlayerPose::Squat] = modelPlatform->CreateRigidModel("./resources/wall", "SquatWall.obj");
	wallModels_[PlayerPose::Base] = modelPlatform->CreateRigidModel("./resources/wall", "BaseWall.obj");
}
