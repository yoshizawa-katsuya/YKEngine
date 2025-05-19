#include "YKFramework.h"

void YKFramework::Initialize()
{
	//スレッドプールの作成
	threadPool_ = ThreadPool::GetInstance();
	threadPool_->Initlaize();

	//ゲームウィンドウの作成
	winApp_ = std::make_unique<WinApp>();
	winApp_->Initialize();

	// DirectX初期化
	dxCommon_ = DirectXCommon::GetInstance();
	dxCommon_->Initialize(winApp_.get());

	//Audio初期化
	audio_ = Audio::GetInstance();
	audio_->Initialize();

	//SrvHeapManager初期化
	srvHeapManager_ = std::make_unique<SrvHeapManager>();
	srvHeapManager_->Initialize(dxCommon_);

#ifdef _DEBUG

	imGuiManager_ = std::make_unique<ImGuiManager>();
	imGuiManager_->Initialize(dxCommon_, winApp_.get(), srvHeapManager_.get());

#endif // _DEBUG

	
	//入力の初期化
	input_ = Input::GetInstance();
	input_->Initialize(winApp_.get());

	//TextureManager初期化
	TextureManager::GetInstance()->Initialize(dxCommon_, srvHeapManager_.get());

	//PSOの設定
	primitiveDrawer_ = std::make_unique<PrimitiveDrawer>();
	primitiveDrawer_->Initialize(dxCommon_);

	//スプライト共通部の初期化
	spritePlatform_ = SpritePlatform::GetInstance();
	spritePlatform_->Initialize(dxCommon_, primitiveDrawer_.get());

	//ParticleManagerの初期化
	ParticleManager::GetInstance()->Initialize(dxCommon_, srvHeapManager_.get(), primitiveDrawer_.get());

	//3Dオブジェクト共通部の初期化
	modelPlatform_ = ModelPlatform::GetInstance();
	modelPlatform_->Initialize(dxCommon_, primitiveDrawer_.get(), srvHeapManager_.get());

	globalVariables_ = GlobalVariables::GetInstance();
	globalVariables_->LoadFiles();

	//シーンマネージャの生成
	sceneManager_ = SceneManager::GetInstance();

}

void YKFramework::Finalize()
{
	dxCommon_->Finalize();

	threadPool_->Finalize();
}

void YKFramework::Update()
{
	//Windowsのメッセージ処理
	if (winApp_->ProcessMessage()) {
		//ゲームループを抜ける
		isEndReqest_ = true;
	}

#ifdef _DEBUG

	//imGuiに、フレームが始まる旨を伝える
	imGuiManager_->Begin();

#endif // _DEBUG

	

	//入力の更新
	input_->Update();

	globalVariables_->Update();

	sceneManager_->Update();

}

void YKFramework::EndFrame()
{

	modelPlatform_->EndFrame();

#ifdef _DEBUG

	//ImGuiの内部コマンドを生成する
	imGuiManager_->End();

#endif // _DEBUG

}

void YKFramework::Run()
{
	//初期化
	Initialize();
	threadPool_->waitForCompletion();

	while (true)	//ゲームループ
	{
		//毎フレーム更新
		Update();
		threadPool_->waitForCompletion();
		//終了リクエストが来たら抜ける
		if (GetIsEndReqest()) {
			break;
		}
		//描画
		Draw();
		threadPool_->waitForCompletion();
	}
	//ゲームの終了
	Finalize();
}
