#include "Player.h"
#include "imgui/imgui.h"
#include "Rigid3dObject.h"
#include "Lerp.h"
#include "Easing.h"

Player::Player()
	: input_(Input::GetInstance())
	, kJumpAcceleration_(0.5f)
	, kLimitFallSpeed_(1.0f)
	, kGravityAcceleration_(0.03f)
	, speed_(0.1f)
	, velocity_({0.0f, 0.0f, 0.0f})
	, onGround_(false)
	, lrDirection_(LRDirection::kRight)
	, angleCompletionRate_(0.2f)
	, turnTimer_(1.0f)
	, isAttack_(false)
	, attackRange_({1.5f, 1.0f})
{
	radius_ = 0.5f;
}

Player::~Player()
{
}

void Player::Initialize(BaseModel* model) {

	Collider::Initialize(model);

	worldTransform_.rotation_.y = destinationRotationYTable[0];
}

void Player::Update() {

	Move();

	MoveAppli();

	GroundCollision();

	Attack();

	// 旋回制御
	if (turnTimer_ < 1.0f) {
		turnTimer_ += 1.0f / 12.0f;

		if (turnTimer_ > 1.0f) {
			turnTimer_ = 1.0f;
		}
		// 自キャラの角度を設定する
		//worldTransform_.rotation_.y = turnFirstRotationY_ * EaseOut(turnTimer_) + destinationRotationY * (1 - EaseOut(turnTimer_));
		worldTransform_.rotation_.y = Lerp(turnFirstRotationY_, targetAngle_, EaseOut(turnTimer_));

	}

#ifdef _DEBUG

	ImGui::Begin("Player");
	if (ImGui::TreeNode("Model")) {
		ImGui::ColorEdit4("color", &object_->GetModel().GetMaterialDataAddress().color.x);
		ImGui::DragFloat3("translate", &worldTransform_.translation_.x, 0.01f);
		ImGui::DragFloat3("rotate", &worldTransform_.rotation_.x, 0.01f);
		ImGui::DragFloat3("scale", &worldTransform_.scale_.x, 0.01f);

		ImGui::TreePop();
	}
	ImGui::End();


#endif // _DEBUG	

	Collider::Update();

}
/*
void Player::Draw(Camera* camera) {

	object_->CameraUpdate(camera);
	object_->Draw();

}
*/
void Player::Move()
{
	//左右移動
	if (input_->PushKey(DIK_A)) {
		worldTransform_.translation_.x -= speed_;
		if (lrDirection_ != LRDirection::kLeft) {
			lrDirection_ = LRDirection::kLeft;
			targetAngle_ = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];
			turnFirstRotationY_ = worldTransform_.rotation_.y;
			turnTimer_ = 0.0f;
		}
	}
	if (input_->PushKey(DIK_D)) {
		worldTransform_.translation_.x += speed_;
		if (lrDirection_ != LRDirection::kRight) {
			lrDirection_ = LRDirection::kRight;
			targetAngle_ = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];
			turnFirstRotationY_ = worldTransform_.rotation_.y;
			turnTimer_ = 0.0f;
		}
	}

	//ジャンプ
	if (onGround_ && input_->TriggerKey(DIK_W)) {
		// ジャンプ加速
		velocity_.y += kJumpAcceleration_;
		onGround_ = false;
	}
	else if (!onGround_) {

		// 落下速度
		velocity_.y += -kGravityAcceleration_;
		// 落下速度制限
		velocity_.y = (std::max)(velocity_.y, -kLimitFallSpeed_);
	}

}

void Player::MoveAppli()
{
	worldTransform_.translation_ += velocity_;
}

void Player::GroundCollision()
{
	if (onGround_) {
		return;
	}

	if (worldTransform_.translation_.y < radius_) {
		worldTransform_.translation_.y = radius_;
		velocity_.y = 0.0f;
		onGround_ = true;
	}

}

void Player::Attack()
{
	if (isAttack_) {
		return;
	}
	if (!input_->TriggerKey(DIK_SPACE)) {
		return;
	}
	isAttack_ = true;
}
