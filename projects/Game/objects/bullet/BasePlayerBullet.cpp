#include "BasePlayerBullet.h"
#include "cassert"
#include "TextureManager.h"
#include "Vector3.h"
#include "manager/EffectManager.h"

using namespace YKEngine;

void BasePlayerBullet::Initialize(BaseModel* model, const Vector3& position, const Vector3& velocity) 
{

	BaseBullet::Initialize(model, position, velocity);
	Collider::SetTypeID(CollisionTypeIdDef::kPlayerBullet);

}

void BasePlayerBullet::Update()
{
	// 回転
	const float kRotationSpeed = 0.3f;
	characterWorldTransform_.rotation_.z += kRotationSpeed;

	BaseBullet::Update();

	EffectManager::GetInstance()->SpawnEffect(EffectType::kPlayerBulletTrac01, worldTransform_.GetWorldPosition());
}

void BasePlayerBullet::OnCollision(Collider* other)
{
	CollisionTypeIdDef typeID = other->GetTypeID();

	if (typeID == CollisionTypeIdDef::kEnemy || typeID == CollisionTypeIdDef::kTackleEnemy)
	{
		isDead_ = true;
	}
}
