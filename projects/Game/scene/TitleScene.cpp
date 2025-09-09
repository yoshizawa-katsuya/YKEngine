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

	fade_ = std::make_unique<Fade>();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 0.5f);


	for (int i = 0; i < 4; i++) {
		std::string path = "Resources/scene/title0" + std::to_string(i + 1) + ".png";
		titles[i] = std::make_unique<Sprite>();
		titles[i]->Initialize(TextureManager::GetInstance()->Load(path));
		titles[i]->SetPosition({ 0.0f, 0.0f });
	}

	menuState = 0;
	frameCount = 0;
	blinkIndex = 0;
}

void TitleScene::Update()
{

#ifdef _DEBUG

	ImGui::Begin("Window");
	ImGui::Text("Title");
	if (ImGui::Button("Go to GameScene"))
	{
		phase_ = Phase::kEnd;
		fade_->Start(Fade::Status::FadeOut, 0.5f);
	}
	ImGui::End();

#endif // _DEBUG

	if (input_->TriggerKey(DIK_W)) {
		menuState = 0;
	} else if (input_->TriggerKey(DIK_S)) {
		menuState = 1;
	}

	frameCount++;
	if (frameCount > 30) {
		blinkIndex = 1 - blinkIndex;
		frameCount = 0;
	}

	switch (phase_)
	{
	case Phase::kStart:

		UpdateStart();
		break;
	case Phase::kMain:

		UpdateMain();
		break;
	case Phase::kEnd:

		UpdateEnd();
		break;
	default:
		break;
	}

}

void TitleScene::Draw()
{

	//Spriteの描画準備。Spriteの描画に共通のグラフィックスコマンドを積む
	spritePlatform_->PreDraw();

	if (menuState == 0) {
		titles[blinkIndex]->Draw();
	} else if (menuState == 1) {
		titles[2 + blinkIndex]->Draw();
	}

	fade_->Draw();
}

void TitleScene::Finalize()
{

}

void TitleScene::UpdateStart()
{
	fade_->Update();
	if (fade_->IsFinished())
	{
		fade_->Stop();
		phase_ = Phase::kMain;
	}
}

void TitleScene::UpdateMain()
{
	if (menuState == 0) {
		if (input_->TriggerKey(DIK_SPACE) || input_->TriggerButton(XINPUT_GAMEPAD_A)) {
			phase_ = Phase::kEnd;
			fade_->Start(Fade::Status::FadeOut, 0.5f);
		}
	} else if (menuState == 1) {
		if (input_->TriggerKey(DIK_SPACE) || input_->TriggerButton(XINPUT_GAMEPAD_A)) {
			PostQuitMessage(0);
		}
	}
}

void TitleScene::UpdateEnd()
{
	fade_->Update();
	if (fade_->IsFinished()) {
		//fade_->Stop();
		//シーン切り替え依頼
		sceneManager_->ChengeScene("GameScene");
	}
}
