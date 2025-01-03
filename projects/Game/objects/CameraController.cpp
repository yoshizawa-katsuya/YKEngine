#include "CameraController.h"
#include "Player.h"
#include "Lerp.h"

void CameraController::Initialize(Camera* camera, Player* target)
{
	camera_ = camera;
	camera_->SetRotateX(0.1f);

	target_ = target;
}

void CameraController::Update()
{
	// 追従対象とオフセットからカメラの目標座標を計算
	targetPosition_ = targetOffset_;
	// 追従対象のワールドトランスフォームを参照
	targetPosition_.x += target_->GetCenterPosition().x;
	
	
	camera_->SetTranslate(Lerp(camera_->GetTranslate(), targetPosition_, kInterpolationRate));

	//移動範囲制限
	if (camera_->GetTranslate().x < movableArea_.left) {
		camera_->SetTranslateX(movableArea_.left);
	}
	else if (camera_->GetTranslate().x > movableArea_.right) {
		camera_->SetTranslateX(movableArea_.right);
	}


}
