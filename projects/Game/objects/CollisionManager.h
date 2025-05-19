#pragma once
#include "Collider.h"
#include "BaseModel.h"
#include "InstancingObjects.h"
#include "GlobalVariables.h"

/// <summary>
/// 衝突マネージャ
/// </summary>
class CollisionManager
{
public:

	//初期化
	void Initialize();

	//更新
	void Update();

	//描画
	void Draw(Camera* camera);

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

	//グローバル変数
	GlobalVariables* globalVariables_ = GlobalVariables::GetInstance();

	const char* groupName_ = "Colliders";

	//コライダー
	std::list<Collider*> colliders_;

	//コライダー表示フラグ
	bool isDrawCollider_ = true;

	//デバッグ表示用モデル
	std::shared_ptr<BaseModel> model_;
	std::unique_ptr<InstancingObjects> objects_;
};

