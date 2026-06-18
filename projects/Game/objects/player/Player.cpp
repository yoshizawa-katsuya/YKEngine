#include "Player.h"
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
#include "manager/CollisionManager.h"
#include "manager/AudioManager.h"

#ifdef USE_IMGUI
#include "imgui/imgui.h"
#endif // USE_IMGUI

using namespace YKEngine;

void Player::Initialize(WorldTransform* parent)
{
	//オーディオ管理クラスの取得
	audioManager_ = AudioManager::GetInstance();

	//グローバル変数の登録
	globalVariables_ = GlobalVariables::GetInstance();
	const std::string& groupName = JsonKey::Player::kGroupName;
	globalVariables_->CreateGroup(groupName);
	globalVariables_->AddItem(groupName, JsonKey::Player::kDodgeSpeed, 0.6f);
	globalVariables_->AddItem(groupName, JsonKey::Player::kDodgeTime, 20.0f);
	globalVariables_->AddItem(groupName, JsonKey::Player::kSpeed, 0.2f);
	globalVariables_->AddItem(groupName, JsonKey::Player::kTiltQuantity, std::numbers::pi_v<float> / 8.0f);
	globalVariables_->AddItem(groupName, JsonKey::Player::kStartAnimeDuration, 1.5f);
	globalVariables_->AddItem(groupName, JsonKey::Player::kStartRotateAnimeQuantity, std::numbers::pi_v<float> * 4.0f);
	globalVariables_->AddItem(groupName, JsonKey::Player::kMoveLimitX, 8.9f);
	globalVariables_->AddItem(groupName, JsonKey::Player::kMoveLimitY, 4.8f);
	globalVariables_->AddItem(groupName, JsonKey::Player::kGameOverRandomMoveRange, 0.1f);
	globalVariables_->AddItem(groupName, JsonKey::Player::kGameOverDuration, 1.5f);
	globalVariables_->AddItem(groupName, JsonKey::Player::kDamageReactionDuration, 0.2f);
	globalVariables_->AddItem(groupName, JsonKey::Player::kDamageReactionMoveRange, 0.2f);
	globalVariables_->AddItem(groupName, JsonKey::Player::kTiltLerpFactor, 0.1f);
	globalVariables_->AddItem(groupName, JsonKey::Player::kDodgeRotateLerpFactor, 0.3f);
	globalVariables_->AddItem(groupName, JsonKey::Player::kGameClearRotateLerpFactor, 0.1f);
	globalVariables_->AddItem(groupName, JsonKey::Player::kRotateLerpFactor, 0.1f);
	globalVariables_->AddItem(groupName, JsonKey::Player::kNormalBulletShotInterval, 0.2f);
	globalVariables_->AddItem(groupName, JsonKey::Player::kChargeBulletShotInterval, 0.5f);

	//モデルとコライダーの初期化
	SkinCharacter::Initialize(ModelPlatform::GetInstance()->CreateSkinModel("./Resources/player", "Player.gltf").get());
	animation_ = std::make_unique<Animation>();
	animation_->LoadAnimationFile("./Resources/player/", "Player.gltf");
	BaseCollider::SetTypeID(CollisionTypeIdDef::kPlayer);

	//アニメーションの適用
	object_->AnimationUpdate(animation_.get());

	input_ = Input::GetInstance();

	worldTransform_.parent_ = parent;
	
	SkinCharacter::Update();

	//開始時のアニメーション設定
	const float kAnimeDuration = globalVariables_->GetFloatValue(groupName, JsonKey::Player::kStartAnimeDuration);
	startAnime_ = std::make_unique<SRTAnimator>();
	startAnime_->SetAnimation({ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, kAnimeDuration);

	const float kRotateQuantity = globalVariables_->GetFloatValue(groupName, JsonKey::Player::kStartRotateAnimeQuantity);
	startRotateAnime_ = std::make_unique<SRTAnimator>();
	startRotateAnime_->SetAnimation({ 0.0f, 0.0f, 0.0f }, { 0.0f, kRotateQuantity, 0.0f }, kAnimeDuration);

	reticleController_ = std::make_unique<ReticleController>();
	reticleController_->Initialize();

	HUDInitialize();

	stateMachine_ = std::make_unique<StateMachine<PlayerStateContext>>();
	stateMachine_->Start(this);
	stateMachine_->ChangeState<PlayerStartState>();

	//衝突マネージャーに登録
	CollisionManager::GetInstance()->AddSphereCollider(this);
}

void Player::Update()
{


#ifdef USE_IMGUI

	ImGui::Begin("Player");
	if (ImGui::TreeNode("Object")) {
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

void Player::OnCollision(BaseCollider* other)
{
	CollisionTypeIdDef typeId = other->GetTypeID();

	if (typeId == CollisionTypeIdDef::kEnemyBullet || typeId == CollisionTypeIdDef::kTackleEnemy)
	{
		hitPoint_--;
		//ダメージリアクション開始
		DamageReactionInitialize();

		EffectManager::GetInstance()->SpawnEffect(EffectType::kHit01, worldTransform_.GetWorldPosition());
		EffectManager::GetInstance()->SpawnEffect(EffectType::kHit02, worldTransform_.GetWorldPosition(), 10);

		//ダメージSE再生
		audioManager_->PlaySE(SEType::kDamage01);

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

	SkinCharacter::Update();
}

void Player::HUDInitialize()
{
	//ハートの表示位置、サイズ、間隔をグローバル変数に登録
	const std::string& groupName = JsonKey::Player::kGroupName;
	globalVariables_->AddItem(groupName, JsonKey::Player::kHeartPosition, Vector2(50.0f, 30.0f));
	globalVariables_->AddItem(groupName, JsonKey::Player::kHeartSize, Vector2(50.0f, 50.0f));
	globalVariables_->AddItem(groupName, JsonKey::Player::kHeartSpacing, 50.0f);

	//ハートのテクスチャ読み込み
	uint32_t heartTextureHandle = TextureManager::GetInstance()->Load("./Resources/heart.png");
	uint32_t heartEmptyTexturehandle = TextureManager::GetInstance()->Load("./Resources/heartFrame.png");

	heartSprites_.resize(kMaxHitPoint_);
	heartEmptySprites_.resize(kMaxHitPoint_);

	Vector2 heartPosition = globalVariables_->GetVector2Value(groupName, JsonKey::Player::kHeartPosition);
	Vector2 heartSize = globalVariables_->GetVector2Value(groupName, JsonKey::Player::kHeartSize);
	float heartSpacing = globalVariables_->GetFloatValue(groupName, JsonKey::Player::kHeartSpacing);

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
	const float rotateQuantity = globalVariables_->GetFloatValue(JsonKey::Player::kGroupName, JsonKey::Player::kTiltQuantity);
	Vector3 targetRotation = { -move_.y * rotateQuantity, 0.0f, -move_.x * rotateQuantity };
	characterWorldTransform_.rotation_ = LerpAngle(characterWorldTransform_.rotation_, targetRotation, globalVariables_->GetFloatValue(JsonKey::Player::kGroupName, JsonKey::Player::kTiltLerpFactor));
}

void Player::Move()
{
	//移動ベクトルの速さの適用
	move_ *= globalVariables_->GetFloatValue(JsonKey::Player::kGroupName, JsonKey::Player::kSpeed);

	//座標移動(ベクトルの加算)
	worldTransform_.translation_ += move_;
}

void Player::DodgeMove()
{
	std::string groupName = JsonKey::Player::kGroupName;

	const float kDodgeSpeed = globalVariables_->GetFloatValue(groupName, JsonKey::Player::kDodgeSpeed);
	t_ += 1.0f / globalVariables_->GetFloatValue(groupName, JsonKey::Player::kDodgeTime);	//ドッジの時間経過

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
	const float kMoveLimitX = globalVariables_->GetFloatValue(JsonKey::Player::kGroupName, JsonKey::Player::kMoveLimitX);
	const float kMoveLimitY = globalVariables_->GetFloatValue(JsonKey::Player::kGroupName, JsonKey::Player::kMoveLimitY);

	//範囲を超えない処理
	worldTransform_.translation_.x = std::clamp(worldTransform_.translation_.x, -kMoveLimitX, kMoveLimitX);
	worldTransform_.translation_.y = std::clamp(worldTransform_.translation_.y, -kMoveLimitY, kMoveLimitY);
}

void Player::UpdateStart()
{
	characterWorldTransform_.scale_ = startAnime_->Update();
	characterWorldTransform_.rotation_ = startRotateAnime_->Update();
	SkinCharacter::Update();
	
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

	//アニメーションの更新
	UpdateAnimation();

	SkinCharacter::Update();

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
	worldTransform_.rotation_ = LerpAngle(worldTransform_.rotation_, Vector3{ 0.0f, 0.0f, 0.0f }, globalVariables_->GetFloatValue(JsonKey::Player::kGroupName, JsonKey::Player::kDodgeRotateLerpFactor));

	//アニメーションの更新
	UpdateAnimation();

	SkinCharacter::Update();

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
	const float kMoveRange = globalVariables_->GetFloatValue(JsonKey::Player::kGroupName, JsonKey::Player::kGameOverRandomMoveRange);

	Random* random = Random::GetInstance();

	characterWorldTransform_.translation_ = random->GetVector3(-kMoveRange, kMoveRange);

	EffectManager::GetInstance()->SpawnEffect(EffectType::kHit02, characterWorldTransform_.GetWorldPosition(), 2);

	gameOverTimer_ += DeltaTime_;

	if (gameOverTimer_ >= globalVariables_->GetFloatValue(JsonKey::Player::kGroupName, JsonKey::Player::kGameOverDuration))
	{
		isGameOverEnd_ = true;
		characterWorldTransform_.scale_ = { 0.0f, 0.0f, 0.0f };
		EffectManager::GetInstance()->SpawnEffect(EffectType::kHit02, characterWorldTransform_.GetWorldPosition(), 100);

		//死亡SE再生
		audioManager_->PlaySE(SEType::kDeath01);
	}

	SkinCharacter::Update();
}

void Player::UpdateGameClear()
{

	worldTransform_.rotation_ = LerpAngle(worldTransform_.rotation_, Vector3{ 0.0f, 0.0f, 0.0f }, globalVariables_->GetFloatValue(JsonKey::Player::kGroupName, JsonKey::Player::kGameClearRotateLerpFactor));

	//移動入力の処理
	HandleMoveInput();

	//キャラクターの傾きの更新
	UpdateTilt();

	//キャラクターの移動処理
	Move();

	//前に進む
	characterWorldTransform_.translation_.z += globalVariables_->GetFloatValue(JsonKey::Player::kGroupName, JsonKey::Player::kSpeed);

	//アニメーションの更新
	UpdateAnimation();

	SkinCharacter::Update();

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
	
	worldTransform_.rotation_ = LerpAngle(worldTransform_.rotation_, targetRotation, globalVariables_->GetFloatValue(JsonKey::Player::kGroupName, JsonKey::Player::kRotateLerpFactor));
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

void Player::DodgeRotate(SRTAnimator* rotateAnime)
{
	characterWorldTransform_.rotation_ = rotateAnime->Update();
}

void Player::HeartUpdate()
{
#ifdef _DEBUG

	const std::string& groupName = JsonKey::Player::kGroupName;
	Vector2 heartPosition = globalVariables_->GetVector2Value(groupName, JsonKey::Player::kHeartPosition);
	Vector2 heartSize = globalVariables_->GetVector2Value(groupName, JsonKey::Player::kHeartSize);
	float heartSpacing = globalVariables_->GetFloatValue(groupName, JsonKey::Player::kHeartSpacing);

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
		shotIntervalTimer_ -= DeltaTime_;
		return;
	}
	//弾発射処理
	if (input_->PushKey(DIK_SPACE) || input_->PushButton(XINPUT_GAMEPAD_RIGHT_SHOULDER) || input_->PushRT())
	{
		//弾の方向
		Vector3 bulletDirection = Normalize(direction_);
		
		BaseEnemy* lockOnTarget = reticleController_->GetTargetEnemy();

		//弾を生成し、初期化
		if (isChargeMax_)
		{
			//チャージ最大なら強力な弾を撃つ
			playerBulletManager_->AddPlayerBullet(GetWorldPosition(), bulletDirection, PlayerBulletType::kCharge, lockOnTarget);
			
			//チャージ弾の発射SE再生
			audioManager_->PlaySE(SEType::kShot02);

			//チャージ弾の発射間隔
			shotIntervalTimer_ = globalVariables_->GetFloatValue(JsonKey::Player::kGroupName, JsonKey::Player::kChargeBulletShotInterval);

			//チャージをリセット
			ChargeReset();
			return;
		}

		//通常弾の発射
		playerBulletManager_->AddPlayerBullet(GetWorldPosition(), bulletDirection, PlayerBulletType::kNormal, lockOnTarget);

		//通常弾の発射SE再生
		audioManager_->PlaySE(SEType::kShot01);

		//通常弾の発射間隔
		shotIntervalTimer_ = globalVariables_->GetFloatValue(JsonKey::Player::kGroupName, JsonKey::Player::kNormalBulletShotInterval);

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
	chargeTime_ += DeltaTime_;
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
	damageReactionTimer_ = globalVariables_->GetFloatValue(JsonKey::Player::kGroupName, JsonKey::Player::kDamageReactionDuration);
}

void Player::DamageReaction()
{
	if (damageReactionTimer_ < 0.0f)
	{
		// ダメージリアクション終了
		characterWorldTransform_.translation_ = { 0.0f, 0.0f, 0.0f };
		return;
	}

	damageReactionTimer_ -= DeltaTime_;

	//乱数での移動量の設定
	const float kMoveRange = globalVariables_->GetFloatValue(JsonKey::Player::kGroupName, JsonKey::Player::kDamageReactionMoveRange);

	characterWorldTransform_.translation_ = Random::GetInstance()->GetVector3(-kMoveRange, kMoveRange);
}

void Player::UpdateAnimation()
{
	animation_->Update();
	object_->AnimationUpdate(animation_.get());
}
