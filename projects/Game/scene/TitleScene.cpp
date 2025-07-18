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

	title_ = TextureManager::GetInstance()->Load("./resources/scene/title.png");
	titleSprite_ = std::make_unique<Sprite>();
	titleSprite_->Initialize(title_);
	titleSprite_->SetPosition({ 0.0f, 0.0f });

	start1_ = TextureManager::GetInstance()->Load("./resources/scene/start1.png");
	startSprite1_ = std::make_unique<Sprite>();
	startSprite1_->Initialize(start1_);
	startSprite1_->SetPosition({ 0.0f, 0.0f });

	start2_ = TextureManager::GetInstance()->Load("./resources/scene/start2.png");
	startSprite2_ = std::make_unique<Sprite>();
	startSprite2_->Initialize(start2_);
	startSprite2_->SetPosition({ 0.0f, 0.0f });
	
	
}

void TitleScene::Update()
{
	Square square;
	square.min = { 254.0f, 369.0f };
	square.max = { 732.0f, 486.0f };

	bool isMouseInside = IsCollision(square, input_->GetMousePosition());

	if (isMouseInside) {
		startSprite2_->SetVisible(true); 
		startSprite1_->SetVisible(false);
	} else {
		startSprite2_->SetVisible(false); 
		startSprite1_->SetVisible(true);
	}
	if (startSprite2_->IsVisible()) {
		if (input_->PushMouseLeft() && !input_->TrigerMouseLeft()) {
			//シーン切り替え依頼
			sceneManager_->ChengeScene("SelectScene");
		}
			


	}

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

	//if (input_->TriggerKey(DIK_SPACE)) {
	//	//シーン切り替え依頼
	//	sceneManager_->ChengeScene("SelectScene");
	//}

}

void TitleScene::Draw()
{

	//Spriteの描画準備。Spriteの描画に共通のグラフィックスコマンドを積む
	spritePlatform_->PreDraw();

	titleSprite_->Draw();

	
	if (startSprite1_->IsVisible()) {
		startSprite1_->Draw();
	}
	if (startSprite2_->IsVisible()) {
		startSprite2_->Draw();
	}
	
	
	
}

void TitleScene::Finalize()
{

}
