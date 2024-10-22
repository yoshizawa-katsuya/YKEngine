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

}

void GameClearScene::Finalize()
{

}