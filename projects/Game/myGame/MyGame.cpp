#include "MyGame.h"
#include "SceneFactory.h"

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
	dxCommon_->PreDrawRenderTexture();

	srvHeapManager_->PreDraw();

	//RootSignatureを設定。PSOに設定しているけど別途設定が必要
	//primitiveDrawer_->SetPipelineSet(dxCommon_->GetCommandList(), BlendMode::kBlendModeNone);
	/*
	dxCommon->GetCommandList()->SetGraphicsRootSignature(primitiveDrawer->GetRootSignature());
	dxCommon->GetCommandList()->SetPipelineState(primitiveDrawer->GetGrahicsPipelineState());	//PSOを設定
	*/
	//commandList_->IASetIndexBuffer(&indexBufferView);	//IBVを設定
	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えておけば良い
	//dxCommon_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	//gameScene_->Draw();
	//titleScene_->Draw();
	sceneManager_->Draw();


	dxCommon_->PreDraw();

	dxCommon_->PostDrawRenderTexture(primitiveDrawer_.get(), srvHeapManager_.get());

#ifdef _DEBUG

	//実際のcommandListのImGuiの描画コマンドを積む
	imGuiManager_->Draw();

#endif // _DEBUG

	dxCommon_->PostDraw();

	TextureManager::GetInstance()->PostDraw();
}
