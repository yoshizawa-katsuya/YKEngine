#include "MyGame.h"
#include "SceneFactory.h"
#include "OffscreenRenderer.h"

void MyGame::Initialize()
{

	//基底クラスの初期化処理
	YKFramework::Initialize();

	//シーンファクトリを生成し、マネージャにセット
	sceneFactory_ = std::make_unique<SceneFactory>();
	sceneManager_->SetSceneFactory(sceneFactory_.get());
	//シーンマネージャに最初のシーンをセット
	sceneManager_->ChengeScene("GameScene");

	
}

void MyGame::Finalize()
{

	//解放処理
	//delete gameScene_;
	//gameScene_ = nullptr;


	YKFramework::Finalize();

}

void MyGame::Update()
{

	YKFramework::Update();

	//ゲームの処理
	//gameScene_->Update();


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

#ifdef _DEBUG

	//実際のcommandListのImGuiの描画コマンドを積む
	imGuiManager_->Draw();

#endif // _DEBUG

	dxCommon_->PostDraw();

	TextureManager::GetInstance()->PostDraw();
}
