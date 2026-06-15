#pragma once
#include "ThreadPool.h"
#include "WinApp.h"
#include "DirectXCommon.h"
#include "Audio.h"
#include "SrvHeapManager.h"
#include "TextureManager.h"
#include "Sprite.h"
#include "SpritePlatform.h"
#include "BaseModel.h"
#include "ModelPlatform.h"
#include "PipelineManager.h"
#include "ParticleManager.h"
#include "Input.h"
#include "dx12.h"
#include "SceneManager.h"
#include "AbstractSceneFactory.h"
#include "GlobalVariables.h"
#include <format>
#include <dxgidebug.h>
#include <dxcapi.h>
#define _USE_MATH_DEFINES
#include <math.h>

#ifdef  USE_IMGUI
#include "ImGuiManager.h"
#endif //  USE_IMGUI

namespace YKEngine
{
class OffscreenRenderer;

/// <summary>
/// フレームワーククラス。
/// ゲームの大枠を管理する。
/// ゲームごとに継承して使用する。
/// ゲームループはRun()で実行される。
/// ゲームループ内でInitialize()、Update()、Draw()が呼ばれる。
/// </summary>
class YKFramework
{
public:

	/// <summary>
	/// デストラクタ。純粋仮想関数として宣言。
	/// </summary>
	virtual ~YKFramework() = default;

	/// <summary>
	/// 初期化。
	/// </summary>
	virtual void Initialize();

	/// <summary>
	/// 終了処理。
	/// </summary>
	virtual void Finalize();

	/// <summary>
	/// 更新。
	/// </summary>
	virtual void Update();

	/// <summary>
	/// フレーム終了処理。
	/// </summary>
	void EndFrame();

	/// <summary>
	/// 描画。純粋仮想関数として宣言。
	/// </summary>
	virtual void Draw() = 0;

	/// <summary>
	/// ゲームループの実行。
	/// </summary>
	void Run();

protected:

	ThreadPool* threadPool_ = nullptr;
	std::unique_ptr<WinApp> winApp_;
	DirectXCommon* dxCommon_ = nullptr;
	Audio* audio_ = nullptr;
	std::unique_ptr<SrvHeapManager> srvHeapManager_;
	Input* input_ = nullptr;
	std::unique_ptr<PipelineManager> primitiveDrawer_;
	SpritePlatform* spritePlatform_ = nullptr;
	ModelPlatform* modelPlatform_ = nullptr;
	OffscreenRenderer* offscreenRenderer_ = nullptr;
	GlobalVariables* globalVariables_ = nullptr;

#ifdef USE_IMGUI
	std::unique_ptr<ImGuiManager> imGuiManager_;
#endif // USE_IMGUI

	std::unique_ptr<SceneManager> sceneManager_;

	//シーンファクトリー
	std::unique_ptr<AbstractSceneFactory> sceneFactory_;

	//ゲーム終了フラグ
	bool isEndReqest_ = false;

};

} // namespace YKEngine