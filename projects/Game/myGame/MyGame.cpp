#include "MyGame.h"
#include "SceneFactory.h"
#include "OffscreenRenderer.h"
#include "manager/EffectManager.h"
#include "SceneChangeStaging.h"
#include "manager/CollisionManager.h"
#include "manager/AudioManager.h"

using namespace YKEngine;

void MyGame::Initialize()
{

	//基底クラスの初期化処理
	YKFramework::Initialize();

	//エフェクト管理クラスの生成
	effectManager_ = EffectManager::GetInstance();
	effectManager_->Initialize();

	//オーディオ管理クラスの生成
	audioManager_ = AudioManager::GetInstance();
	audioManager_->Initialize();

	//シーンチェンジ演出クラスの生成
	sceneChangeStaging_ = SceneChangeStaging::GetInstance();
	sceneChangeStaging_->Initialize();

	//衝突管理クラスの生成
	collisionManager_ = CollisionManager::GetInstance();
	collisionManager_->Initialize();

	//シーンファクトリを生成し、マネージャにセット
	sceneFactory_ = std::make_unique<SceneFactory>();
	sceneManager_->SetSceneFactory(sceneFactory_.get());

	//シーンマネージャに最初のシーンをセット
#ifdef _DEBUG
	sceneManager_->ChengeScene("GameScene");
#else
	sceneManager_->ChengeScene("TitleScene");
#endif

}

void MyGame::Finalize()
{
	//終了処理
	sceneChangeStaging_->Finalize();
	collisionManager_->Finalize();

	YKFramework::Finalize();

}

void MyGame::Update()
{
	//更新処理
	effectManager_->Update();

	sceneChangeStaging_->Update();

	collisionManager_->Update();

	YKFramework::Update();

	YKFramework::EndFrame();

}

void MyGame::Draw()
{

	//描画開始
	srvHeapManager_->PreDraw();


	offscreenRenderer_->PreDrawRenderTexture();
	
	//オフスクリーンレンダリングを使用していない場合は、ここで描画コマンドを積む
	if (!offscreenRenderer_->GetUseOffscreenRender())
	{
		dxCommon_->PreDraw();
	}

	sceneManager_->Draw();

	//オフスクリーンレンダリングを使用している場合は、ここで描画コマンドを積む
	if (offscreenRenderer_->GetUseOffscreenRender())
	{
		dxCommon_->PreDraw();
	}

	offscreenRenderer_->PostDrawRenderTexture(primitiveDrawer_.get(), srvHeapManager_.get());

#ifdef USE_IMGUI

	//実際のcommandListのImGuiの描画コマンドを積む
	imGuiManager_->Draw();

#endif // USE_IMGUI

	dxCommon_->PostDraw();

	TextureManager::GetInstance()->PostDraw();
}
