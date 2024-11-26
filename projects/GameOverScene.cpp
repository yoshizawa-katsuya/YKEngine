#include "GameOverScene.h"
#include "imgui/imgui.h"
#include "SceneManager.h"

GameOverScene::~GameOverScene()
{
}

void GameOverScene::Initialize()
{

	dxCommon_ = DirectXCommon::GetInstance();
	audio_ = Audio::GetInstance();
	input_ = Input::GetInstance();
	spritePlatform_ = SpritePlatform::GetInstance();
	modelPlatform_ = ModelPlatform::GetInstance();

	over1Sprite_ = TextureManager::GetInstance()->Load("./resources/screen/over1.png");
	over2Sprite_ = TextureManager::GetInstance()->Load("./resources/screen/over2.png");

	sprite1_ = std::make_unique<Sprite>();
	sprite1_->Initialize(over1Sprite_, spritePlatform_);
	sprite1_->SetPosition({ 0.0f, 0.0f });

	sprite2_ = std::make_unique<Sprite>();
	sprite2_->Initialize(over2Sprite_, spritePlatform_);
	sprite2_->SetPosition({ 0.0f, 0.0f });

	time = 40;
}

void GameOverScene::Update()
{

	if (input_->TriggerKey(DIK_SPACE)) {
		//シーン切り替え依頼
		sceneManager_->ChengeScene("TitleScene");
	}

}

void GameOverScene::Draw()
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

void GameOverScene::Finalize()
{
}
