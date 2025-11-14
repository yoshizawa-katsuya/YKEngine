#include "Player.h"
#include "Rigid3dObject.h"
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

void Player::Initialize(BaseModel* model, Matrix4x4* viewPortMatrix, WorldTransform* parent, uint32_t heartTextureHandle, uint32_t heartEmptyTexturehandle)
{

	BaseCharacter::Initialize(model);
	Collider::SetTypeID(CollisionTypeIdDef::kPlayer);

	input_ = Input::GetInstance();

	worldTransform_.parent_ = parent;
	
	BaseCharacter::Update();

	//�J�n���̃A�j���[�V�����ݒ�
	startAnime_ = std::make_unique<SRTAnimator>();
	startAnime_->SetAnimation({ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, 1.5f);

	reticleController_ = std::make_unique<ReticleController>();
	reticleController_->Initialize(viewPortMatrix);

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
	case Player::Phase::Main:
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

	//HP�̕\��
	for (int i = 0; i < maxHitPoint_; i++)
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

	//�e�̉�]��l�����������x�N�g���̌v�Z
	Matrix4x4 parentMat = MakeRotateMatrix(worldTransform_.parent_->rotation_);
	Matrix4x4 invParentMat = Inverse(parentMat);

	//�e�̉�]��ł�����
	Vector3 localDirection = TransformNormal(direction_, invParentMat);
	Vector3 targetRotation = TransformHelpers::FaceToVelocityDirection(worldTransform_.rotation_, localDirection);
	
	worldTransform_.rotation_ = targetRotation;
	BaseCharacter::Update();
}

void Player::HUDInitialize(uint32_t heartTextureHandle, uint32_t heartEmptyTexturehandle)
{
	heratSprites_.resize(maxHitPoint_);
	heratEmptySprites_.resize(maxHitPoint_);

	for (int i = 0; i < maxHitPoint_; i++)
	{
		heratSprites_[i] = std::make_unique<Sprite>();
		heratSprites_[i]->Initialize(heartTextureHandle);
		heratSprites_[i]->SetPosition(Vector2(50.0f + i * 50.0f, 50.0f)); //�ʒu��ݒ�
		heratSprites_[i]->SetSize(Vector2(50.0f, 50.0f)); //�T�C�Y��ݒ�

		heratEmptySprites_[i] = std::make_unique<Sprite>();
		heratEmptySprites_[i]->Initialize(heartEmptyTexturehandle);
		heratEmptySprites_[i]->SetPosition(Vector2(50.0f + i * 50.0f, 50.0f)); //�ʒu��ݒ�
		heratEmptySprites_[i]->SetSize(Vector2(50.0f, 50.0f)); //�T�C�Y��ݒ�
	}
}

void Player::HandleMoveInput()
{
	//�L�����N�^�[�̈ړ��x�N�g��
	Vector3 move = { 0, 0, 0 };

	move.x = input_->GetLeftStickX();
	move.y = input_->GetLeftStickY();

	if (move.x == 0 && move.y == 0) {
		//���X�e�B�b�N���j���[�g�����Ȃ�A�L�[�{�[�h���͂�m�F
		//�����������ňړ��x�N�g����ύX(���E)
		if (input_->PushKey(DIK_A)) {
			move.x = -1.0f;
		}
		else if (input_->PushKey(DIK_D)) {
			move.x = 1.0f;
		}

		// �����������ňړ��x�N�g����ύX(�㉺)
		if (input_->PushKey(DIK_S)) {
			move.y = -1.0f;
		}
		else if (input_->PushKey(DIK_W)) {
			move.y = 1.0f;
		}

		move = Normalize(move); //�ړ��x�N�g���̐��K��

	}

	//�L�����N�^�[�̈ړ�����
	const float kCharacterSpeed = 0.2f;
	//�ړ��x�N�g���̑����̓K�p
	move *= kCharacterSpeed;

	//���W�ړ�(�x�N�g���̉��Z)
	worldTransform_.translation_ += move;
}

void Player::UpdateStart()
{
	characterWorldTransform_.scale_ = startAnime_->Update();
	BaseCharacter::Update();
	
	EffectManager::GetInstance()->SpawnEffect(EffectType::PlayerStartEffect01, GetWorldPosition());

	if (startAnime_->GetIsEnd())
	{
		phase_ = Phase::Main;
	}
}

void Player::UpdateMain(Camera* railCamera)
{
	HandleMoveInput();

	//�ړ����E���W
	const float kMoveLimitX = 8.9f;
	const float kMoveLimitY = 4.8f;

	//�͈͂𒴂��Ȃ�����
	worldTransform_.translation_.x = std::clamp(worldTransform_.translation_.x, -kMoveLimitX, kMoveLimitX);
	worldTransform_.translation_.y = std::clamp(worldTransform_.translation_.y, -kMoveLimitY, kMoveLimitY);

	//��]
	Rotate();

	BaseCharacter::Update();

	//�Ə��I�u�W�F�N�g�̍X�V
	ReticleUpdate(railCamera);

	//�`���[�W����
	Charge();

	//�L�����N�^�[�U������
	Attack();
}

void Player::UpdateGameOver()
{
	if (isGameOverEnd_)
	{
		return;
	}
	//�����ł̈ړ��ʂ̐ݒ�
	const float kMoveRange = 0.1f;

	std::uniform_real_distribution<float> distribution(-kMoveRange, kMoveRange);

	//���������G���W���ւ̃|�C���^
	std::mt19937* randomEngine = Random::GetInstance()->GetRandomEnginePtr();

	characterWorldTransform_.translation_ = { distribution(*randomEngine), distribution(*randomEngine), distribution(*randomEngine) };

	EffectManager::GetInstance()->SpawnEffect(EffectType::PlayerEndEffect01, characterWorldTransform_.GetWorldPosition());

	gameOverTimer_ += 1.0f / 60.0f;

	if (gameOverTimer_ >= 1.5f)
	{
		isGameOverEnd_ = true;
		characterWorldTransform_.scale_ = { 0.0f, 0.0f, 0.0f };
		EffectManager::GetInstance()->SpawnEffect(EffectType::PlayerEndEffect01, characterWorldTransform_.GetWorldPosition(), 100);
	}

	BaseCharacter::Update();
}

void Player::Rotate()
{
	//���������̌v�Z
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
	
	//�e�̉�]��l�����������x�N�g���̌v�Z
	Matrix4x4 parentMat = MakeRotateMatrix(worldTransform_.parent_->rotation_);
	Matrix4x4 invParentMat = Inverse(parentMat);

	//�e�̉�]��ł�����
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
			playerBulletManager_->AddPlayerBullet(GetWorldPosition(), velocity, PlayerBulletType::Charge);

			//�`���[�W����Z�b�g
			ChargeReset();
			return;
		}

		playerBulletManager_->AddPlayerBullet(GetWorldPosition(), velocity, PlayerBulletType::Normal);
		//�`���[�W����Z�b�g
		ChargeReset();

	}

}

void Player::Charge()
{
	//�`���[�W���ő�Ȃ珈�����Ȃ�
	if (isChargeMax_)
	{
		return;
	}

	//�`���[�W
	chargeTime_ += 1.0f / 60.0f;
	if (chargeTime_ >= maxChargeTime_) 
	{
		chargeTime_ = maxChargeTime_;
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
