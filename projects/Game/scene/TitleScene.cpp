#include "TitleScene.h"
#include "imgui/imgui.h"
#include "SceneManager.h"

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
	
	textureHandleTitle_ = TextureManager::GetInstance()->Load("./resources/Title.png");
	textuHandleSetsumei_ = TextureManager::GetInstance()->Load("./resources/Setsumei01.png");
	textuHandleSetsumei02_ = TextureManager::GetInstance()->Load("./resources/Setsumei02.png");
	textuHandleSetsumei03_ = TextureManager::GetInstance()->Load("./resources/Setsumei03.png");

	spriteTitle_ = std::make_unique<Sprite>();
	spriteTitle_->Initialize(textureHandleTitle_);

	fade_ = std::make_unique<Fade>();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 0.5f);
}

void TitleScene::Update()
{

#ifdef _DEBUG
	
	ImGui::Begin("Window");
	ImGui::Text("Title");
	ImGui::End();
	
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
		if (input_->TriggerKey(DIK_SPACE) || input_->TriggerButton(XINPUT_GAMEPAD_A)) {
			if (phase2_ == 0) {
				phase2_++;
				spriteTitle_->SetTexture(textuHandleSetsumei_);
			}
			else if (phase2_ == 1) {
				phase2_++;
				spriteTitle_->SetTexture(textuHandleSetsumei02_);
			}
			else if (phase2_ == 2) {
				phase2_++;
				spriteTitle_->SetTexture(textuHandleSetsumei03_);
			}
			else {
				fade_->Start(Fade::Status::FadeOut, 0.5f);
				phase_ = Phase::kFadoOut;
				
			}
		}
		break;

	case TitleScene::Phase::kFadoOut:
		fade_->Update();
		if (fade_->IsFinished()) {
			//fade_->Stop();
			//シーン切り替え依頼
			sceneManager_->ChengeScene("GameScene");
		}
		break;
	}

	
	
}

void TitleScene::Draw()
{

	//Spriteの描画準備。Spriteの描画に共通のグラフィックスコマンドを積む
	spritePlatform_->PreDraw();
	
	spriteTitle_->Draw();

	fade_->Draw();
}

void TitleScene::Finalize()
{

}
