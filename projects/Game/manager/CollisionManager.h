#pragma once
#include "SphereCollider.h"
#include "OBBCollider.h"
#include "BaseModel.h"
#include "InstancingObjects.h"
#include "GlobalVariables.h"
#include <set>

/// <summary>
/// 衝突マネージャ。
/// 衝突判定と応答を行う。
/// 衝突する可能性のあるコライダーを登録し、衝突判定を行う。
/// 衝突した場合は、各コライダーの応答関数を呼び出す。
/// デバッグ用にコライダーの形状を描画する機能も持つ。
/// </summary>
class CollisionManager
{
public:

	/// <summary>
	/// シングルトンインスタンスの取得。
	/// </summary>
	/// <returns>シングルトンインスタンス</returns>
	static CollisionManager* GetInstance();

	/// <summary>
	/// 終了処理。
	/// </summary>
	void Finalize();

	/// <summary>
	/// 初期化。
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新。
	/// </summary>
	void Update();

	/// <summary>
	/// 描画。
	/// </summary>
	/// <param name="camera">描画に使用するカメラ</param>
	void Draw(YKEngine::Camera* camera);

	/// <summary>
	/// 登録されたコライダーリストを空にする。
	/// </summary>
	void Reset();

	/// <summary>
	/// 全てのコライダーの衝突判定を行う。
	/// </summary>
	void CheckAllCollisions();

	/// <summary>
	/// 球コライダーを登録する。
	/// </summary>
	/// <param name="collider">登録するコライダー</param>
	void AddSphereCollider(SphereCollider* collider);

	/// <summary>
	/// OBBコライダーを登録する。
	/// </summary>
	/// <param name="collider">登録するコライダー</param>
	void AddOBBCollider(OBBCollider* collider);

	/// <summary>
	/// 球コライダーを削除する。
	/// </summary>
	/// <param name="collider">削除するコライダー</param>
	void RemoveSphereCollider(SphereCollider* collider);

	/// <summary>
	/// OBBコライダーを削除する。
	/// </summary>
	/// <param name="collider">削除するコライダー</param>
	void RemoveOBBCollider(OBBCollider* collider);

	//コンストラクタに渡すための鍵
	class ConstructorKey {
	private:
		ConstructorKey() = default;
		friend class CollisionManager;
	};

	//PassKeyを受け取るコンストラクタ
	explicit CollisionManager(ConstructorKey key) {}

private:

	// シングルトンインスタンス
	static std::unique_ptr<CollisionManager> instance_;
	friend struct std::default_delete<CollisionManager>;

	~CollisionManager() = default;
	CollisionManager(CollisionManager&) = delete;
	CollisionManager& operator=(CollisionManager&) = delete;

	/// <summary>
	/// 球コライダー同士の衝突判定と応答を行う。
	/// </summary>
	void CheckSphereCollisions();

	/// <summary>
	/// 球コライダーとOBBコライダーの衝突判定と応答を行う。
	/// </summary>
	void CheckSphereOBBCollisions();

	/// <summary>
	/// 球コライダー2つの衝突判定と応答
	/// </summary>
	/// <param name="colliderA">コライダーA</param>
	/// <param name="colliderB">コライダーB</param>
	void CheckSphereColliderPair(SphereCollider* colliderA, SphereCollider* colliderB);

	bool IsCollisionPair(CollisionTypeIdDef typeA, CollisionTypeIdDef typeB);

	//グローバル変数
	YKEngine::GlobalVariables* globalVariables_ = YKEngine::GlobalVariables::GetInstance();

	const std::string kGroupName_ = "Colliders";

	//衝突判定を行う球コライダーのリスト
	std::vector<SphereCollider*> sphereColliders_;

	//衝突判定を行うOBBコライダーのリスト
	std::vector<OBBCollider*> obbColliders_;

	//コライダー表示フラグ
	bool isDrawCollider_ = true;

	//デバッグ表示用モデル
	std::shared_ptr<YKEngine::BaseModel> model_;
	std::unique_ptr<YKEngine::InstancingObjects> objects_;

	const std::set<std::pair<CollisionTypeIdDef, CollisionTypeIdDef>> kCollisionPairs_ = {
		{CollisionTypeIdDef::kPlayer, CollisionTypeIdDef::kEnemyBullet},
		{CollisionTypeIdDef::kPlayer, CollisionTypeIdDef::kTackleEnemy},

		{CollisionTypeIdDef::kEnemy, CollisionTypeIdDef::kPlayerBullet},
		{CollisionTypeIdDef::kEnemy, CollisionTypeIdDef::kCuboidObstacle},

		{CollisionTypeIdDef::kTackleEnemy, CollisionTypeIdDef::kPlayerBullet},

		{CollisionTypeIdDef::kRailMover, CollisionTypeIdDef::kEvent},

	};

};

