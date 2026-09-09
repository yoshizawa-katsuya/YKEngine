#pragma once
#include <memory>
#include "BaseScene.h"
#include "Audio.h"
#include "Input.h"
#include "ModelPlatform.h"
#include "SpritePlatform.h"
#include "DirectionalLight.h"
#include "Sprite.h"
#include "PlayerManager.h"
#include "Camera.h"
#include "DebugCamera.h"
#include "CollisionManager.h"
#include "BaseEnemy.h"
#include "SceneTransition.h"
class AudioManager;

/// <summary>
/// ゲームシーン。
/// ゲームのメイン処理を行う。
/// </summary>
class GameScene : public YKEngine::BaseScene
{
public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene() override;

	/// <summary>
	/// 初期化。
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新。
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画。
	/// </summary>
	void Draw() override;

	/// <summary>
	/// 終了。
	/// </summary>
	void Finalize() override;

private:

	/// <summary>
	/// レベルの生成。
	/// </summary>
	void CreateLevel();

	//デバイス
	YKEngine::DirectXCommon* dxCommon_;

	YKEngine::Audio* audio_;
	YKEngine::Input* input_;

	YKEngine::SpritePlatform* spritePlatform_;
	YKEngine::ModelPlatform* modelPlatform_;

	//オーディオマネージャー
	AudioManager* audioManager_;

	YKEngine::Camera* mainCamera_ = nullptr;


	std::unique_ptr<YKEngine::Camera> camera_;

	std::unique_ptr<YKEngine::DebugCamera> debugCamera_;

	bool isActiveDebugCamera_ = false;

	//平行光源
	YKEngine::DirectionalLight directionalLight_;

	//プレイヤー管理クラス
	std::unique_ptr<PlayerManager> playerManager_;

	CollisionManager* collisionManager_ = nullptr;

	//敵
	std::vector<std::unique_ptr<BaseEnemy>> enemies_;

	float timer_ = 0.0f;

	// シーン遷移
	SceneTransition sceneTransition_;

	std::unique_ptr<YKEngine::Sprite> leftPlayerIcon_;
	std::unique_ptr<YKEngine::Sprite> rightPlayerIcon_;
	std::unique_ptr<YKEngine::Sprite> moveSprite_;
	std::unique_ptr<YKEngine::Sprite> wasdSprite_;
	std::unique_ptr<YKEngine::Sprite> arrowKeySprite_;
	std::unique_ptr<YKEngine::Sprite> energyLabelSprite_;

	YKEngine::Vector2 leftPlayerIconPos_;
	YKEngine::Vector2 rightPlayerIconPos_;
	YKEngine::Vector2 moveSpritePos_;
	YKEngine::Vector2 wasdSpritePos_;
	YKEngine::Vector2 arrowKeySpritePos_;
	YKEngine::Vector2 energyLabelSpritePos_;
};