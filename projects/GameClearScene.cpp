#include "GameClearScene.h"
#include "imgui/imgui.h"
#include "SceneManager.h"

GameClearScene::~GameClearScene()
{
}

void GameClearScene::Initialize()
{

	dxCommon_ = DirectXCommon::GetInstance();
	audio_ = Audio::GetInstance();
	input_ = Input::GetInstance();
	spritePlatform_ = SpritePlatform::GetInstance();
	modelPlatform_ = ModelPlatform::GetInstance();

	clear1Sprite_ = TextureManager::GetInstance()->Load("./resources/screen/clear1.png");
	sprite1_ = std::make_unique<Sprite>();
	sprite1_->Initialize(clear1Sprite_, spritePlatform_);
	sprite1_->SetPosition({ 0.0f, 0.0f });

}

void GameClearScene::Update()
{
	if (input_->TriggerKey(DIK_SPACE)) {
		//シーン切り替え依頼
		sceneManager_->ChengeScene("TitleScene");
	}

}

void GameClearScene::Draw()
{
	spritePlatform_->PreDraw();
	sprite1_->Draw();

}

void GameClearScene::Finalize()
{
}
