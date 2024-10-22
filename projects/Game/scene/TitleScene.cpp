#include "TitleScene.h"
#include "imgui/imgui.h"
#include "SceneManager.h"

void TitleScene::Initialize()
{

	dxCommon_ = DirectXCommon::GetInstance();
	audio_ = Audio::GetInstance();
	input_ = Input::GetInstance();
	spritePlatform_ = SpritePlatform::GetInstance();
	modelPlatform_ = ModelPlatform::GetInstance();
	/*
	textureHandle_ = TextureManager::GetInstance()->Load("./resources/Title.png");

	sprite_ = std::make_unique<Sprite>();
	sprite_->Initialize(textureHandle_, spritePlatform_);
	sprite_->SetPosition({ 100.0f, 100.0f });
	*/
}

void TitleScene::Update()
{

#ifdef _DEBUG
	/*
	ImGui::Begin("Window");

	ImGui::End();
	*/
	if (input_->TriggerKey(DIK_RETURN)) {
		//シーン切り替え依頼
		sceneManager_->ChengeScene("GAMEPLAY");
	}
#endif // _DEBUG

}

void TitleScene::Draw()
{

	//Spriteの描画準備。Spriteの描画に共通のグラフィックスコマンドを積む
	//spritePlatform_->PreDraw();

	//sprite_->Draw();

}

void TitleScene::Finalize()
{

}
