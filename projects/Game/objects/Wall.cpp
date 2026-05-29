#include "Wall.h"
#include "ModelPlatform.h"
#include "GlobalVariables.h"
#include "JsonKeys.h"
#include "WallModels.h"

using namespace YKEngine;

void Wall::Initialize(const WallData& wallData, bool* isStart, WorldTransform* parent, WallModels* wallModels)
{
	//GlovalVariablesから壁の移動速度を取得
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	speed_ = globalVariables->GetFloatValue(JsonKey::Wall::kGroupName, JsonKey::Wall::kSpeed);

	// 流れ始めるかどうかのフラグのポインタを受け取る
	isStart_ = isStart;

	// 壁のモデルを受け取る
	wallModels_ = wallModels;

    //初期状態を設定
    state_ = { wallData.pose, wallData.direction };
    feintPose_ = wallData.feintPose;

	//===== モデルの生成 =====
	ModelPlatform* modelPlatform = ModelPlatform::GetInstance();
    object_ = std::make_unique<My3dObject>();
    if (feintPose_.has_value())
    {
        //フェイント用のポーズがある場合はフェイント用のポーズのモデルを使用
        object_->Initialize(wallModels_->GetWallModel(feintPose_.value()).get());
    }
    else
    {
        //ない場合は通常のポーズのモデルを使用
        object_->Initialize(wallModels_->GetWallModel(state_.pose).get());
	}
    

	//初期位置の設定
    worldTransform_.Initialize();
	worldTransform_.parent_ = parent;
	worldTransform_.translation_.z = wallData.translate.z;

	
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
		object_->Initialize(wallModels_->GetWallModel(state_.pose).get()); //通常のポーズのモデルを使用
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
