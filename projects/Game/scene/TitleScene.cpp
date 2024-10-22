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

	fade_ = std::make_unique<Fade>();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 1.0f);
}

void TitleScene::Update()
{

#ifdef _DEBUG
	/*
	ImGui::Begin("Window");

	if (ImGui::TreeNode("Sprite")) {
		ImGui::DragFloat2("tranlate", &sprite_->GetPosition().x, 0.01f);
		ImGui::DragFloat2("size", &sprite_->GetSize().x, 0.01f);
		ImGui::SliderAngle("rotate", &sprite_->GetRotation());
		ImGui::ColorEdit4("color", &sprite_->GetColor().x);
		ImGui::DragFloat2("anchprPoint", &sprite_->GetAnchorPoint().x, 0.01f);
		ImGui::Checkbox("IsFlipX", &sprite_->GetIsFlipX());
		ImGui::Checkbox("IsFlipY", &sprite_->GetIsFlipY());
		ImGui::DragFloat2("textureLeftTop", &sprite_->GetTextureLeftTop().x, 0.01f);
		ImGui::DragFloat2("textureSize", &sprite_->GetTextureSize().x, 0.01f);

		ImGui::TreePop();
	}

	ImGui::End();
	*/
	
#endif // _DEBUG

	switch (phase_) {
	case TitleScene::Phase::kFadeIn:
		fade_->Update();
		if (fade_->IsFinished()) {
			fade_->Stop();
			phase_ = Phase::kMain;
		}
		break;

	case TitleScene::Phase::kMain:

		if (input_->TriggerKey(DIK_SPACE)) {
			fade_->Start(Fade::Status::FadeOut, 1.0f);
			phase_ = Phase::kFadoOut;
		}

		break;

	case TitleScene::Phase::kFadoOut:
		fade_->Update();
		if (fade_->IsFinished()) {
			//シーン切り替え依頼
			sceneManager_->ChengeScene("GAMEPLAY");
		}
		break;
	}

}

void TitleScene::Draw()
{

	//Spriteの描画準備。Spriteの描画に共通のグラフィックスコマンドを積む
	spritePlatform_->PreDraw();
	

	//sprite_->Draw();

	fade_->Draw();
}

void TitleScene::Finalize()
{

}
