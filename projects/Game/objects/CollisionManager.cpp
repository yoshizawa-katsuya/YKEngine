#include "CollisionManager.h"
#include "Collision.h"

void CollisionManager::Initialize()
{
}

void CollisionManager::UpdateWorldTransform()
{
}

void CollisionManager::Reset()
{
	//リストを空っぽにする
	colliders_.clear();
}

void CollisionManager::CheckAllCollisions()
{
	//リスト内のペアを総当たり
	std::list<Collider*>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); ++itrA) {
		Collider* colliderA = *itrA;

		//イテレータBはイテレータAの次の要素から回す(重複判定を回避)
		std::list<Collider*>::iterator itrB = itrA;
		itrB++;

		for (; itrB != colliders_.end(); ++itrB) {
			Collider* colliderB = *itrB;

			//ペアの当たり判定
			CheckColliderPair(colliderA, colliderB);
		}
	}
}

void CollisionManager::AddCollider(Collider* collider)
{
	colliders_.push_back(collider);
}

void CollisionManager::CheckColliderPair(Collider* colliderA, Collider* colliderB)
{

	//球と球の交差判定
	if (IsCollision(Sphere{colliderA->GetCenterPosition(), colliderA->GetRadius()}, Sphere{ colliderB->GetCenterPosition(), colliderB->GetRadius() })) {

		//コライダーAの衝突時コールバックを呼び出す
		colliderA->OnCollision(colliderB);
		// コライダーBの衝突時コールバックを呼び出す
		colliderB->OnCollision(colliderA);

	}
}
