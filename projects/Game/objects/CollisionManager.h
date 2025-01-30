#pragma once
#include <algorithm>
#include <list>
#include "Collider.h"
#include "Model.h"

/// <summary>
/// 衝突マネージャ
/// </summary>
class CollisionManager {
public:

	//初期化
	void Initialize();

	//ワールドトランスフォームの更新
	void UpdateWorldTransform();

	//描画
	void Draw(const ViewProjection& viewProjection);

	//リセット
	void Reset();

	//すべての当たり判定をチェック
	void CheckAllCollisions();

	void AddCollider(Collider* collider);

private:

	/// <summary>
	/// コライダー2つの衝突判定と応答
	/// </summary>
	/// <param name="colliderA">コライダーA</param>
	/// <param name="colliderB">コライダーB</param>
	void CheckColliderPair(Collider* colliderA, Collider* colliderB);

	const char* groupName_ = "Colliders";

	//コライダー
	std::list<Collider*> colliders_;

	//コライダー表示
	bool isDrawCollider_ = false;

	//デバッグ表示用モデル
	std::unique_ptr<Model> model_;
};
