#include "GameOverScene.h"
#include "imgui/imgui.h"
#include "SceneManager.h"

void GameOverScene::Initialize()
{

	dxCommon_ = DirectXCommon::GetInstance();
	audio_ = Audio::GetInstance();
	input_ = Input::GetInstance();
	spritePlatform_ = SpritePlatform::GetInstance();
	modelPlatform_ = ModelPlatform::GetInstance();

	gameOver1Sprite_ = TextureManager::GetInstance()->Load("./resources/gameOver1.png");
	gameOver2Sprite_ = TextureManager::GetInstance()->Load("./resources/gameOver2.png");

	gameOver1_ = std::make_unique<Sprite>();
	gameOver1_->Initialize(gameOver1Sprite_, spritePlatform_);
	gameOver1_->SetPosition({ 0.0f, 0.0f });

	gameOver2_ = std::make_unique<Sprite>();
	gameOver2_->Initialize(gameOver2Sprite_, spritePlatform_);
	gameOver2_->SetPosition({ 0.0f, 0.0f });


}

void GameOverScene::Update()
{

	if (input_->TriggerKey(DIK_SPACE)) {
		//シーン切り替え依頼
		sceneManager_->ChengeScene("TITLE");
	}

}

void GameOverScene::Draw()
{

	time--;
	if (time <= 40) {
		gameOver1_->Draw();
		if (time <= 20) {
			gameOver2_->Draw();
		}
	}
	if (time == 0) {
		time = 40;
	}

}

void GameOverScene::Finalize()
{

}