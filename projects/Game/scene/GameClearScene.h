#pragma once
#include "BaseScene.h"
#include "DirectXCommon.h"
#include "Audio.h"
#include "Input.h"
#include "TextureManager.h"
#include "ModelPlatform.h"
#include "SpritePlatform.h"
#include "Sprite.h"
#include "SceneManager.h"
#include "SceneTransition.h"
#include "My3dObject.h"
#include "Camera.h"
#include "TitleScene.h"

class GameClearScene : public YKEngine::BaseScene
{
public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameClearScene() override;

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
	/// ImGuiの描画。
	/// </summary>
	void DrawImGui();

	/// <summary>
	/// 終了。
	/// </summary>
	void Finalize() override;
private:

	/// <summary>
	// 決定操作の更新（決定ボタン）
	/// </summary>
	void UpdateDecide();

private:
	//デバイス
	YKEngine::DirectXCommon* dxCommon_;

	YKEngine::Audio* audio_;
	YKEngine::Input* input_;

	YKEngine::SpritePlatform* spritePlatform_;
	YKEngine::ModelPlatform* modelPlatform_;

	YKEngine::Camera* mainCamera_ = nullptr;

	std::unique_ptr<YKEngine::Camera> camera_;

	// 選択肢の種類
	enum class SELECT
	{
		kRetry, // リトライ
		kTitle, // タイトルへ戻る
	};

	// 現在選択中の項目
	SELECT select_ = SELECT::kRetry;

	// 決定されたかどうか（演出用に決定後は入力を受け付けなくする）
	bool isDecided_ = false;


	SceneTransition sceneTransition_;

	// 天球
	std::shared_ptr<YKEngine::BaseModel> skySphereModel_;
	std::unique_ptr<YKEngine::My3dObject> skySphereObject_;

	YKEngine::WorldTransform skySphereTransform_;

	// 背景
	std::shared_ptr<YKEngine::BaseModel> backgroundModel_;
	std::unique_ptr<YKEngine::My3dObject> backgroundObject_;

	YKEngine::WorldTransform backgroundTransform_;

	std::unique_ptr<YKEngine::Sprite> gameClearSprite_;
	YKEngine::Vector2 gameClearPos_;

	std::unique_ptr<YKEngine::Sprite> returnSprite_;
	YKEngine::Vector2 returnPos_;


	float pressKeyTimer_ = 0.0f;           // 点滅用の経過時間

};

