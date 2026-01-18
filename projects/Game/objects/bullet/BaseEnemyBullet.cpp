#include "BaseEnemyBullet.h"
#include "cassert"
#include "TextureManager.h"
#include "Vector3.h"
#include "GlobalVariables.h"
#include "JsonKeys.h"

using namespace YKEngine;

void BaseEnemyBullet::Initialize(BaseModel* model, const Vector3& position, const Vector3& velocity, Player* target, float speed)
{
	speed_ = speed;
	BaseBullet::Initialize(model, position, velocity);
	Collider::SetTypeID(CollisionTypeIdDef::kEnemyBullet);

}

void BaseEnemyBullet::Update(Camera* railCamera)
{
	//弾の回転
	const float kRotateSpeed = GlobalVariables::GetInstance()->GetFloatValue(JsonKey::Bullet::Enemy::kGroupName, JsonKey::Bullet::kRotateSpeed);
	characterWorldTransform_.rotation_.z += kRotateSpeed;

	BaseBullet::Update();

	//画面外に出たら消す
	if (!IsVisible(railCamera))
	{
		isDead_ = true;
	}
}

void BaseEnemyBullet::OnCollision(Collider* other)
{
	CollisionTypeIdDef typeID = other->GetTypeID();

	if (typeID == CollisionTypeIdDef::kPlayer)
	{
		isDead_ = true;
	}
}
