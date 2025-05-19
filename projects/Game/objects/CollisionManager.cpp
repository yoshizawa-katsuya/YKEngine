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

	//球と球の交差判定
	if (IsCollision(Sphere{colliderA->GetCenterPosition(), colliderA->GetRadius()}, Sphere{ colliderB->GetCenterPosition(), colliderB->GetRadius() })) {

		//コライダーAの衝突時コールバックを呼び出す
		colliderA->OnCollision(colliderB);
		// コライダーBの衝突時コールバックを呼び出す
		colliderB->OnCollision(colliderA);

	}
}
