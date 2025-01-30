#include "FollowCamera.h"
#include "Vector.h"
#include "Matrix.h"
#include "Input.h"
#include "Lerp.h"
#include "LockOn.h"

void FollowCamera::Initialize() {
	viewProjection_.Initialize();
}

void FollowCamera::Reset() {

	//追従対象がいれば
	if (target_) {
	
		//追従座標・角度の初期化
		interTarget_ = target_->translation_;
		viewProjection_.rotation_.y = target_->rotation_.y;

	}
	destinationAngleY_ = viewProjection_.rotation_.y;

	//追従対象からのオフセット
	Vector3 offset = OffsetCalculation();
	viewProjection_.translation_ = interTarget_ + offset;

}

void FollowCamera::SetTarget(const WorldTransform* target) {

	target_ = target;
	Reset();

}

Vector3 FollowCamera::OffsetCalculation() {

	// 追従対象からカメラまでのオフセット
	Vector3 offset = {0.0f, 2.0f, -20.0f};

	Matrix4x4 rotationMatrix = MakeRotateYMatrix(viewProjection_.rotation_.y);

	// オフセットをカメラの回転に合わせて回転させる
	offset = TransformNormal(offset, rotationMatrix);

	return offset;

}

void FollowCamera::Update() {

	

	// ゲームパッドの状態を得る変数
	XINPUT_STATE joyState;

	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
	
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

			destinationAngleY_ += static_cast<float>(joyState.Gamepad.sThumbRX) / SHRT_MAX * rotationSpeed;
		}

		//右スティック押し込みでリセット
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB) {
			destinationAngleY_ = target_->rotation_.y;
		}
	}

	const float rotateCompletionRate = 0.1f;
	viewProjection_.rotation_.y = LeapShortAngle(viewProjection_.rotation_.y, destinationAngleY_, rotateCompletionRate);
	

	if (target_) {
		
		const float completionRate = 0.5f;

		//追従対象の補完
		interTarget_ = Leap(interTarget_, target_->translation_, completionRate);

		
	}

	//追従対象からのオフセット
	Vector3 offset = OffsetCalculation();
	// 座標をコピーしてオフセット分ずらす
	viewProjection_.translation_ = interTarget_ + offset;

	//ビュー行列の更新
	viewProjection_.UpdateViewMatrix();

}