#include "CollisionManager.h"
#include "Collision.h"
#include "ModelPlatform.h"

void CollisionManager::Initialize()
{
	uint32_t textureHandle = TextureManager::GetInstance()->Load("./Resources/white.png");
	model_ = ModelPlatform::GetInstance()->CreateSphere(textureHandle, "Collider");

	objects_ = std::make_unique<InstancingObjects>();
	objects_->Initialize(model_.get(), 255);

	// グループを追加
	globalVariables_->CreateGroup(groupName_);
	globalVariables_->AddItem(groupName_, "isDrawCollider", isDrawCollider_);
	isDrawCollider_ = globalVariables_->GetBoolValue(groupName_, "isDrawCollider");
}

void CollisionManager::Update()
{
	isDrawCollider_ = globalVariables_->GetBoolValue(groupName_, "isDrawCollider");

}

void CollisionManager::Draw(Camera* camera)
{
	// 非表示なら抜ける
	if (!isDrawCollider_) {
		return;
	}

	objects_->PreUpdate();
	for (Collider* collider : colliders_) {
		// コライダーのワールドトランスフォームを取得
		WorldTransform worldTransform = collider->GetWorldTransform();

		// ワールドトランスフォームをインスタンシングオブジェクトに追加
		objects_->WorldTransformUpdate(worldTransform);
	}
	objects_->CameraUpdate(camera);
	objects_->Draw();
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
	CollisionTypeIdDef typeID = colliderA->GetTypeID();
	
	switch (typeID)
	{
	case CollisionTypeIdDef::kDefault:
		break;
	case CollisionTypeIdDef::kPlayer:
		CheckPlayerCollisions(colliderA, colliderB);
		break;
	case CollisionTypeIdDef::kEnemy:
		CheckEnemyCollisions(colliderA, colliderB);
		break;
	case CollisionTypeIdDef::kPlayerBullet:
		CheckPlayerBulletCollisions(colliderA, colliderB);
		break;
	case CollisionTypeIdDef::kEnemyBullet:
		CheckEnemyBulletCollisions(colliderA, colliderB);
		break;
	default:
		break;
	}
}

void CollisionManager::CheckPlayerCollisions(Collider* player, Collider* colliderB)
{
	CollisionTypeIdDef typeID = colliderB->GetTypeID();

	switch (typeID)
	{
	case CollisionTypeIdDef::kEnemyBullet:
		//球と球の交差判定
		if (IsCollision(Sphere{ player->GetCenterPosition(), player->GetRadius() }, Sphere{ colliderB->GetCenterPosition(), colliderB->GetRadius() })) {
			// プレイヤーの衝突時
			player->OnCollision(colliderB);
			colliderB->OnCollision(player);
		}
		return;
	default:
		return;
	}
	
}

void CollisionManager::CheckEnemyCollisions(Collider* enemy, Collider* colliderB)
{
	CollisionTypeIdDef typeID = colliderB->GetTypeID();

	switch (typeID)
	{
	case CollisionTypeIdDef::kPlayerBullet:
		//球と球の交差判定
		if (IsCollision(Sphere{ enemy->GetCenterPosition(), enemy->GetRadius() }, Sphere{ colliderB->GetCenterPosition(), colliderB->GetRadius() })) {
			// 敵の衝突時
			enemy->OnCollision(colliderB);
			colliderB->OnCollision(enemy);
		}
		return;
	default:
		return;
	}

}

void CollisionManager::CheckPlayerBulletCollisions(Collider* playerBullet, Collider* colliderB)
{
	CollisionTypeIdDef typeID = colliderB->GetTypeID();

	switch (typeID)
	{
	case CollisionTypeIdDef::kEnemy:
	case CollisionTypeIdDef::kEnemyBullet:
		//球と球の交差判定
		if (IsCollision(Sphere{ playerBullet->GetCenterPosition(), playerBullet->GetRadius() }, Sphere{ colliderB->GetCenterPosition(), colliderB->GetRadius() })) {
			// プレイヤー弾の衝突時
			playerBullet->OnCollision(colliderB);
			colliderB->OnCollision(playerBullet);
		}
		return;
	default:
		return;
	}
}

void CollisionManager::CheckEnemyBulletCollisions(Collider* enemyBullet, Collider* colliderB)
{
	CollisionTypeIdDef typeID = colliderB->GetTypeID();

	switch (typeID)
	{
	case CollisionTypeIdDef::kPlayer:
	case CollisionTypeIdDef::kPlayerBullet:
		//球と球の交差判定
		if (IsCollision(Sphere{ enemyBullet->GetCenterPosition(), enemyBullet->GetRadius() }, Sphere{ colliderB->GetCenterPosition(), colliderB->GetRadius() })) {
			// 敵弾の衝突時
			enemyBullet->OnCollision(colliderB);
			colliderB->OnCollision(enemyBullet);
		}
		return;

	default:
		return;
	}
}
