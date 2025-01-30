#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "ModelPlatform.h"
#include "SpritePlatform.h"
#include "Sprite.h"
#include "WorldTransform.h"
#include "RigidModel.h"
#include "DebugCamera.h"
#include "Camera.h"
#include "Player.h"
#include "LockOn.h"
#include "Enemy.h"
#include "Skydome.h"
#include "Ground.h"
#include "FollowCamera.h"
#include "CollisionManager.h"
#include <memory>

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private:

	/// <summary>
	/// 衝突判定と応答
	/// </summary>
	void CheckAllCollisions();

	// メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	SpritePlatform* spritePlatform_;
	ModelPlatform* modelPlatform_;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	
	//ビュープロジェクション
	Camera* mainCamera_ = nullptr;

	std::unique_ptr<Camera> camera_;
	std::unique_ptr<Camera> camera2_;

	//デバッグカメラ
	std::unique_ptr<DebugCamera> debugCamera_;

	//デバッグカメラ有効
	bool isActiveDebugCamera_ = false;

	//追従カメラ
	std::unique_ptr<FollowCamera> followCamera_;

	//衝突マネージャ
	std::unique_ptr<CollisionManager> collisionManager_;

	//テクスチャハンドル
	uint32_t textureHnadle_ = 0;

	//モデル
	//std::unique_ptr<Model> model_;
	std::unique_ptr<RigidModel> modelSkydome_;
	std::unique_ptr<RigidModel> modelGround_;

	std::unique_ptr<RigidModel> modelFighterBody_;
	std::unique_ptr<RigidModel> modelFighterHead_;
	std::unique_ptr<RigidModel> modelFighterL_arm_;
	std::unique_ptr<RigidModel> modelFighterR_arm_;

	std::unique_ptr<RigidModel> modelHammer_;

	std::unique_ptr<RigidModel> modelEnemyBody_;
	std::unique_ptr<RigidModel> modelEnemyL_arm_;
	std::unique_ptr<RigidModel> modelEnemyR_arm_;

	// スカイドーム
	std::unique_ptr<Skydome> skydome_;

	//地面
	std::unique_ptr<Ground> ground_;

	//自キャラ
	std::unique_ptr<Player> player_;

	//ロックオン
	std::unique_ptr<LockOn> lockOn_;

	//敵
	std::list<std::unique_ptr<Enemy>> enemies_;
	
};
