#include "BasePlayer.h"
#include "TransformHelpers.h"
#include "CollisionManager.h"

using namespace YKEngine;

void BasePlayer::Initialize(BaseModel* model, int32_t* hp)
{
	// 球コライダーの初期化
	SphereCollider::Initialize();

	// 球コライダーの半径を設定
	SetRadius(0.5f);

	//オブジェクトの生成
	object_ = std::make_unique<My3dObject>();
	object_->Initialize(model);

	//体力を設定
	hp_ = hp;


	input_ = Input::GetInstance();

	//Colliderの種別IDをプレイヤーに設定
	BaseCollider::SetTypeID(CollisionTypeIdDef::kPlayer);

	//衝突マネージャーに登録
	CollisionManager::GetInstance()->AddSphereCollider(this);
}

void BasePlayer::Update()
{

	// 移動
	Move();

	// 回転
	Rotate();

	// ワールド変換行列の更新
	SphereCollider::Update();
	object_->WorldTransformUpdate(worldTransform_);

}

void BasePlayer::Draw(Camera* camera)
{

	object_->CameraUpdate(camera);
	object_->Draw();

}

void BasePlayer::OnCollision(BaseCollider* other)
{
	if (other->GetTypeID() == CollisionTypeIdDef::kEnemy) {
		// 敵と衝突した場合、体力を減らす
		if (hp_ && *hp_ > 0) 
		{
			(*hp_)--;
		}
	}
}

void BasePlayer::Rotate()
{
	// 自分ではない方の自機を向く
	Vector3 direction = otherPlayerWorldTransform_->GetWorldPosition() - worldTransform_.GetWorldPosition();	// 自分ではない方の自機への方向ベクトル
	Vector3 targetRotate = TransformHelpers::FaceToVelocityDirection(worldTransform_.rotation_, direction);	// 方向ベクトルから回転角を取得

	worldTransform_.rotation_ = TransformHelpers::NormalizeAngle(targetRotate);
}
