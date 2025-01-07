#include "Player.h"
#include "imgui/imgui.h"
#include "Rigid3dObject.h"
#include "Lerp.h"
#include "Easing.h"
#include "BaseEnemy.h"
#include "PlayerBullet.h"
#include "GameScene.h"

Player::Player()
	: input_(Input::GetInstance())
	, kJumpAcceleration_(0.5f)
	, kLimitFallSpeed_(1.0f)
	, kGravityAcceleration_(0.03f)
	, speed_(0.1f)
	, velocity_({0.0f, 0.0f, 0.0f})
	, onGround_(false)
	, lrDirection_(LRDirection::kRight)
	, turnTimer_(1.0f)
{
	radius_ = 0.5f;
}

Player::~Player()
{
}

void Player::Initialize(BaseModel* model) {

	Collider::Initialize(model);

	worldTransform_.rotation_.y = destinationRotationYTable[0];
	worldTransform_.translation_.y = radius_;
	worldTransform_.UpdateMatrix();
	object_->WorldTransformUpdate(worldTransform_);
}

void Player::Update() {

	Move();

	MoveAppli();

	GroundCollision();

	Attack();

	AttackUpdate();

	Fire();

	InvincibleTimeUpdate();

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

	HammerUpdate();

}

void Player::Draw(Camera* camera)
{
	//無敵時間のときは点滅する
	if (!(status_.remainingInvincibleTime_ % 2 == 1)) {
		Collider::Draw(camera);
	}

	if (workAttack_.attackParameter_ == 0) {
		return;
	}
	hammer_.object_->CameraUpdate(camera);
	hammer_.object_->Draw();
}

void Player::HUDInitialize(uint32_t textureHandleHeartFrame, uint32_t textureHandleHeart)
{
	hud_.spriteHeartFrame_.resize(status_.maxHP_);
	hud_.spriteHeart_.resize(status_.maxHP_);

	for (uint32_t i = 0; i < status_.maxHP_; i++) {
		hud_.spriteHeartFrame_[i] = std::make_unique<Sprite>();
		hud_.spriteHeartFrame_[i]->Initialize(textureHandleHeartFrame);
		hud_.spriteHeartFrame_[i]->SetPosition({ 60.0f + 80.0f * i, 600.0f });

		hud_.spriteHeart_[i] = std::make_unique<Sprite>();
		hud_.spriteHeart_[i]->Initialize(textureHandleHeart);
		hud_.spriteHeart_[i]->SetPosition({ 60.0f + 80.0f * i, 600.0f });

	}

}

void Player::HUDDraw()
{
	for (uint32_t i = 0; i < status_.maxHP_; i++) {
		hud_.spriteHeartFrame_[i]->Draw();
	}
	for (uint32_t i = 0; i < status_.HP_; i++) {
		hud_.spriteHeart_[i]->Draw();
	}
}

void Player::OnCollision()
{
	if (status_.remainingInvincibleTime_ != 0) {
		return;
	}
	if (!status_.isAlive_) {
		return;
	}

	status_.HP_--;
	if (status_.HP_ <= 0) {
		status_.isAlive_ = false;
	}
	status_.remainingInvincibleTime_ = status_.maxInvincibleTime_;

}

void Player::HammerInitialize(BaseModel* model)
{
	hammer_.object_ = std::make_unique<Rigid3dObject>();
	hammer_.object_->Initialize(model);

	hammer_.worldTransform_.Initialize();
	hammer_.worldTransform_.parent_ = &worldTransform_;
}

void Player::AttackHit(BaseEnemy* enemy)
{
	if (workAttack_.contactRecord_.HistoryCheck(enemy->GetSerialNumber())) {
		return;
	}

	//接触履歴に追加
	workAttack_.contactRecord_.AddRecord(enemy->GetSerialNumber());

	enemy->TakeHammer(workAttack_.attackPower_);
}

void Player::HammerUpdate()
{
	if (workAttack_.attackParameter_ <= workAttack_.swingEndTime_) {
		hammer_.worldTransform_.rotation_.y -= 0.5f;
	}

	hammer_.worldTransform_.UpdateMatrix();
	hammer_.object_->WorldTransformUpdate(hammer_.worldTransform_);

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
	if (input_->PushKey(DIK_A) || input_->PushButton(XINPUT_GAMEPAD_DPAD_LEFT) || input_->GetLeftStickX() < -0.5f) {
		worldTransform_.translation_.x -= speed_;
		if (lrDirection_ != LRDirection::kLeft) {
			lrDirection_ = LRDirection::kLeft;
			targetAngle_ = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];
			turnFirstRotationY_ = worldTransform_.rotation_.y;
			turnTimer_ = 0.0f;
		}
	}
	if (input_->PushKey(DIK_D) || input_->PushButton(XINPUT_GAMEPAD_DPAD_RIGHT) || input_->GetLeftStickX() > 0.5f) {
		worldTransform_.translation_.x += speed_;
		if (lrDirection_ != LRDirection::kRight) {
			lrDirection_ = LRDirection::kRight;
			targetAngle_ = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];
			turnFirstRotationY_ = worldTransform_.rotation_.y;
			turnTimer_ = 0.0f;
		}
	}

	//ジャンプ
	if (onGround_ && input_->TriggerKey(DIK_W) || input_->TriggerButton(XINPUT_GAMEPAD_A)) {
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
	
	//移動範囲制限
	worldTransform_.translation_.x = (std::max)(worldTransform_.translation_.x, moveRange_.min_);
	worldTransform_.translation_.x = (std::min)(worldTransform_.translation_.x, moveRange_.max_);

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
	if (workAttack_.attackParameter_ != 0) {
		return;
	}
	if (!input_->TriggerKey(DIK_SPACE) && !input_->TriggerButton(XINPUT_GAMEPAD_X)) {
		return;
	}
	workAttack_.isAttack_ = true;
	hammer_.worldTransform_.rotation_.y = destinationRotationYTable[0];
}

void Player::Fire()
{
	if (!input_->TriggerKey(DIK_K) && !input_->TriggerButton(XINPUT_GAMEPAD_RIGHT_THUMB) && !input_->TriggerButton(XINPUT_GAMEPAD_RIGHT_SHOULDER)) {
		return;
	}
	if (lrDirection_ == LRDirection::kRight) {
		gameScene_->AddPlayerBullet({ bulletSpped_, 0.0f, 0.0f });
		return;
	}
	else {
		gameScene_->AddPlayerBullet({ -bulletSpped_, 0.0f, 0.0f });
		return;
	}
	
}

void Player::AttackUpdate()
{
	
	if (workAttack_.isAttack_) {
		workAttack_.attackParameter_++;
		if (workAttack_.attackParameter_ > workAttack_.swingEndTime_) {
			workAttack_.isAttack_ = false;
			return;
		}
		return;
	}

	if (workAttack_.attackParameter_ > workAttack_.swingEndTime_) {
		workAttack_.attackParameter_++;
		if (workAttack_.attackParameter_ > workAttack_.attackTime_) {
			workAttack_.contactRecord_.Clear();
			workAttack_.attackParameter_ = 0;
			return;
		}
		return;
	}
}

void Player::InvincibleTimeUpdate()
{
	if (status_.remainingInvincibleTime_ == 0) {
		return;
	}
	status_.remainingInvincibleTime_--;
}

Player::WorkAttack::WorkAttack()
	: isAttack_(false)
	, attackPower_(2)
	, attackRange_({ 3.3f, 1.0f })
	, attackParameter_(0)
	, attackTime_(24)
	, swingEndTime_(6)
{
}

Player::MoveRange::MoveRange()
	: min_(-20.0f)
	, max_(20.0f)
{
}

Player::StatusWork::StatusWork()
	: maxHP_(3)
	, HP_(maxHP_)
	, isAlive_(true)
	, remainingInvincibleTime_(0)
	, maxInvincibleTime_(60)
{
}
