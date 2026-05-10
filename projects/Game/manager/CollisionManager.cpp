#include "CollisionManager.h"
#include "Collision.h"
#include "ModelPlatform.h"

using namespace YKEngine;

std::unique_ptr<CollisionManager> CollisionManager::instance_ = nullptr;

CollisionManager* CollisionManager::GetInstance()
{
	if (instance_ == nullptr)
	{
		instance_ = std::make_unique<CollisionManager>(ConstructorKey());
	}
	return instance_.get();
}

void CollisionManager::Finalize()
{
	//リソースリークチェックのため、明示的にインスタンスを破棄する
	instance_.reset();
}

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
	for (BaseCollider* collider : sphereColliders_) 
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
	sphereColliders_.clear();
	obbColliders_.clear();
}

void CollisionManager::CheckAllCollisions()
{
	//全ての衝突判定を行う
	CheckSphereCollisions();
	CheckSphereOBBCollisions();
}

void CollisionManager::AddSphereCollider(SphereCollider* collider)
{
	sphereColliders_.push_back(collider);
}

void CollisionManager::AddOBBCollider(OBBCollider* collider)
{
	obbColliders_.push_back(collider);
}

void CollisionManager::RemoveSphereCollider(SphereCollider* collider)
{
	//リストから削除
	sphereColliders_.erase(std::remove(sphereColliders_.begin(), sphereColliders_.end(), collider), sphereColliders_.end());
}

void CollisionManager::RemoveOBBCollider(OBBCollider* collider)
{
	//リストから削除
	obbColliders_.erase(std::remove(obbColliders_.begin(), obbColliders_.end(), collider), obbColliders_.end());
}

void CollisionManager::CheckSphereCollisions()
{
	//リスト内のペアを総当たり
	std::vector<SphereCollider*>::iterator itrA = sphereColliders_.begin();
	for (; itrA != sphereColliders_.end(); ++itrA)
	{
		SphereCollider* colliderA = *itrA;

		//イテレータBはイテレータAの次の要素から回す(重複判定を回避)
		std::vector<SphereCollider*>::iterator itrB = itrA;
		itrB++;

		for (; itrB != sphereColliders_.end(); ++itrB)
		{
			SphereCollider* colliderB = *itrB;

			//ペアの当たり判定
			CheckSphereColliderPair(colliderA, colliderB);
		}
	}
}

void CollisionManager::CheckSphereOBBCollisions()
{
	for (SphereCollider* sphereCollider : sphereColliders_)
	{
		for (OBBCollider* obbCollider : obbColliders_)
		{
			//衝突ペアでなければ抜ける
			if (!IsCollisionPair(sphereCollider->GetTypeID(), obbCollider->GetTypeID()))
			{
				continue;
			}
			if (IsCollision(obbCollider->GetOBB(), Sphere{ sphereCollider->GetCenterPosition(), sphereCollider->GetRadius() }))
			{
				// 衝突時の処理
				sphereCollider->OnCollision(obbCollider);
				obbCollider->OnCollision(sphereCollider);
			}
		}
	}
}

void CollisionManager::CheckSphereColliderPair(SphereCollider* colliderA, SphereCollider* colliderB)
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

