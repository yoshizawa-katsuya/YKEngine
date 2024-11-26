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
	clear2Sprite_ = TextureManager::GetInstance()->Load("./resources/screen/clear2.png");

	sprite1_ = std::make_unique<Sprite>();
	sprite1_->Initialize(clear1Sprite_, spritePlatform_);
	sprite1_->SetPosition({ 0.0f, 0.0f });

	sprite2_ = std::make_unique<Sprite>();
	sprite2_->Initialize(clear2Sprite_, spritePlatform_);
	sprite2_->SetPosition({ 0.0f, 0.0f });

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
	time--;
	if (time <= 40) {
		sprite1_->Draw();
		if (time <= 20) {
			sprite2_->Draw();
		}
	}
	if (time == 0) {
		time = 40;
	}

}

void GameClearScene::Finalize()
{
}
