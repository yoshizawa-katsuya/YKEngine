#include "BaseEnemy.h"
#include "CollisionManager.h"
#include "Laser.h"
#include "ModelPlatform.h"
#include "EffectManager.h"
#include "AudioManager.h"

#ifdef USE_IMGUI
#include "imgui/imgui.h"
#endif // USE_IMGUI

#include <numbers>

using namespace YKEngine;

void BaseEnemy::Initialize(const EnemySpawnData& spawnData, const BasePlayer* target1, const BasePlayer* target2)
{
	// 球コライダーの初期化
	SphereCollider::Initialize();

	//モデルの生成
	ModelPlatform* modelPlatform = ModelPlatform::GetInstance();
	std::shared_ptr<BaseModel> modelPlayer = modelPlatform->CreateRigidModel("./resources/baseEnemy", "base_enemy.obj", color_);

	object_ = std::make_unique<My3dObject>();
	object_->Initialize(modelPlayer.get());

	//worldTransform_.translation_ = { 0.0f, 0.0f, 30.0f };
	worldTransform_.translation_ = spawnData.transform.translation;
	worldTransform_.UpdateMatrix();
	velocity_ = { 0.0f, 0.0f, -maxSpeed_ };

	SetTargets(target1, target2);

	//Colliderの種別IDをプレイヤーに設定
	BaseCollider::SetTypeID(CollisionTypeIdDef::kEnemy);

	//衝突マネージャーに登録
	CollisionManager::GetInstance()->AddSphereCollider(this);

	object_->SetColor(color_);
}

void BaseEnemy::Update()
{
#ifdef USE_IMGUI

	ImGui::Begin("Enemy");
	if (ImGui::TreeNode("Model")) {
		ImGui::DragFloat3("translate", &worldTransform_.translation_.x, 0.01f);
		ImGui::DragFloat3("rotate", &worldTransform_.rotation_.x, 0.01f);
		ImGui::DragFloat3("scale", &worldTransform_.scale_.x, 0.01f);
		ImGui::DragFloat3("color", &color_.x, 0.01f);

		ImGui::TreePop();
	}
	ImGui::End();

#endif // USE_IMGUI	

	Move();

	worldTransform_.translation_ += velocity_;

	SphereCollider::Update();
	object_->WorldTransformUpdate(worldTransform_);
}

void BaseEnemy::Draw(Camera* camera)
{
	object_->CameraUpdate(camera);
	object_->Draw();
}

void BaseEnemy::SetTargets(const BasePlayer* target1, const BasePlayer* target2)
{
	target1_ = target1;
	target2_ = target2;
}

bool BaseEnemy::IsWithinAngle(const YKEngine::Vector3& position, float angle)
{
	// ターゲットとの内積を計算
	float dot = GetDotProduct(position);

	// 角度の閾値を計算
	float threshold = cosf(angle * (std::numbers::pi_v<float> / 180.0f));

	if (dot >= threshold)
	{
		return true; // 角度内
	}
	return false; // 角度外
}

float BaseEnemy::GetDotProduct(const YKEngine::Vector3& position)
{
	// 速度を正規化
	Vector3 direction = Normalize(velocity_);

	// ターゲットの方向ベクトルを計算
	Vector3 toTarget = position - worldTransform_.translation_;

	// 正規化
	toTarget = Normalize(toTarget);

	// 2つのベクトルの内積を計算
	return Dot(direction, toTarget);
}

void BaseEnemy::OnCollision(BaseCollider* other)
{
	if (other->GetTypeID() == CollisionTypeIdDef::kLaser)
	{
		// レーザーと衝突した場合の処理
		hp_ -= dynamic_cast<Laser*>(other)->GetDamage();
		EffectManager::GetInstance()->SpawnEffect(EffectType::kHit01, worldTransform_.GetWorldPosition());
		AudioManager::GetInstance()->PlaySE(SEType::kDamage02);

		if (hp_ <= 0)
		{
			isAlive_ = false; // 敵を死亡状態にする
			EffectManager::GetInstance()->SpawnEffect(EffectType::kEnemyEnd01, worldTransform_.GetWorldPosition());
			AudioManager::GetInstance()->PlaySE(SEType::kDeath01);
		}
	}
	else if (other->GetTypeID() == CollisionTypeIdDef::kPlayer)
	{
		EffectManager::GetInstance()->SpawnEffect(EffectType::kHit01, worldTransform_.GetWorldPosition());

		// プレイヤーと衝突した場合の処理
		isAlive_ = false; // 敵を死亡状態にする
	}
}
