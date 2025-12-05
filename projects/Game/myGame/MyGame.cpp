#include "MyGame.h"
#include "SceneFactory.h"
#include "OffscreenRenderer.h"
#include "manager/EffectManager.h"

using namespace YKEngine;

void MyGame::Initialize()
{

	//基底クラスの初期化処理
	YKFramework::Initialize();

	effectManager_ = EffectManager::GetInstance();
	effectManager_->Initialize();

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

	//解放処理
	YKFramework::Finalize();

}

void MyGame::Update()
{
	effectManager_->Update();

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
