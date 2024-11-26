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

	over1Sprite_ = TextureManager::GetInstance()->Load("./resources/screen/over.png");
	sprite1_ = std::make_unique<Sprite>();
	sprite1_->Initialize(over1Sprite_, spritePlatform_);
	sprite1_->SetPosition({ 0.0f, 0.0f });


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
	sprite1_->Draw();

}

void GameOverScene::Finalize()
{
}
