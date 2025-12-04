#include "CollisionManager.h"
#include "Collision.h"
#include "ModelPlatform.h"

void CollisionManager::Initialize()
{
	uint32_t textureHandle = TextureManager::GetInstance()->Load("./Resources/white.png");
	model_ = ModelPlatform::GetInstance()->CreateSphere(textureHandle, "Collider");

	// コライダー描画用オブジェクトの初期化
	objects_ = std::make_unique<InstancingObjects>();
	objects_->Initialize(model_.get(), 255);

	// グループを追加
	globalVariables_->CreateGroup(kGroupName_);
	globalVariables_->AddItem(kGroupName_, "isDrawCollider", isDrawCollider_);
	isDrawCollider_ = globalVariables_->GetBoolValue(kGroupName_, "isDrawCollider");
}

void CollisionManager::Update()
{
#ifdef _DEBUG

	isDrawCollider_ = globalVariables_->GetBoolValue(kGroupName_, "isDrawCollider");

#endif //DEBUG
}

void CollisionManager::Draw(Camera* camera)
{
	// 非表示なら抜ける
	if (!isDrawCollider_) 
	{
		return;
	}

	objects_->PreUpdate();
	for (Collider* collider : colliders_) 
	{
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
	for (; itrA != colliders_.end(); ++itrA) 
	{
		Collider* colliderA = *itrA;

		//イテレータBはイテレータAの次の要素から回す(重複判定を回避)
		std::list<Collider*>::iterator itrB = itrA;
		itrB++;

		for (; itrB != colliders_.end(); ++itrB)
		{
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
	//衝突ペアでなければ抜ける
	if (!IsCollisionPair(colliderA->GetTypeID(), colliderB->GetTypeID()))
	{
		return;
	}

	if (IsCollision(Sphere{ colliderA->GetCenterPosition(), colliderA->GetRadius() }, Sphere{ colliderB->GetCenterPosition(), colliderB->GetRadius() }))
	{
		// 衝突時の処理
		colliderA->OnCollision(colliderB);
		colliderB->OnCollision(colliderA);
	}

}

bool CollisionManager::IsCollisionPair(CollisionTypeIdDef typeA, CollisionTypeIdDef typeB)
{
	return kCollisionPairs_.contains({ typeA, typeB }) || kCollisionPairs_.contains({ typeB, typeA });
}

