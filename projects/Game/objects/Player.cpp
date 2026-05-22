#include "Player.h"
#include "Input.h"
#include <numbers>
#ifdef USE_IMGUI
#include "imgui/imgui.h"
#endif // USE_IMGUI

using namespace YKEngine;

void Player::Initialize(BaseModel* model) {

	object_ = std::make_unique<Skin3dObject>();
	object_->Initialize(model);

	input_ = Input::GetInstance();

	worldTransform_.Initialize();

	pose_ = PlayerPose::Base;
	direction_ = PlayerDirection::Front;

	kAngle_=std::numbers::pi_v<float>/4.0f;
	
	// 全てのアニメーションファイルを最初にメモリにロード（プリロード）
	std::vector<std::string> animNames = {
		"Stay", "Squat", "SquatReturn",
		"PoseA", "PoseAReturn", "PoseB", "PoseBReturn",
		"PoseC", "PoseCReturn", "PoseD", "PoseDReturn"
	};

	for (const auto& name : animNames) {
		auto anim = std::make_unique<YKEngine::Animation>();
		anim->LoadAnimationFile("./resources/playerAnimation", name + ".gltf");
		animations_[name] = std::move(anim);
	}
	kAngle_ = std::numbers::pi_v<float> / 4.0f;

	startPosition_ = worldTransform_.translation_;
	startRotation_ = worldTransform_.rotation_;
	startScale_ = worldTransform_.scale_;
}

	// 初期アニメーションを設定
	PlayAnimation("Stay");
}

void Player::Update() {


#ifdef USE_IMGUI

	ImGui::Begin("Player");
	if (ImGui::TreeNode("Model")) {
		ImGui::DragFloat3("translate", &worldTransform_.translation_.x, 0.01f);
		ImGui::DragFloat3("rotate", &worldTransform_.rotation_.x, 0.01f);
		ImGui::DragFloat3("scale", &worldTransform_.scale_.x, 0.01f);
		int poseInt = static_cast<int>(pose_);
		ImGui::DragInt("pose", &poseInt, 1.0f, 0, 5);
		pose_ = static_cast<PlayerPose>(poseInt);
		ImGui::SliderFloat3("RightDeathVelocity", &kRightDeathVelocity.x, -2.0f, 2.0f);
		ImGui::SliderFloat3("LeftDeathVelocity", &kLeftDeathVelocity.x, -2.0f, 2.0f);
		ImGui::SliderFloat3("FrontDeathVelocity", &kFrontDeathVelocity.x, -2.0f, 2.0f);
		// deathVariationの切り替え
		const char* variations[] = { "Right", "Left", "InFront" };
		int variationIndex = static_cast<int>(deathVariation_);
		if (ImGui::Combo("Death Variation", &variationIndex, variations, IM_ARRAYSIZE(variations))) {
			deathVariation_ = static_cast<DeathVariation>(variationIndex);
		}
		
		if (ImGui::Button("Death")) { RequestDeath(); }
		if (ImGui::Button("Reset")) { Reset(); }
		ImGui::TreePop();
	}
	ImGui::End();


#endif // USE_IMGUI	

	// 死亡開始
	if (requestDeath_ && !isDead_)
	{
		StartDeathAnimation();

		requestDeath_ = false;
	}

	// 死亡アニメーション再生中は操作を受け付けない
	if (isDead_)
	{
		PlayDeathAnimation();
	}
	else
	{
		ChangePose();
		ChangeDirection();
	}
	prevPose_ = pose_;
	ChangePose();
	ChangeDirection();

	UpdateColorForDebug();

	UpdateAnimationTrigger();
	UpdateAnimationTimers();

	// 現在アクティブなアニメーションのみを更新・適用する
	if (currentAnimation_) {
		bool isLoop = (currentAnimationName_ == "Stay");

		currentAnimation_->Update(isLoop);
		object_->AnimationUpdate(currentAnimation_);
	}

	worldTransform_.UpdateMatrix();
	object_->WorldTransformUpdate(worldTransform_);

}

void Player::Draw(Camera* camera) {

	object_->CameraUpdate(camera);
	object_->Draw();

}

void Player::SetColorForDebug(Vector4& color) const
{
	object_->SetColor(color);
}

void Player::Reset()
{
	// Transform戻す
	worldTransform_.translation_ = startPosition_;
	worldTransform_.rotation_ = startRotation_;
	worldTransform_.scale_ = startScale_;

	// 状態戻す
	pose_ = PlayerPose::Base;
	direction_ = PlayerDirection::Front;

	// 死亡関連リセット
	requestDeath_ = false;
	isDead_ = false;
	isDeathFinished_ = false;

	deathVelocity_ = {};
	deathRotateVelocity_ = {};

	deathTimer_ = 0.0f;
}

void Player::ChangePose()
{
	// ポーズ切り替え
	// デフォルト
	pose_ = PlayerPose::Base;

	// 押されたらポーズ変更
	if (input_->PushKey(DIK_DOWNARROW))
	{
		pose_ = PlayerPose::Squat;
	}
	else if (input_->PushKey(DIK_1))
	{
		pose_ = PlayerPose::A;
	}
	else if (input_->PushKey(DIK_2))
	{
		pose_ = PlayerPose::B;
	}
	else if (input_->PushKey(DIK_3))
	{
		pose_ = PlayerPose::C;
	}
	else if (input_->PushKey(DIK_4))
	{
		pose_ = PlayerPose::D;
	}
}

void Player::ChangeDirection()
{
	// 向き切り替え
	int dir = static_cast<int>(direction_);
	if (input_->TriggerKey(DIK_LEFTARROW))
		dir--;
	if (input_->TriggerKey(DIK_RIGHTARROW))
		dir++;

	// 範囲制限（0～2）
	dir = std::clamp(dir, 0, 2);
	direction_ = static_cast<PlayerDirection>(dir);

	// -1, 0, 1 に変換
	int offset = dir - 1;

	// 目標角度
	float targetRot = offset * kAngle_;

	// 現在角度
	float currentRot = worldTransform_.rotation_.y;

	// スムーズ回転
	float speed = 20.0f;
	float deltaTime = 1.0f / 60.0f; // 仮

	worldTransform_.rotation_.y += (targetRot - currentRot) * speed * deltaTime;
	if (fabs(targetRot - worldTransform_.rotation_.y) < 0.001f)
	{
		worldTransform_.rotation_.y = targetRot;
	}
}

void Player::UpdateColorForDebug()
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

	object_->SetColor(kPoseColors[static_cast<int>(pose_)]);
}

void Player::PlayAnimation(const std::string& name)
{
	// 無駄な重ねがけ（同じモーションの再トリガー）を防止
	if (currentAnimationName_ == name) return;

	auto it = animations_.find(name);
	if (it != animations_.end()) {
		currentAnimation_ = it->second.get();
		currentAnimationName_ = name;

		// 切り替えた瞬間にアニメーション時間を 0.0f に巻き戻す（頭出し）
		currentAnimation_->SetAnimationTime(0.0f);

		std::string animLog = "[Anim Switch] Activated -> " + name + "\n";
		OutputDebugStringA(animLog.c_str());
	}
}

void Player::UpdateAnimationTrigger() {
	// ポーズが変わっていなければ何もしない
	if (pose_ == prevPose_) return;

	// Base状態から特定のキーが押された瞬間（各ポーズの開始）
	if (prevPose_ == PlayerPose::Base) {
		isReturnPhase_ = false; // 他のポーズへの割り込み時はReturnフラグを折る

		if (pose_ == PlayerPose::Squat) PlayAnimation("Squat");
		else if (pose_ == PlayerPose::A)     PlayAnimation("PoseA");
		else if (pose_ == PlayerPose::B)     PlayAnimation("PoseB");
		else if (pose_ == PlayerPose::C)     PlayAnimation("PoseC");
		else if (pose_ == PlayerPose::D)     PlayAnimation("PoseD");
	}
	// キーが離されてBase状態（元の姿勢）に戻った瞬間（Returnモーションの開始）
	else if (pose_ == PlayerPose::Base) {
		isReturnPhase_ = true;

		if (prevPose_ == PlayerPose::Squat) PlayAnimation("SquatReturn");
		else if (prevPose_ == PlayerPose::A)     PlayAnimation("PoseAReturn");
		else if (prevPose_ == PlayerPose::B)     PlayAnimation("PoseBReturn");
		else if (prevPose_ == PlayerPose::C)     PlayAnimation("PoseCReturn");
		else if (prevPose_ == PlayerPose::D)     PlayAnimation("PoseDReturn");

		// 現在のアニメーションの正確な長さをタイマーに設定
		if (currentAnimation_) {
			returnTimer_ = currentAnimation_->GetDuration();
		}
	}
}

void Player::UpdateAnimationTimers() {
	// Returnフェーズ中でなければ何もしない
	if (!isReturnPhase_) return;

	// 1フレーム分の時間を減算（1/60秒固定値。可変フレーム時はdeltaTime等を使用）
	returnTimer_ -= (1.0f / 60.0f);

	// モーション時間が終了したら通常待機（Stay）へ自動遷移
	if (returnTimer_ <= 0.0f) {
		isReturnPhase_ = false;
		PlayAnimation("Stay");
	}
}

void Player::StartDeathAnimation()
{
	isDead_ = true;

	switch (deathVariation_)
	{
	case DeathVariation::Right:
		// 右手前方向へ吹っ飛ばす
		deathVelocity_ = kRightDeathVelocity;
		break;

	case DeathVariation::Left:
		// 左手前方向へ吹っ飛ばす
		deathVelocity_ = kLeftDeathVelocity;
		break;
	case DeathVariation::InFront:
		// 正面方向へ吹っ飛ばす
		deathVelocity_ = kFrontDeathVelocity;
		break;
	}
	// グルグル回転
	deathRotateVelocity_ =
	{
		0.25f,
		0.35f,
		0.15f
	};

	deathTimer_ = 0.0f;
}

void Player::PlayDeathAnimation()
{
	// 死亡アニメーションの再生
	deathTimer_ += 1.0f / 60.0f;

	// 移動
	worldTransform_.translation_.x += deathVelocity_.x;
	worldTransform_.translation_.y += deathVelocity_.y;
	worldTransform_.translation_.z += deathVelocity_.z;

	// 回転
	worldTransform_.rotation_.x += deathRotateVelocity_.x;
	worldTransform_.rotation_.y += deathRotateVelocity_.y;
	worldTransform_.rotation_.z += deathRotateVelocity_.z;

	// 重力
	deathVelocity_.y -= 0.01f;

	// 少し減速
	deathVelocity_.x *= 0.99f;
	deathVelocity_.z *= 0.99f;

	// 演出終了
	if (worldTransform_.translation_.z < -20.0f)
	{
		isDeathFinished_ = true;
	}
}
