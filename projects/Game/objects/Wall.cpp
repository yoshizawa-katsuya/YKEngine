#include "Wall.h"
#include "ModelPlatform.h"
#include "GlobalVariables.h"
#include "JsonKeys.h"

using namespace YKEngine;

void Wall::Initialize(const WallData& wallData, bool* isStart, WorldTransform* parent)
{
	//GlovalVariablesから壁の移動速度を取得
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	speed_ = globalVariables->GetFloatValue(JsonKey::Wall::kGroupName, JsonKey::Wall::kSpeed);

	// 流れ始めるかどうかのフラグのポインタを受け取る
	isStart_ = isStart;

	//===== モデルの生成 =====
	ModelPlatform* modelPlatform = ModelPlatform::GetInstance();
    object_ = std::make_unique<My3dObject>();
    object_->Initialize(modelPlatform->CreateRigidModel("./resources/wall", "wall.obj").get());

	//初期位置の設定
    worldTransform_.Initialize();
	worldTransform_.parent_ = parent;
	worldTransform_.translation_.z = wallData.translate.z;

	//初期状態を設定
    state_ = { wallData.pose, wallData.direction };
	feintPose_ = wallData.feintPose;
}

void Wall::Update() 
{

    // ===== 流す処理 =====
    if (*isStart_)
    {
        worldTransform_.translation_.z -= speed_;
    }

    if (worldTransform_.translation_.z <= 5.0f)
    {
		feintPose_ = std::nullopt; //フェイント用のポーズをリセット
    }

    UpdateColorForDebug();
    worldTransform_.UpdateMatrix();
    object_->WorldTransformUpdate(worldTransform_);
}

void Wall::Draw(Camera* camera)
{

    object_->CameraUpdate(camera);
    object_->Draw();

}

void Wall::UpdateColorForDebug()
{
    static const Vector4 kPoseColors[] =
    {
        {1,1,1,1}, // Base
        {0,0,0,1}, // Squat
        {1,0,0,1}, // A
        {0,1,0,1}, // B
        {0,0,1,1}, // C
        {1,1,0,1}, // D
    };

    if (feintPose_.has_value())
    {
        //フェイント用のポーズがある場合はフェイント用のポーズの色を設定
        object_->SetColor(kPoseColors[static_cast<int>(feintPose_.value())]);
    }
    else
    {
        //ない場合は通常のポーズの色を設定
        object_->SetColor(kPoseColors[static_cast<int>(state_.pose)]);
    }

}