#include "TitleScene.h"
#include "imgui/imgui.h"
#include "SceneManager.h"
#include "Collision.h"
#include "Vector.h"

TitleScene::~TitleScene()
{
	//Finalize();
}

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

	//select
	


}

void TitleScene::Update()
{
	
	


#ifdef _DEBUG
	
	ImGui::Begin("Window");
	ImGui::Text("Title");
	//ImGui::DragFloat2("tutorialSprite_", &tutorialSprite_->GetPosition().x, 0.11f);
	/*
	ImGui::DragFloat2("selectdSprite1_", &selectdSprite1_->GetPosition().x, 0.11f);
	ImGui::DragFloat2("selectdSprite2_", &selectdSprite2_->GetPosition().x, 0.11f);
	*/
	ImGui::End();
	
#endif // _DEBUG

	if (input_->TriggerKey(DIK_SPACE)) {
		//シーン切り替え依頼
		sceneManager_->ChengeScene("SelectScene");
	}

}

void TitleScene::Draw()
{

	//Spriteの描画準備。Spriteの描画に共通のグラフィックスコマンドを積む
	spritePlatform_->PreDraw();



}

void TitleScene::Finalize()
{

}
