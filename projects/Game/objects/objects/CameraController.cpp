#define NOMINMAX
#include "CameraController.h"
#include "Player.h"
#include "Lerp.h"
#include <algorithm>
#include "Camera.h"
#include "MapChipField.h"

void CameraController::Initialize(Camera* camera, Player* target, MapChipField* mapChipField) {

	camera_ = camera;
	target_ = target;
	transform_.translation = camera_->GetTranslate();
	transform_.rotation = { 0, 0, 0 };
	transform_.scale = { 1, 1, 1 };

	movableArea_.right = mapChipField->GetNumCellHorizontal() * 2.0f - 13.0f;
	movableArea_.top = mapChipField->GetNumCellVirtical() * 2.0f - 7.65f;

	if (movableArea_.right < movableArea_.left)
	{
		float center = (movableArea_.left + movableArea_.right) / 2.0f;
		movableArea_.left = center;
		movableArea_.right = center;
	}

	if (movableArea_.top < movableArea_.bottom)
	{
		float center = (movableArea_.top + movableArea_.bottom) / 2.0f;
		movableArea_.top = center;
		movableArea_.bottom = center;
	}

	Reset();
}

void CameraController::Update() {

	// 追従対象のワールドトランスフォームを参照
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
	// 追従対象とオフセットからカメラの目標座標を計算
	targetPosition_ = targetWorldTransform.translation_ + targetOffset_;
	
	//座標補完によりゆったり追従
	transform_.translation = Lerp(transform_.translation, targetPosition_, kInterpolationRate);

	//追従対象が画面外に出ないように補正
	transform_.translation.x = std::clamp(transform_.translation.x, targetWorldTransform.translation_.x + margin_.left, targetWorldTransform.translation_.x + margin_.right);
	transform_.translation.y = std::clamp(transform_.translation.y, targetWorldTransform.translation_.y + margin_.bottom, targetWorldTransform.translation_.y + margin_.top);
	
	ClampPosition();

	//行列を更新する
	camera_->SetTranslate(transform_.translation);
	camera_->Update();


}

void CameraController::Reset() {

	//追従対象のワールドトランスフォームを参照
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
	//追従対象とオフセットからカメラの座標を計算
	transform_.translation = targetWorldTransform.translation_;
	transform_.translation.z += targetOffset_.z;
	
	ClampPosition();

	camera_->SetTranslate(transform_.translation);
	camera_->Update();
}

void CameraController::ClampPosition()
{
	//移動範囲制限
	transform_.translation.x = std::clamp(transform_.translation.x, movableArea_.left, movableArea_.right);
	transform_.translation.y = std::clamp(transform_.translation.y, movableArea_.bottom, movableArea_.top);
}
