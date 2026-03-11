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
#include "ModelPlatform.h"
#include "GlobalVariables.h"
#include "JsonKeys.h"
#include "PlayerStartState.h"

#ifdef USE_IMGUI
#include "imgui/imgui.h"
#endif // USE_IMGUI

using namespace YKEngine;

void Player::Initialize(WorldTransform* parent)
{
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const std::string& groupName = JsonKey::Player::kGroupName;
	globalVariables->CreateGroup(groupName);
	globalVariables->AddItem(groupName, JsonKey::Player::kDodgeSpeed, 0.6f);
	globalVariables->AddItem(groupName, JsonKey::Player::kDodgeLerpFactor, 1.0f / 20.0f);

	BaseCharacter::Initialize(ModelPlatform::GetInstance()->CreateRigidModel("./Resources/player", "Player.obj").get());
	Collider::SetTypeID(CollisionTypeIdDef::kPlayer);

	input_ = Input::GetInstance();

	worldTransform_.parent_ = parent;
	
	BaseCharacter::Update();

	//開始時のアニメーション設定
	const float kAnimeDuration = 1.5f;
	startAnime_ = std::make_unique<SRTAnimator>();
	startAnime_->SetAnimation({ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, kAnimeDuration);

	const float kRotateQuantity = std::numbers::pi_v<float> * 4.0f;
	startRotateAnime_ = std::make_unique<SRTAnimator>();
	startRotateAnime_->SetAnimation({ 0.0f, 0.0f, 0.0f }, { 0.0f, kRotateQuantity, 0.0f }, kAnimeDuration);

	reticleController_ = std::make_unique<ReticleController>();
	reticleController_->Initialize();

	HUDInitialize();

	stateMachine_ = std::make_unique<StateMachine<PlayerStateContext>>();
	stateMachine_->Start(this);
	stateMachine_->ChangeState<PlayerStartState>();

}

void Player::Update()
{


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

	HeartUpdate();

	//状態遷移と更新
	stateMachine_->Update();
	
}

void Player::OnCollision(Collider* other)
{
	CollisionTypeIdDef typeId = other->GetTypeID();

	if (typeId == CollisionTypeIdDef::kEnemyBullet || typeId == CollisionTypeIdDef::kTackleEnemy)
	{
		hitPoint_--;
		//ダメージリアクション開始
		DamageReactionInitialize();

		EffectManager::GetInstance()->SpawnEffect(EffectType::kHit01, worldTransform_.GetWorldPosition());
		EffectManager::GetInstance()->SpawnEffect(EffectType::kHit02, worldTransform_.GetWorldPosition(), 10);

		if (hitPoint_ > 0)
		{
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
			heartSprites_[i]->Draw();
		}
		else
		{
			heartEmptySprites_[i]->Draw();
		}
	}
}

void Player::SetLockOnTarget(const std::list<std::unique_ptr<BaseEnemy>>& enemies)
{
	reticleController_->SetLockOnTarget(enemies, railCamera_);
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
	//向く方向の計算
	Vector3 targetRotation = RotateCommon();
	
	worldTransform_.rotation_ = targetRotation;

	BaseCharacter::Update();
}

void Player::HUDInitialize()
{
	//ハートの表示位置、サイズ、間隔をグローバル変数に登録
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const std::string& groupName = JsonKey::Player::kGroupName;
	globalVariables->AddItem(groupName, JsonKey::Player::kHeartPosition, Vector2(50.0f, 30.0f));
	globalVariables->AddItem(groupName, JsonKey::Player::kHeartSize, Vector2(50.0f, 50.0f));
	globalVariables->AddItem(groupName, JsonKey::Player::kHeartSpacing, 50.0f);

	//ハートのテクスチャ読み込み
	uint32_t heartTextureHandle = TextureManager::GetInstance()->Load("./Resources/heart.png");
	uint32_t heartEmptyTexturehandle = TextureManager::GetInstance()->Load("./Resources/heartFrame.png");

	heartSprites_.resize(kMaxHitPoint_);
	heartEmptySprites_.resize(kMaxHitPoint_);

	Vector2 heartPosition = globalVariables->GetVector2Value(groupName, JsonKey::Player::kHeartPosition);
	Vector2 heartSize = globalVariables->GetVector2Value(groupName, JsonKey::Player::kHeartSize);
	float heartSpacing = globalVariables->GetFloatValue(groupName, JsonKey::Player::kHeartSpacing);

	for (int i = 0; i < kMaxHitPoint_; i++)
	{
		heartSprites_[i] = std::make_unique<Sprite>();
		heartSprites_[i]->Initialize(heartTextureHandle);
		heartSprites_[i]->SetPosition(Vector2(heartPosition.x + i * heartSpacing, heartPosition.y)); //位置を設定
		heartSprites_[i]->SetSize(heartSize); //サイズを設定
		heartSprites_[i]->SetAlpha(0.0f); //最初は透明にする

		heartEmptySprites_[i] = std::make_unique<Sprite>();
		heartEmptySprites_[i]->Initialize(heartEmptyTexturehandle);
		heartEmptySprites_[i]->SetPosition(Vector2(heartPosition.x + i * heartSpacing, heartPosition.y)); //位置を設定
		heartEmptySprites_[i]->SetSize(heartSize); //サイズを設定
		heartEmptySprites_[i]->SetAlpha(0.0f); //最初は透明にする
	}
}

void Player::HandleMoveInput()
{
	//キャラクターの移動ベクトル
	move_ = { 0.0f, 0.0f, 0.0f };

	move_.x = input_->GetLeftStickX();
	move_.y = input_->GetLeftStickY();

	if (move_.x == 0 && move_.y == 0) {
		//左スティックがニュートラルなら、キーボード入力を確認
		//押した方向で移動ベクトルを変更(左右)
		if (input_->PushKey(DIK_A)) {
			move_.x = -1.0f;
		}
		else if (input_->PushKey(DIK_D)) {
			move_.x = 1.0f;
		}

		// 押した方向で移動ベクトルを変更(上下)
		if (input_->PushKey(DIK_S)) {
			move_.y = -1.0f;
		}
		else if (input_->PushKey(DIK_W)) {
			move_.y = 1.0f;
		}

		move_ = Normalize(move_); //移動ベクトルの正規化

	}
}

void Player::UpdateTilt()
{
	//キャラクターの傾きの設定
	const float rotateQuantity = std::numbers::pi_v<float> / 8.0f; //傾きの大きさ
	Vector3 targetRotation = { -move_.y * rotateQuantity, 0.0f, -move_.x * rotateQuantity };
	characterWorldTransform_.rotation_ = LerpAngle(characterWorldTransform_.rotation_, targetRotation, 0.1f);
}

void Player::Move()
{
	//キャラクターの移動速さ
	const float kCharacterSpeed = 0.2f;
	//移動ベクトルの速さの適用
	move_ *= kCharacterSpeed;

	//座標移動(ベクトルの加算)
	worldTransform_.translation_ += move_;
}

void Player::DodgeMove()
{
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	std::string groupName = JsonKey::Player::kGroupName;

	const float kDodgeSpeed = globalVariables->GetFloatValue(groupName, JsonKey::Player::kDodgeSpeed);
	t_ += globalVariables->GetFloatValue(groupName, JsonKey::Player::kDodgeLerpFactor);	//ドッジの時間経過

	if (t_ > 1.0f)
	{
		t_ = 1.0f; //ドッジの時間が最大を超えないようにする
	}

	move_ = Normalize(move_) * Lerp(kDodgeSpeed, 0.0f, EaseOutCubic(t_)); //ドッジの移動量を時間で減衰させる

	//座標移動(ベクトルの加算)
	worldTransform_.translation_ += move_;
}

void Player::ClampMove()
{
	//移動限界座標
	const float kMoveLimitX = 8.9f;
	const float kMoveLimitY = 4.8f;

	//範囲を超えない処理
	worldTransform_.translation_.x = std::clamp(worldTransform_.translation_.x, -kMoveLimitX, kMoveLimitX);
	worldTransform_.translation_.y = std::clamp(worldTransform_.translation_.y, -kMoveLimitY, kMoveLimitY);
}

void Player::UpdateStart()
{
	characterWorldTransform_.scale_ = startAnime_->Update();
	characterWorldTransform_.rotation_ = startRotateAnime_->Update();
	BaseCharacter::Update();
	
	EffectManager* effectManager = EffectManager::GetInstance();

	effectManager->SpawnEffect(EffectType::kPlayerStart01, GetWorldPosition());
	effectManager->SpawnEffect(EffectType::kPlayerStart02, GetWorldPosition());

}

void Player::UpdateMain()
{
	//移動入力の処理
	HandleMoveInput();

	//キャラクターの傾きの更新
	UpdateTilt();

	//キャラクターの移動処理
	Move();

	//移動限界の座標を超えないようにする処理
	ClampMove();

	//回転
	Rotate();

	BaseCharacter::Update();

	//照準オブジェクトの更新
	ReticleUpdate();

	//ダメージリアクション処理
	DamageReaction();

	//チャージ処理
	Charge();

	//キャラクター攻撃処理
	Attack();
}

void Player::UpdateDodge()
{
	//キャラクターの移動処理
	DodgeMove();
	
	//移動限界の座標を超えないようにする処理
	ClampMove();

	//回転
	worldTransform_.rotation_ = LerpAngle(worldTransform_.rotation_, Vector3{ 0.0f, 0.0f, 0.0f }, 0.3f);

	BaseCharacter::Update();

	//照準オブジェクトの更新
	ReticleUpdate();

	//ダメージリアクション処理
	DamageReaction();

	//チャージ処理
	Charge();

}

void Player::UpdateGameOver()
{
	if (isGameOverEnd_)
	{
		return;
	}
	//乱数での移動量の設定
	const float kMoveRange = 0.1f;

	Random* random = Random::GetInstance();

	characterWorldTransform_.translation_ = random->GetVector3(-kMoveRange, kMoveRange);

	EffectManager::GetInstance()->SpawnEffect(EffectType::kHit02, characterWorldTransform_.GetWorldPosition(), 2);

	gameOverTimer_ += 1.0f / 60.0f;

	if (gameOverTimer_ >= 1.5f)
	{
		isGameOverEnd_ = true;
		characterWorldTransform_.scale_ = { 0.0f, 0.0f, 0.0f };
		EffectManager::GetInstance()->SpawnEffect(EffectType::kPlayerEnd01, characterWorldTransform_.GetWorldPosition(), 100);
	}

	BaseCharacter::Update();
}

void Player::UpdateGameClear()
{

	worldTransform_.rotation_ = LerpAngle(worldTransform_.rotation_, Vector3{ 0.0f, 0.0f, 0.0f }, 0.1f);

	//移動入力の処理
	HandleMoveInput();

	//キャラクターの傾きの更新
	UpdateTilt();

	//キャラクターの移動処理
	Move();

	//前に進む
	const float kMoveSpeed = 0.3f;
	characterWorldTransform_.translation_.z += kMoveSpeed;

	BaseCharacter::Update();

}

void Player::AfterStartComplete()
{
	characterWorldTransform_.scale_ = { 1.0f, 1.0f, 1.0f };
	characterWorldTransform_.rotation_ = { 0.0f, 0.0f, 0.0f };

	EffectManager::GetInstance()->SpawnEffect(EffectType::kPlayerStart03, GetWorldPosition(), 100);
	
	for (int i = 0; i < kMaxHitPoint_; i++)
	{
		heartSprites_[i]->SetAlpha(1.0f); //表示する

		heartEmptySprites_[i]->SetAlpha(1.0f); //表示する
	}

	//レティクルを表示する
	reticleController_->OnStartCinematicEnd();
}

void Player::Rotate()
{
	//向く方向の計算
	Vector3 targetRotation = RotateCommon();
	
	worldTransform_.rotation_ = LerpAngle(worldTransform_.rotation_, targetRotation, 0.1f);
}

Vector3 Player::RotateCommon()
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

	return targetRotation;
}

void Player::DodgeRotate(float rotateSpeed)
{
	characterWorldTransform_.rotation_.z += rotateSpeed; //回転量
}

void Player::HeartUpdate()
{
#ifdef _DEBUG

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const std::string& groupName = JsonKey::Player::kGroupName;
	Vector2 heartPosition = globalVariables->GetVector2Value(groupName, JsonKey::Player::kHeartPosition);
	Vector2 heartSize = globalVariables->GetVector2Value(groupName, JsonKey::Player::kHeartSize);
	float heartSpacing = globalVariables->GetFloatValue(groupName, JsonKey::Player::kHeartSpacing);

	for (int i = 0; i < kMaxHitPoint_; i++)
	{
		heartSprites_[i]->SetPosition(Vector2(heartPosition.x + i * heartSpacing, heartPosition.y)); //位置を設定
		heartSprites_[i]->SetSize(heartSize); //サイズを設定

		heartEmptySprites_[i]->SetPosition(Vector2(heartPosition.x + i * heartSpacing, heartPosition.y)); //位置を設定
		heartEmptySprites_[i]->SetSize(heartSize); //サイズを設定
	}
#endif // _DEBUG

}

void Player::ReticleUpdate()
{
	reticleController_->Update(railCamera_);
}

void Player::Attack() {


	if (shotIntervalTimer_ > 0.0f)
	{
		shotIntervalTimer_ -= 1.0f / 60.0f;
		return;
	}
	//弾発射処理
	if (input_->PushKey(DIK_SPACE) || input_->PushButton(XINPUT_GAMEPAD_RIGHT_SHOULDER))
	{
		//弾の方向
		Vector3 bulletDirection = Normalize(direction_);
		
		BaseEnemy* lockOnTarget = reticleController_->GetTargetEnemy();

		//弾を生成し、初期化
		if (isChargeMax_)
		{
			//チャージ最大なら強力な弾を撃つ
			playerBulletManager_->AddPlayerBullet(GetWorldPosition(), bulletDirection, PlayerBulletType::kCharge, lockOnTarget);

			const float kChargeBulletShotInterval = 0.5f;
			shotIntervalTimer_ = kChargeBulletShotInterval; //チャージ弾の発射間隔

			//チャージをリセット
			ChargeReset();
			return;
		}

		playerBulletManager_->AddPlayerBullet(GetWorldPosition(), bulletDirection, PlayerBulletType::kNormal, lockOnTarget);

		const float kNormalBulletShotInterval = 0.2f;
		shotIntervalTimer_ = kNormalBulletShotInterval; //通常弾の発射間隔

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

void Player::DamageReactionInitialize()
{
	const float kDamageReactionFrame = 0.2f;
	damageReactionTimer_ = kDamageReactionFrame;
}

void Player::DamageReaction()
{
	if (damageReactionTimer_ < 0.0f)
	{
		// ダメージリアクション終了
		characterWorldTransform_.translation_ = { 0.0f, 0.0f, 0.0f };
		return;
	}

	damageReactionTimer_ -= 1.0f / 60.0f;

	//乱数での移動量の設定
	const float kMoveRange = 0.2f;

	characterWorldTransform_.translation_ = Random::GetInstance()->GetVector3(-kMoveRange, kMoveRange);
}
