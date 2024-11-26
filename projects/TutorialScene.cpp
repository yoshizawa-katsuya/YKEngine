#include "TutorialScene.h"
#include "imgui/imgui.h"
#include "SceneManager.h"

TutorialScene::~TutorialScene()
{
}

void TutorialScene::Initialize()
{
	dxCommon_ = DirectXCommon::GetInstance();
	audio_ = Audio::GetInstance();
	input_ = Input::GetInstance();
	spritePlatform_ = SpritePlatform::GetInstance();
	modelPlatform_ = ModelPlatform::GetInstance();
	
	title1Sprite_ = TextureManager::GetInstance()->Load("./resources/screen/tutorial.png");
	sprite1_ = std::make_unique<Sprite>();
	sprite1_->Initialize(title1Sprite_, spritePlatform_);
	sprite1_->SetPosition({ 0.0f, 0.0f });
}

void TutorialScene::Update()
{
#ifdef _DEBUG

	ImGui::Begin("Window");
	ImGui::Text("Tutorial");
	ImGui::End();

#endif // _DEBUG

	if (input_->TriggerKey(DIK_SPACE)) {
		//シーン切り替え依頼
		sceneManager_->ChengeScene("GameScene");
	}

}

void TutorialScene::Draw()
{
	//Spriteの描画準備。Spriteの描画に共通のグラフィックスコマンドを積む
	spritePlatform_->PreDraw();
	sprite1_->Draw();
}

void TutorialScene::Finalize()
{
}
