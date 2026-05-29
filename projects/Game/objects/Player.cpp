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

	startPosition_ = worldTransform_.translation_;
	startRotation_ = worldTransform_.rotation_;
	startScale_ = worldTransform_.scale_;

	hitStopTimer_ = kHitStopTime_;
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

	switch (state_)
	{
	case PlayerState::Normal:
		// 生きているとき
		prevPose_ = pose_;
		if (isAutoPoseDemo_) {
			worldTransform_.rotation_.y += 0.02f;
			autoPoseTimer_ += 1.0f / 60.0f;

			// 5秒ごとに切り替え
			if (autoPoseTimer_ >= 5.0f) {
				autoPoseTimer_ = 0.0f;
				autoPoseIndex_++;

				if(autoPoseIndex_>3){
					autoPoseIndex_ = 0;
				}
			}

			switch (autoPoseIndex_) {

			case 0:
				pose_ = PlayerPose::A;
				break;

			case 1:
				pose_ = PlayerPose::B;
				break;

			case 2:
				pose_ = PlayerPose::C;
				break;

			case 3:
				pose_ = PlayerPose::D;
				break;
			}
		}
		else {
    		ChangePose();
    		ChangeDirection();
		}


		// 死亡開始
		if (requestDeath_)
		{
			StartDeathAnimation();
			requestDeath_ = false;
		}

		break;
	case PlayerState::HitImpact:

		hitStopTimer_ -= 1.0f / 60.0f;

		if (hitStopTimer_ <= 0.0f)
		{
			state_ = PlayerState::Dead;
		}

		break;


	case PlayerState::Dead:
		// 死んでいるとき
		PlayDeathAnimation();

		break;

	case PlayerState::DeadFinished:
		// 死亡演出終了後（完全に吹っ飛んでしまった後など）
		break;
	}
	


	UpdateColorForDebug();

	UpdateAnimationTrigger();
	UpdateAnimationTimers();

	// 現在アクティブなアニメーションのみを更新・適用する
	if (currentAnimation_ && state_ != PlayerState::HitImpact) {
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

bool Player::ConsumeResetRequest()
{
	if (resetRequested_)
	{
		resetRequested_ = false;
		return true;
	}

	return false;
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
	state_ = PlayerState::Normal;

	deathVelocity_ = {};
	
	deathTimer_ = 0.0f;

	hitStopTimer_ = kHitStopTime_;

	resetRequested_ = true;
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
	else if (input_->PushKey(DIK_W))
	{
		pose_ = PlayerPose::A;
	}
	else if (input_->PushKey(DIK_A))
	{
		pose_ = PlayerPose::B;
	}
	else if (input_->PushKey(DIK_S))
	{
		pose_ = PlayerPose::C;
	}
	else if (input_->PushKey(DIK_D))
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

	// 1. 新しいポーズが「Base以外」なら、迷わずそのポーズを再生する
	//    (A→B への移動も、Base→A への移動もこれで解決します)
	if (pose_ != PlayerPose::Base) {
		isReturnPhase_ = false; // Return処理を中断

		if (pose_ == PlayerPose::Squat) PlayAnimation("Squat");
		else if (pose_ == PlayerPose::A)     PlayAnimation("PoseA");
		else if (pose_ == PlayerPose::B)     PlayAnimation("PoseB");
		else if (pose_ == PlayerPose::C)     PlayAnimation("PoseC");
		else if (pose_ == PlayerPose::D)     PlayAnimation("PoseD");
	}
	// 2. 新しいポーズが「Base」なら、Return処理を開始する
	else if (pose_ == PlayerPose::Base) {
		isReturnPhase_ = true;

		// 直前まで再生していたポーズに応じたReturnモーションを再生
		if (prevPose_ == PlayerPose::Squat) PlayAnimation("SquatReturn");
		else if (prevPose_ == PlayerPose::A)     PlayAnimation("PoseAReturn");
		else if (prevPose_ == PlayerPose::B)     PlayAnimation("PoseBReturn");
		else if (prevPose_ == PlayerPose::C)     PlayAnimation("PoseCReturn");
		else if (prevPose_ == PlayerPose::D)     PlayAnimation("PoseDReturn");

		// モーションの長さをセット
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
	state_ = PlayerState::HitImpact;

	switch (deathVariation_)
	{
	case DeathVariation::Right:
		// 右手前方向へ吹っ飛ばす
		deathVelocity_ = kRightDeathVelocity;
		deathRotateVelocity_ =
		{
			0.35f,
			0.08f,
			-0.15f
		};
		break;

	case DeathVariation::Left:
		// 左手前方向へ吹っ飛ばす
		deathVelocity_ = kLeftDeathVelocity;
		deathRotateVelocity_ =
		{
			0.35f,
			-0.08f,
			0.15f
		};
		break;
	case DeathVariation::InFront:
		// 正面方向へ吹っ飛ばす
		deathVelocity_ = kFrontDeathVelocity;
		deathRotateVelocity_ =
		{
			0.55f,
			0.0f,
			0.0f
		};
		break;
	}
	
	deathTimer_ = 0.0f;
}

void Player::PlayDeathAnimation()
{
	// 死亡アニメーションの再生
	deathTimer_ += 1.0f / 60.0f;

	Vector3 dir = Normalize(deathVelocity_);

	// 移動
	worldTransform_.translation_.x += deathVelocity_.x;
	worldTransform_.translation_.y += deathVelocity_.y;
	worldTransform_.translation_.z += deathVelocity_.z;

	// 回転
	worldTransform_.rotation_.x += deathRotateVelocity_.x;
	worldTransform_.rotation_.y = std::atan2(dir.x, dir.z);
	worldTransform_.rotation_.y += deathRotateVelocity_.y;
	worldTransform_.rotation_.z += deathRotateVelocity_.z;

	// 重力
	deathVelocity_.y -= 0.01f;

	// 少し減速
	deathVelocity_.x *= 0.99f;
	deathVelocity_.z *= 0.99f;

	deathRotateVelocity_.x *= 0.985f;
	deathRotateVelocity_.y *= 0.985f;
	deathRotateVelocity_.z *= 0.985f;

	// 演出終了
	if (worldTransform_.translation_.z < -75.0f)
	{
		//state_ = PlayerState::DeadFinished;
	}
}