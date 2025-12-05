#include "Player.h"
#include "WinApp.h"
#include "Camera.h"
#include "Lerp.h"
#include "manager/PlayerBulletManager.h"
#include "ReticleController.h"
#include "TransformHelpers.h"
#include "bullet/PlayerBulletType.h"
#include "Matrix.h"
#include "manager/EffectManager.h"
#include "Random.h"

#ifdef USE_IMGUI
#include "imgui/imgui.h"
#endif // USE_IMGUI

using namespace YKEngine;

void Player::Initialize(BaseModel* model, WorldTransform* parent, uint32_t heartTextureHandle, uint32_t heartEmptyTexturehandle)
{

	BaseCharacter::Initialize(model);
	Collider::SetTypeID(CollisionTypeIdDef::kPlayer);

	input_ = Input::GetInstance();

	worldTransform_.parent_ = parent;
	
	BaseCharacter::Update();

	//開始時のアニメーション設定
	startAnime_ = std::make_unique<SRTAnimator>();
	startAnime_->SetAnimation({ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, 1.5f);

	reticleController_ = std::make_unique<ReticleController>();
	reticleController_->Initialize();

	HUDInitialize(heartTextureHandle, heartEmptyTexturehandle);

}

void Player::Update(Camera* railCamera) {


#ifdef USE_IMGUI

	ImGui::Begin("Player");
	if (ImGui::TreeNode("Object")) {
		ImGui::ColorEdit4("color", &object_->GetModel().GetMaterialDataAddress().color.x);
		ImGui::DragFloat3("translate", &worldTransform_.translation_.x, 0.01f);
		ImGui::DragFloat3("rotate", &worldTransform_.rotation_.x, 0.01f);
		ImGui::DragFloat3("scale", &worldTransform_.scale_.x, 0.01f);

		ImGui::TreePop();
	}
	ImGui::DragInt("HP", &hitPoint_, 1.0f, 0, 100);
	ImGui::End();


#endif // USE_IMGUI

	switch (phase_)
	{
	case Player::Phase::Start:
		UpdateStart();
		break;
	case Player::Phase::kMain:
		UpdateMain(railCamera);
		break;
	case Player::Phase::GameOver:
		UpdateGameOver();
		break;
	default:
		break;
	}
	
	
}

void Player::OnCollision(Collider* other)
{
	CollisionTypeIdDef typeId = other->GetTypeID();

	if (typeId == CollisionTypeIdDef::kEnemyBullet || typeId == CollisionTypeIdDef::kTackleEnemy)
	{
		hitPoint_--;

		if (hitPoint_ > 0) {
			return;
		}
		isDead_ = true;
	}
}

void Player::DrawUI()
{
	reticleController_->Draw();

	//HPの表示
	for (int i = 0; i < kMaxHitPoint_; i++)
	{
		if (i < hitPoint_)
		{
			heratSprites_[i]->Draw();
		}
		else
		{
			heratEmptySprites_[i]->Draw();
		}
	}
}

void Player::SetLockOnTarget(const std::list<std::unique_ptr<BaseEnemy>>& enemies, Camera* railCamera)
{
	reticleController_->SetLockOnTarget(enemies, railCamera);
}

Vector3 Player::GetWorldPosition() {

	return worldTransform_.GetWorldPosition();

}

Vector3 Player::GetInverseLocalDirection()
{
	return -direction_;
}

void Player::GameOverRotate()
{
	Vector3 toPosition;
	if (reticleController_->IsLockOn())
	{
		toPosition = reticleController_->GetTargetPosition();
	}
	else
	{
		toPosition = reticleController_->Get3DReticlePosition();
	}
	direction_ = Subtract(toPosition, GetWorldPosition());

	//親の回転を考慮した方向ベクトルの計算
	Matrix4x4 parentMat = MakeRotateMatrix(worldTransform_.parent_->rotation_);
	Matrix4x4 invParentMat = Inverse(parentMat);

	//親の回転を打ち消す
	Vector3 localDirection = TransformNormal(direction_, invParentMat);
	Vector3 targetRotation = TransformHelpers::FaceToVelocityDirection(worldTransform_.rotation_, localDirection);
	
	worldTransform_.rotation_ = targetRotation;
	BaseCharacter::Update();
}

void Player::HUDInitialize(uint32_t heartTextureHandle, uint32_t heartEmptyTexturehandle)
{
	heratSprites_.resize(kMaxHitPoint_);
	heratEmptySprites_.resize(kMaxHitPoint_);

	for (int i = 0; i < kMaxHitPoint_; i++)
	{
		heratSprites_[i] = std::make_unique<Sprite>();
		heratSprites_[i]->Initialize(heartTextureHandle);
		heratSprites_[i]->SetPosition(Vector2(50.0f + i * 50.0f, 50.0f)); //位置を設定
		heratSprites_[i]->SetSize(Vector2(50.0f, 50.0f)); //サイズを設定

		heratEmptySprites_[i] = std::make_unique<Sprite>();
		heratEmptySprites_[i]->Initialize(heartEmptyTexturehandle);
		heratEmptySprites_[i]->SetPosition(Vector2(50.0f + i * 50.0f, 50.0f)); //位置を設定
		heratEmptySprites_[i]->SetSize(Vector2(50.0f, 50.0f)); //サイズを設定
	}
}

void Player::HandleMoveInput()
{
	//キャラクターの移動ベクトル
	Vector3 move = { 0, 0, 0 };

	move.x = input_->GetLeftStickX();
	move.y = input_->GetLeftStickY();

	if (move.x == 0 && move.y == 0) {
		//左スティックがニュートラルなら、キーボード入力を確認
		//押した方向で移動ベクトルを変更(左右)
		if (input_->PushKey(DIK_A)) {
			move.x = -1.0f;
		}
		else if (input_->PushKey(DIK_D)) {
			move.x = 1.0f;
		}

		// 押した方向で移動ベクトルを変更(上下)
		if (input_->PushKey(DIK_S)) {
			move.y = -1.0f;
		}
		else if (input_->PushKey(DIK_W)) {
			move.y = 1.0f;
		}

		move = Normalize(move); //移動ベクトルの正規化

	}

	//キャラクターの移動速さ
	const float kCharacterSpeed = 0.2f;
	//移動ベクトルの速さの適用
	move *= kCharacterSpeed;

	//座標移動(ベクトルの加算)
	worldTransform_.translation_ += move;
}

void Player::UpdateStart()
{
	characterWorldTransform_.scale_ = startAnime_->Update();
	BaseCharacter::Update();
	
	EffectManager* effectManager = EffectManager::GetInstance();

	effectManager->SpawnEffect(EffectType::kGather01, GetWorldPosition());
	effectManager->SpawnEffect(EffectType::kGather02, GetWorldPosition());

	if (startAnime_->GetIsEnd())
	{
		phase_ = Phase::kMain;
		
		effectManager->SpawnEffect(EffectType::kScatter02, GetWorldPosition(), 100);
	}
}

void Player::UpdateMain(Camera* railCamera)
{
	HandleMoveInput();

	//移動限界座標
	const float kMoveLimitX = 8.9f;
	const float kMoveLimitY = 4.8f;

	//範囲を超えない処理
	worldTransform_.translation_.x = std::clamp(worldTransform_.translation_.x, -kMoveLimitX, kMoveLimitX);
	worldTransform_.translation_.y = std::clamp(worldTransform_.translation_.y, -kMoveLimitY, kMoveLimitY);

	//回転
	Rotate();

	BaseCharacter::Update();

	//照準オブジェクトの更新
	ReticleUpdate(railCamera);

	//チャージ処理
	Charge();

	//キャラクター攻撃処理
	Attack();
}

void Player::UpdateGameOver()
{
	if (isGameOverEnd_)
	{
		return;
	}
	//乱数での移動量の設定
	const float kMoveRange = 0.1f;

	std::uniform_real_distribution<float> distribution(-kMoveRange, kMoveRange);

	//乱数生成エンジンへのポインタ
	std::mt19937* randomEngine = Random::GetInstance()->GetRandomEnginePtr();

	characterWorldTransform_.translation_ = { distribution(*randomEngine), distribution(*randomEngine), distribution(*randomEngine) };

	EffectManager::GetInstance()->SpawnEffect(EffectType::kScatter01, characterWorldTransform_.GetWorldPosition(), 2);

	gameOverTimer_ += 1.0f / 60.0f;

	if (gameOverTimer_ >= 1.5f)
	{
		isGameOverEnd_ = true;
		characterWorldTransform_.scale_ = { 0.0f, 0.0f, 0.0f };
		EffectManager::GetInstance()->SpawnEffect(EffectType::kScatter03, characterWorldTransform_.GetWorldPosition(), 100);
	}

	BaseCharacter::Update();
}

void Player::Rotate()
{
	//向く方向の計算
	Vector3 toPosition;
	if (reticleController_->IsLockOn())
	{
		toPosition = reticleController_->GetTargetPosition();
	}
	else
	{
		toPosition = reticleController_->Get3DReticlePosition();
	}
	direction_ = Subtract(toPosition, GetWorldPosition());
	
	//親の回転を考慮した方向ベクトルの計算
	Matrix4x4 parentMat = MakeRotateMatrix(worldTransform_.parent_->rotation_);
	Matrix4x4 invParentMat = Inverse(parentMat);

	//親の回転を打ち消す
	Vector3 localDirection = TransformNormal(direction_, invParentMat);
	Vector3 targetRotation = TransformHelpers::FaceToVelocityDirection(worldTransform_.rotation_, localDirection);
	
	worldTransform_.rotation_ = LerpAngle(worldTransform_.rotation_, targetRotation, 0.1f);
}

void Player::ReticleUpdate(Camera* railCamera)
{
	reticleController_->Update(railCamera);
}

void Player::Attack() {

	if (input_->TriggerKey(DIK_SPACE) || input_->TriggerButton(XINPUT_GAMEPAD_RIGHT_SHOULDER))
	{

		//弾の速度
		const float kBulletSpeed = 2.0f;

		Vector3 velocity = Multiply(kBulletSpeed, Normalize(direction_));
		
		//弾を生成し、初期化
		if (isChargeMax_)
		{
			//チャージ最大なら強力な弾を撃つ
			playerBulletManager_->AddPlayerBullet(GetWorldPosition(), velocity, PlayerBulletType::kCharge);

			//チャージをリセット
			ChargeReset();
			return;
		}

		playerBulletManager_->AddPlayerBullet(GetWorldPosition(), velocity, PlayerBulletType::kNormal);
		//チャージをリセット
		ChargeReset();

	}

}

void Player::Charge()
{
	//チャージが最大なら処理しない
	if (isChargeMax_)
	{
		return;
	}

	//チャージ
	chargeTime_ += 1.0f / 60.0f;
	if (chargeTime_ >= kMaxChargeTime_) 
	{
		chargeTime_ = kMaxChargeTime_;
		isChargeMax_ = true;
		reticleController_->ChargeMax();
	}
}

void Player::ChargeReset()
{
	isChargeMax_ = false;
	chargeTime_ = 0.0f;
	reticleController_->ChargeReset();
}
