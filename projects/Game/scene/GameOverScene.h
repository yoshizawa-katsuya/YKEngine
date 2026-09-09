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
class AudioManager;

// ゲームオーバーシーン
class GameOverScene : public YKEngine::BaseScene
{
public:		
	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameOverScene() override;

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
	// 選択操作の更新（左右キー）
	/// </summary>
	void UpdateSelect();

	/// <summary>
	// 決定操作の更新（決定ボタン）
	/// </summary>
	void UpdateDecide();

	/// <summary>
	/// 選択中のスプライトの点滅
	/// </summary>
	void UpdateSelectSprite();

private:
	//デバイス
	YKEngine::DirectXCommon* dxCommon_;

	YKEngine::Audio* audio_;
	YKEngine::Input* input_;

	YKEngine::SpritePlatform* spritePlatform_;
	YKEngine::ModelPlatform* modelPlatform_;

	YKEngine::Camera* mainCamera_ = nullptr;

	std::unique_ptr<YKEngine::Camera> camera_;
	//オーディオマネージャー
	AudioManager* audioManager_;

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

	std::unique_ptr<YKEngine::Sprite> linkLostSprite_;
	YKEngine::Vector2 linkLostPos_;

	std::unique_ptr<YKEngine::Sprite> retrySprite_;
	YKEngine::Vector2 retryPos_;

	std::unique_ptr<YKEngine::Sprite> titleSprite_;
	YKEngine::Vector2 titlePos_;

	float selectFlickerTimer_ = 0.0f;

	// 点滅速度
	static constexpr float kSelectFlickerSpeed = 5.0f;

	// 選択中の最低アルファ
	static constexpr float kSelectFlickerMinAlpha = 0.3f;

	// 通常のアルファ
	static constexpr float kSelectFlickerMaxAlpha = 1.0f;
};

