#include "RailCamera.h"
#include "Matrix.h"
#include "Curve.h"
#include "TransformHelpers.h"
#include "ModelPlatform.h"
#include "Slerp.h"
#include "Camera.h"
#include "GlobalVariables.h"
#include "JsonKeys.h"
#include "RailCameraMainState.h"
#include "RailCameraStartState.h"
#include "RailCameraClearSceneState.h"
#include "Easing.h"

#ifdef USE_IMGUI
#include "imgui/imgui.h"
#endif // USE_IMGUI

using namespace YKEngine;

void RailCamera::Initialize(WorldTransform* parent, WorldTransform* playerWorldTransform)
{
	//jsonファイルに変数を登録
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const std::string& groupName = JsonKey::RailCamera::kGroupName;
	globalVariables->CreateGroup(groupName);
	globalVariables->AddItem(groupName, JsonKey::RailCamera::kClearLerpFactor, 0.9f);
	globalVariables->AddItem(groupName, JsonKey::RailCamera::kClearLerpTranslateFactor, 0.1f);
	globalVariables->AddItem(groupName, JsonKey::RailCamera::kClearLerpRotateFactor, 0.03f);
	globalVariables->AddItem(groupName, JsonKey::RailCamera::kGameOverIncrease, 0.03f);
	globalVariables->AddItem(groupName, JsonKey::RailCamera::kStartTIncrease, 1.0f / 60.0f);
	globalVariables->AddItem(groupName, JsonKey::RailCamera::kStartOfset, Vector3{ 0.0f, 100.0f, 0.0f });

	//ワールドトランスフォームの初期設定
	worldTransform_.Initialize();
	worldTransform_.translation_.z = -13.0f;
	worldTransform_.parent_ = parent; // 親ワールドトランスフォームを設定

	targetParentWorldTransform_.Initialize();
	targetParentWorldTransform_.translation_.z = 8.0f;
	targetParentWorldTransform_.parent_ = playerWorldTransform;

	worldTransform_.UpdateMatrix();

	//カメラの初期設定
	camera_ = std::make_unique<Camera>();
	camera_->SetTranslate(worldTransform_.GetWorldPosition());
	camera_->SetRotate(worldTransform_.parent_->rotation_);
	camera_->Update();

	stateMachine_ = std::make_unique<StateMachine<RailCameraStateContext>>();
	stateMachine_->Start(this);
	stateMachine_->ChangeState<RailCameraMainState>();
}

void RailCamera::Update() {

	worldTransform_.UpdateMatrix();

	stateMachine_->Update();

#ifdef USE_IMGUI

	//カメラの座標を画面表示する処理
	ImGui::Begin("Camera");
	ImGui::SliderFloat3("translate", &worldTransform_.translation_.x, -20.0f, 20.0f);
	ImGui::SliderFloat3("rotation", &worldTransform_.rotation_.x, -20.0f, 20.0f);
	ImGui::End();

#endif // USE_IMGUI	
}

void RailCamera::SetClearScene()
{
	stateMachine_->ChangeState<RailCameraClearSceneState>();
}

void RailCamera::SetStart()
{
	stateMachine_->ChangeState<RailCameraStartState>();
}

void RailCamera::CreateTargetRotationFromDirection(const Vector3& direction)
{
	targetRotation_ = TransformHelpers::FaceToVelocityDirection(worldTransform_.rotation_, direction);
}

void RailCamera::UpdateStart()
{
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();

	t_ += globalVariables->GetFloatValue(JsonKey::RailCamera::kGroupName, JsonKey::RailCamera::kStartTIncrease);
	if (t_ > 1.0f)
	{
		t_ = 1.0f;
	}

	// 親の位置から上にオフセットした位置をスタート位置とする
	Vector3 ofset = globalVariables->GetVector3Value(JsonKey::RailCamera::kGroupName, JsonKey::RailCamera::kStartOfset);
	ofset = TransformNormal(ofset, worldTransform_.parent_->worldMatrix_); // オフセットを親の回転に合わせて変換

	Vector3 startPosition = worldTransform_.GetWorldPosition() + ofset;

	//カメラの更新
	camera_->SetTranslate(Lerp(startPosition, worldTransform_.GetWorldPosition(), EaseOutCubic(t_)));

	Vector3 direction = worldTransform_.parent_->GetWorldPosition() - camera_->GetTranslate(); // カメラから親の位置へのベクトルを計算
	Vector3 targetRotate = TransformHelpers::FaceToVelocityDirection(worldTransform_.rotation_, direction); // カメラの回転をベクトルの方向に向ける

	camera_->SetRotate(targetRotate);
	camera_->Update();
}

void RailCamera::UpdateMain()
{
	//カメラの更新
	camera_->SetTranslate(worldTransform_.GetWorldPosition());
	camera_->SetRotate(worldTransform_.parent_->rotation_);
	camera_->Update();
}

void RailCamera::UpdateGameOver()
{
	targetParentWorldTransform_.UpdateMatrix();

	// t_が1.0f以上になったら処理を終了する
	if (t_ >= 1.0f)
	{
		return;
	}
	// t_を0.0fから1.0fまで徐々に増加させる
	t_ += GlobalVariables::GetInstance()->GetFloatValue(JsonKey::RailCamera::kGroupName, JsonKey::RailCamera::kGameOverIncrease);
	if (t_ > 1.0f)
	{
		t_ = 1.0f;
	}

	//カメラの更新
	// カメラの位置をプレイヤーの位置に徐々に近づける
	camera_->SetTranslate(SlerpTranslateByCenterAxis(targetParentWorldTransform_.parent_->GetWorldPosition(), Vector3{ 0.0f, 1.0f, 0.0f }, worldTransform_.GetWorldPosition(), targetParentWorldTransform_.GetWorldPosition(), t_));
	// プレイヤーの正面を向くようにカメラの回転を調整する
	camera_->SetRotate(LerpAngle(worldTransform_.parent_->rotation_, targetRotation_, t_));
	camera_->Update();

}

void RailCamera::UpdateClearScene()
{
	targetParentWorldTransform_.UpdateMatrix();

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const std::string& groupName = JsonKey::RailCamera::kGroupName;
	const float lerpFactor = globalVariables->GetFloatValue(groupName, JsonKey::RailCamera::kClearLerpFactor);
	const float lerpTranslateFactor = globalVariables->GetFloatValue(groupName, JsonKey::RailCamera::kClearLerpTranslateFactor);
	const float lerpRotateFactor = globalVariables->GetFloatValue(groupName, JsonKey::RailCamera::kClearLerpRotateFactor);

	//カメラの更新
	Vector3 targetTranslate = SlerpTranslateByCenterAxis(targetParentWorldTransform_.parent_->GetWorldPosition(), Vector3{ 0.0f, 1.0f, 0.0f }, worldTransform_.GetWorldPosition(), targetParentWorldTransform_.GetWorldPosition(), -1.0f, lerpFactor);
	camera_->SetTranslate(Lerp(camera_->GetTranslate(), targetTranslate, lerpTranslateFactor));
	// プレイヤーの正面を向くようにカメラの回転を調整する
	targetRotation_ = LerpAngle(worldTransform_.parent_->rotation_, targetRotation_, lerpFactor);
	camera_->SetRotate(LerpAngle(camera_->GetRotate(), targetRotation_, lerpRotateFactor));
	camera_->Update();
}

void RailCamera::EnterClearScene()
{
	targetParentWorldTransform_.UpdateMatrix();

	const float lerpFactor = GlobalVariables::GetInstance()->GetFloatValue(JsonKey::RailCamera::kGroupName, JsonKey::RailCamera::kClearLerpFactor);

	//カメラの更新
	camera_->SetTranslate(SlerpTranslateByCenterAxis(targetParentWorldTransform_.parent_->GetWorldPosition(), Vector3{ 0.0f, 1.0f, 0.0f }, worldTransform_.GetWorldPosition(), targetParentWorldTransform_.GetWorldPosition(), -1.0f, lerpFactor));
	// プレイヤーの正面を向くようにカメラの回転を調整する
	camera_->SetRotate(LerpAngle(worldTransform_.parent_->rotation_, targetRotation_, lerpFactor));
	camera_->Update();
}
