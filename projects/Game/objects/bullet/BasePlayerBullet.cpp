#include "BasePlayerBullet.h"
#include "cassert"
#include "TextureManager.h"
#include "Vector3.h"

void BasePlayerBullet::Initialize(BaseModel* model, const Vector3& position, const Vector3& velocity, uint32_t textureHandle) 
{

	BaseBullet::Initialize(model, position, velocity, textureHandle);
	Collider::SetTypeID(CollisionTypeIdDef::kPlayerBullet);

}

void BasePlayerBullet::OnCollision(Collider* other)
{
	CollisionTypeIdDef typeID = other->GetTypeID();

	if (typeID == CollisionTypeIdDef::kEnemy || typeID == CollisionTypeIdDef::kTackleEnemy)
	{
		isDead_ = true;

		
	}
}
