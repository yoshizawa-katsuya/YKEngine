#include "EnemyBullet.h"
#include "cassert"
#include "TextureManager.h"
#include "Vector3.h"

void EnemyBullet::Initialize(BaseModel* model, const Vector3& position, const Vector3& velocity, uint32_t textureHandle) {

	BaseBullet::Initialize(model, position, velocity, textureHandle);
	Collider::SetTypeID(CollisionTypeIdDef::kEnemyBullet);

}

void EnemyBullet::OnCollision(Collider* other)
{
	CollisionTypeIdDef typeID = other->GetTypeID();

	if (typeID == CollisionTypeIdDef::kPlayer || typeID == CollisionTypeIdDef::kPlayerBullet)
	{
		isDead_ = true;
	}
}


