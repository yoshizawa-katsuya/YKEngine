#pragma once
#include "YKFramework.h"
class EffectManager;
class SceneChangeStaging;

/// <summary>
/// ゲームのメインクラス。
/// YKFrameworkを継承して作成する。
/// YKFrameworkの機能を使用してゲームを作成する。
/// </summary>
class MyGame : public YKEngine::YKFramework
{
public:

	/// <summary>
	/// 初期化。
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 終了処理。
	/// </summary>
	void Finalize() override;

	/// <summary>
	/// 更新。
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画。
	/// </summary>
	void Draw() override;

	
private:
	
	EffectManager* effectManager_ = nullptr;

	SceneChangeStaging* sceneChangeStaging_ = nullptr;
};

