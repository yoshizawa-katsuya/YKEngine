#pragma once
#include "Collider.h"
#include "BaseModel.h"
#include "InstancingObjects.h"
#include "GlobalVariables.h"

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
	void Draw(Camera* camera);

	/// <summary>
	/// 登録されたコライダーリストを空にする。
	/// </summary>
	void Reset();

	/// <summary>
	/// 全てのコライダーの衝突判定を行う。
	/// </summary>
	void CheckAllCollisions();

	/// <summary>
	/// コライダーを登録する。
	/// </summary>
	/// <param name="collider">登録するコライダー</param>
	void AddCollider(Collider* collider);

private:

	/// <summary>
	/// コライダー2つの衝突判定と応答
	/// </summary>
	/// <param name="player">コライダーA</param>
	/// <param name="colliderB">コライダーB</param>
	void CheckColliderPair(Collider* colliderA, Collider* colliderB);

	/// <summary>
	/// プレイヤーコライダーと他のコライダーの衝突判定と応答。
	/// </summary>
	/// <param name="player">プレイヤーコライダー</param>
	/// <param name="colliderB">他のコライダー</param>
	void CheckPlayerCollisions(Collider* player, Collider* colliderB);

	/// <summary>
	/// 敵コライダーと他のコライダーの衝突判定と応答。
	/// </summary>
	/// <param name="enemy">敵コライダー</param>
	/// <param name="colliderB">他のコライダー</param>
	void CheckEnemyCollisions(Collider* enemy, Collider* colliderB);

	/// <summary>
	/// タックルタイプの敵コライダーと他のコライダーの衝突判定と応答。
	/// </summary>
	/// <param name="tackleEnemy">タックルタイプの敵コライダー</param>
	/// <param name="colliderB">他のコライダー</param>
	void CheckTackleEnemyCollisions(Collider* tackleEnemy, Collider* colliderB);

	/// <summary>
	/// プレイヤー弾コライダーと他のコライダーの衝突判定と応答。
	/// </summary>
	/// <param name="playerBullet">プレイヤー弾コライダー</param>
	/// <param name="colliderB">他のコライダー</param>
	void CheckPlayerBulletCollisions(Collider* playerBullet, Collider* colliderB);

	/// <summary>
	/// 敵弾コライダーと他のコライダーの衝突判定と応答。
	/// </summary>
	/// <param name="enemyBullet">敵弾コライダー</param>
	/// <param name="colliderB">他のコライダー</param>
	void CheckEnemyBulletCollisions(Collider* enemyBullet, Collider* colliderB);

	/// <summary>
	/// レールムーバーコライダーと他のコライダーの衝突判定と応答。
	/// </summary>
	/// <param name="railMover">レールムーバーコライダー</param>
	/// <param name="colliderB">他のコライダー</param>
	void CheakRailMoverCollisions(Collider* railMover, Collider* colliderB);

	/// <summary>
	/// イベントコライダーと他のコライダーの衝突判定と応答。
	/// </summary>
	/// <param name="event">イベントコライダー</param>
	/// <param name="colliderB">他のコライダー</param>
	void CheckEventCollisions(Collider* event, Collider* colliderB);

	//グローバル変数
	GlobalVariables* globalVariables_ = GlobalVariables::GetInstance();

	const std::string kGroupName_ = "Colliders";

	//コライダー
	std::list<Collider*> colliders_;

	//コライダー表示フラグ
	bool isDrawCollider_ = true;

	//デバッグ表示用モデル
	std::shared_ptr<BaseModel> model_;
	std::unique_ptr<InstancingObjects> objects_;
};

