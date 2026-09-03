#include "MyGame.h"
#include "SceneFactory.h"
#include "OffscreenRenderer.h"
#include "CollisionManager.h"

using namespace YKEngine;

void MyGame::Initialize()
{

	//基底クラスの初期化処理
	YKFramework::Initialize();

	//衝突マネージャの初期化
	collisionManager_ = CollisionManager::GetInstance();
	collisionManager_->Initialize();

	//シーンファクトリを生成し、マネージャにセット
	sceneFactory_ = std::make_unique<SceneFactory>();
	sceneManager_->SetSceneFactory(sceneFactory_.get());
	//シーンマネージャに最初のシーンをセット
	sceneManager_->ChengeScene("GameScene");

	
}

void MyGame::Finalize()
{
	//衝突マネージャの終了処理
	collisionManager_->Finalize();

	YKFramework::Finalize();

}

void MyGame::Update()
{
	//衝突マネージャの更新
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
