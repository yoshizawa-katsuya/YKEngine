#pragma once
#include "ThreadPool.h"
#include "WinApp.h"
#include "DirectXCommon.h"
#include "Audio.h"
#include "SrvHeapManager.h"
#include "ImGuiManager.h"
#include "TextureManager.h"
#include "Sprite.h"
#include "SpritePlatform.h"
#include "BaseModel.h"
#include "ModelPlatform.h"
#include "PrimitiveDrawer.h"
#include "ParticleManager.h"
#include "Input.h"
#include "dx12.h"
#include "SceneManager.h"
#include "AbstractSceneFactory.h"
#include <format>
#include <dxgidebug.h>
#include <dxcapi.h>
#define _USE_MATH_DEFINES
#include <math.h>

//ゲーム全体
class YKFramework
{
public:

	virtual ~YKFramework() = default;

	//初期化
	virtual void Initialize();

	//終了
	virtual void Finalize();

	//更新
	virtual void Update();

	void EndFrame();

	//描画
	virtual void Draw() = 0;

	//実行
	void Run();

	//終了フラグのチェック
	virtual bool GetIsEndReqest() { return isEndReqest_; }

protected:

	ThreadPool* threadPool_ = nullptr;
	std::unique_ptr<WinApp> winApp_;
	DirectXCommon* dxCommon_ = nullptr;
	Audio* audio_ = nullptr;
	std::unique_ptr<SrvHeapManager> srvHeapManager_;
	std::unique_ptr<ImGuiManager> imGuiManager_;
	Input* input_ = nullptr;
	std::unique_ptr<PrimitiveDrawer> primitiveDrawer_;
	SpritePlatform* spritePlatform_ = nullptr;
	ModelPlatform* modelPlatform_ = nullptr;

	SceneManager* sceneManager_ = nullptr;

	//シーンファクトリー
	std::unique_ptr<AbstractSceneFactory> sceneFactory_;

	//ゲーム終了フラグ
	bool isEndReqest_ = false;

};

