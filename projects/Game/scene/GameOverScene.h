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

private:
	//デバイス
	YKEngine::DirectXCommon* dxCommon_;

	YKEngine::Audio* audio_;
	YKEngine::Input* input_;

	YKEngine::SpritePlatform* spritePlatform_;
	YKEngine::ModelPlatform* modelPlatform_;

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
};

