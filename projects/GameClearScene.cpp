#include "GameClearScene.h"
#include "imgui/imgui.h"
#include "SceneManager.h"

void GameClearScene::Initialize()
{

	dxCommon_ = DirectXCommon::GetInstance();
	audio_ = Audio::GetInstance();
	input_ = Input::GetInstance();
	spritePlatform_ = SpritePlatform::GetInstance();
	modelPlatform_ = ModelPlatform::GetInstance();

	gameClear1Sprite_ = TextureManager::GetInstance()->Load("./resources/gameClear1.png");
	gameClear2Sprite_ = TextureManager::GetInstance()->Load("./resources/gameClear2.png");

	gameClear1_ = std::make_unique<Sprite>();
	gameClear1_->Initialize(gameClear1Sprite_, spritePlatform_);
	gameClear1_->SetPosition({ 0.0f, 0.0f });

	gameClear2_ = std::make_unique<Sprite>();
	gameClear2_->Initialize(gameClear2Sprite_, spritePlatform_);
	gameClear2_->SetPosition({ 0.0f, 0.0f });

	time = 40;

}

void GameClearScene::Update()
{

	if (input_->TriggerKey(DIK_SPACE)) {

		//シーン切り替え依頼
		sceneManager_->ChengeScene("TITLE");
	}

}

void GameClearScene::Draw()
{

	time--;
	if (time <= 40) {
		gameClear1_->Draw();
		if (time <= 20) {
			gameClear2_->Draw();
		}
	}
	if (time == 0) {
		time = 40;
	}


}

void GameClearScene::Finalize()
{

}