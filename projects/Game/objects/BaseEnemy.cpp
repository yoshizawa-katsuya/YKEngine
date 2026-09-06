#include "BaseEnemy.h"
#include "CollisionManager.h"

#include "ModelPlatform.h"

#ifdef USE_IMGUI
#include "imgui/imgui.h"
#endif // USE_IMGUI

#include <numbers>

using namespace YKEngine;

void BaseEnemy::Initialize()
{
	// 球コライダーの初期化
	SphereCollider::Initialize();

	//モデルの生成
	ModelPlatform* modelPlatform = ModelPlatform::GetInstance();
	std::shared_ptr<BaseModel> modelPlayer = modelPlatform->CreateRigidModel("./resources/Player", "Player.obj");

	object_ = std::make_unique<My3dObject>();
	object_->Initialize(modelPlayer.get());

	worldTransform_.translation_ = { 0.0f, 0.0f, 30.0f };
	velocity_ = { 0.0f, 0.0f, -kMaxSpeed_ };

	//Colliderの種別IDをプレイヤーに設定
	BaseCollider::SetTypeID(CollisionTypeIdDef::kEnemy);

	//衝突マネージャーに登録
	CollisionManager::GetInstance()->AddSphereCollider(this);
}

void BaseEnemy::Update()
{
#ifdef USE_IMGUI

	ImGui::Begin("Enemy");
	if (ImGui::TreeNode("Model")) {
		ImGui::DragFloat3("translate", &worldTransform_.translation_.x, 0.01f);
		ImGui::DragFloat3("rotate", &worldTransform_.rotation_.x, 0.01f);
		ImGui::DragFloat3("scale", &worldTransform_.scale_.x, 0.01f);

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
		isAlive_ = false; // 敵を死亡状態にする
	}
}
