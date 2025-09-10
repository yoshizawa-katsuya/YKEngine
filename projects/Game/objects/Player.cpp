#include "Player.h"
#include "imgui/imgui.h"
#include "Rigid3dObject.h"
#include "Input.h"
#include <numbers>
#include "MapChipField.h"
#include "Easing.h"
#include "Lerp.h"

#include "PlayerJumpState.h"
#include "PlayerGroundState.h"

void Player::Initialize(BaseModel* model, const Vector3& position)
{
	input_ = Input::GetInstance();

	BaseCharacter::Initialize(model, position);

	kWidth_ = 0.719f;
	kHeight_ = 1.79f;

	// 最初の状態をIdleにする
	ChangeState(std::make_unique<PlayerGroundState>());
}

/*
void Player::SetElectricModel(BaseModel* electricModel) {
	electricRange_ = std::make_unique<ElectricRange>();
	electricRange_->Initialize(electricModel, worldTransform_.translation_);
	electricRange_->SetColor({ 1.0f, 0.902f, 0.0f, 0.45f });
	electricRange_->SetFollowTarget(&worldTransform_.translation_); 
}
*/
void Player::Update() {

	//移動入力
	Move();

	// ステートの更新
	StateUpdate();

	// 基底クラスの更新
	BaseCharacter::Update();

	//electricRange_->Update();

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
	//electricRange_->DebugImGui();

#endif // _DEBUG	

}

void Player::Draw(Camera* camera) {

	BaseCharacter::Draw(camera);
	//電気範囲
    //electricRange_->Draw(camera);

}

void Player::ChangeState(std::unique_ptr<IPlayerState> state)
{
	if (!state)
	{
		return;
	}

	// もしstate_がnullptrじゃなかったら
	if (state_)
	{
		// state_のExitを呼ぶ
		state_->Exit();
	}

	// state_に引数のstateをムーブする
	state_ = std::move(state);

	// state_のEnterを呼ぶ
	state_->Enter(this);
}

void Player::StateUpdate()
{
	if (state_)
	{
		state_->Update();
	}
}

void Player::CheckHitMapChipType(MapChipType mapChipType)
{
	switch (mapChipType)
	{
	case MapChipType::kBlock:

		hitBlock_ = true;
		break;

	case MapChipType::kDoorTrigger:

		hitBlock_ = true;
		break;

	case MapChipType::kClosedDoor:

		hitBlock_ = true;
		break;

	case MapChipType::kDisappearTrigger:

		hitBlock_ = true;
		break;

	case MapChipType::kInactiveDisappear:

		hitBlock_ = true;
		break;

	case MapChipType::kAppearTrigger:

		hitBlock_ = true;
		break;

	case MapChipType::kActiveAppear:

		hitBlock_ = true;
		break;

	case MapChipType::kSpineTrap:

		hitSpine_ = true;
		break;

	case MapChipType::kSpineTrigger:

		hitSpine_ = true;
		break;

	case MapChipType::kActiveSpine:

		hitSpine_ = true;
		break;
	

	case MapChipType::kGoal:
		hitGoal_ = true;
		break;

	

	

	default:
		break;
	}
}

void Player::Move()
{
	velocity_.x = 0.0f;	// 水平速度リセット
	// 移動入力
	// 左右移動操作
	if (Input::GetInstance()->PushKey(DIK_D)) {

		velocity_.x += kMoveSpeed_;
		if (lrDirection_ != LRDirection::kRight) {
			lrDirection_ = LRDirection::kRight;
			turnFirstRotationY_ = worldTransform_.rotation_.y;
			turnTimer_ = 0.0f;
		}
	}
	else if (Input::GetInstance()->PushKey(DIK_A)) {
		velocity_.x -= kMoveSpeed_;
		if (lrDirection_ != LRDirection::kLeft) {
			lrDirection_ = LRDirection::kLeft;
			turnFirstRotationY_ = worldTransform_.rotation_.y;
			turnTimer_ = 0.0f;
		}
	}

	if (dynamic_cast<PlayerGroundState*>(state_.get()) && input_->TriggerKey(DIK_W))
	{
		velocity_.y += physicsParam_.kJumpAcceleration;

		state_->Exit();

		ChangeState(std::make_unique<PlayerJumpState>());

		state_->Enter(this);

		Audio::GetInstance()->SoundPlayWave(*jumpSE_);
	}
	else if (dynamic_cast<PlayerJumpState*>(state_.get()) && onGround_)
	{
		state_->Exit();

		ChangeState(std::make_unique<PlayerGroundState>());

		state_->Enter(this);
	}
	else if (dynamic_cast<PlayerGroundState*>(state_.get()) && !onGround_)
	{
		state_->Exit();

		ChangeState(std::make_unique<PlayerJumpState>());

		state_->Enter(this);
	}
}

void Player::SpineCollision(const CollisionMapInfo& info)
{
	if (!info.isSpineCollision)
	{
		return;
	}
	isDead_ = true;
}
