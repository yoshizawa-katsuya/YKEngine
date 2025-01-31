#include "FollowCamera.h"
#include "Vector.h"
#include "Matrix.h"
#include "Input.h"
#include "Lerp.h"
#include "LockOn.h"

void FollowCamera::Initialize(Camera* camera) {
	camera_ = camera;
}

void FollowCamera::Reset() {

	//追従対象がいれば
	if (target_) {
	
		//追従座標・角度の初期化
		interTarget_ = target_->translation_;
		camera_->SetRotateY(target_->rotation_.y);

	}
	destinationAngleY_ = camera_->GetRotate().y;

	//追従対象からのオフセット
	Vector3 offset = OffsetCalculation();
	camera_->SetTranslate(interTarget_ + offset);

}

void FollowCamera::SetTarget(const WorldTransform* target) {

	target_ = target;
	Reset();

}

Vector3 FollowCamera::OffsetCalculation() {

	// 追従対象からカメラまでのオフセット
	Vector3 offset = {0.0f, 2.0f, -20.0f};

	Matrix4x4 rotationMatrix = MakeRotateYMatrix(camera_->GetRotate().y);

	// オフセットをカメラの回転に合わせて回転させる
	offset = TransformNormal(offset, rotationMatrix);

	return offset;

}

void FollowCamera::Update() {


	//ロックオン中
	if (lockOn_->ExistTarget()) {
		//カメラをロックオン対象に向ける

		//ロックオン座標
		Vector3 lockOnPosition = lockOn_->GetTargetPosition();
		//追従対象からロックオン対象へのベクトル
		Vector3 sub = lockOnPosition - target_->translation_;

		//Y軸周り角度
		destinationAngleY_ = std::atan2(sub.x, sub.z);


	} else {
		//スティック入力で角度を変更
		float rotationSpeed = 0.1f;

		destinationAngleY_ += Input::GetInstance()->GetRightStickX() * rotationSpeed;
	}

	//右スティック押し込みでリセット
	if (Input::GetInstance()->TriggerButton(XINPUT_GAMEPAD_RIGHT_THUMB)) {
		destinationAngleY_ = target_->rotation_.y;
	}
	

	const float rotateCompletionRate = 0.1f;
	camera_->SetRotateY(LeapShortAngle(camera_->GetRotate().y, destinationAngleY_, rotateCompletionRate));
	

	if (target_) {
		
		const float completionRate = 0.5f;

		//追従対象の補完
		interTarget_ = Lerp(interTarget_, target_->translation_, completionRate);

		
	}

	//追従対象からのオフセット
	Vector3 offset = OffsetCalculation();
	// 座標をコピーしてオフセット分ずらす
	camera_->SetTranslate(interTarget_ + offset);


}