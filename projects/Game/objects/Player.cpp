#include "Player.h"
#include "imgui/imgui.h"
#include "Rigid3dObject.h"

void Player::Initialize(BaseModel* model) {

	BaseCharacter::Initialize(model);

	// 最初の状態をIdleにする
	ChangeState(std::make_unique<PlayerIdleState>());
}

void Player::Update() {

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
	
	// ステートの更新
	StateUpdate();

	// 基底クラスの更新
	BaseCharacter::Update();

}

void Player::Draw(Camera* camera) {

	BaseCharacter::Draw(camera);

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


void Player::HandleInput()
{
	Input* input = Input::GetInstance();

	if (input->PushKey(DIK_SPACE) && !dynamic_cast<PlayerJumpState*>(state_.get()))
	{
		state_->Exit();

		state_ = std::move(std::make_unique<PlayerJumpState>());

		state_->Enter(this);
	}
}

void Player::StateUpdate()
{
	if (state_)
	{
		state_->Update();
	}
}

