#include "TitleScene.h"
#include "SceneManager.h"

#ifdef USE_IMGUI
#include "imgui/imgui.h"
#endif // USE_IMGUI

using namespace YKEngine;

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
	transitionSprite_.Initialize(TextureManager::GetInstance()->Load("./resources/brickLoad.png"));
	transitionSprite_.SetMaskTexture(TextureManager::GetInstance()->Load("./resources/brickMask2.png"));
	transitionSprite_.SetProgress(progress_);
}

void TitleScene::Update()
{

#ifdef USE_IMGUI

	ImGui::Begin("Window");
	ImGui::Text("Title");
	ImGui::Text("%s",state_ == State::START ? "Press Space Key" : state_ == State::OPTIONS ? "Left/right arrow keys Select Difficulty Press Space Key" : "Go To GameScene...");
	ImGui::Text("State: %s", state_ == State::START ? "START" : state_ == State::OPTIONS ? "OPTIONS" : "EXIT");
	ImGui::Text("Difficulty: %s", difficulty_ == Difficulty::EASY ? "EASY" : difficulty_ == Difficulty::NORMAL ? "NORMAL" : "HARD");
	ImGui::SliderFloat("progress", &progress_, 0.0f, 1.0f);
	ImGui::End();

#endif // USE_IMGUI
	switch (state_)
	{
	case State::START:
		//スタート画面の更新処理
		if (input_->TriggerKey(DIK_SPACE)) {
			// START状態でスペースキーが押されたときの処理
			// ステートをOPTIONSに変更
			state_ = State::OPTIONS;
			//シーン切り替え依頼
			//sceneManager_->ChengeScene("GameScene");
		}
		break;
	case State::OPTIONS:
		//オプション画面の更新処理
		// 難易度と音量の設定
		// 十字キー左右で難易度の選択(初期選択EASY)
		if (difficulty_ == Difficulty::EASY) {
			if (input_->TriggerKey(DIK_SPACE)) {
				// 難易度がEASYのときスペースキーが押されたときの処理
				// ステートをEXITに変更
				state_ = State::EXIT;
			}
			if (input_->TriggerKey(DIK_RIGHT)) {
				// 難易度がEASYのとき右キーが押されたときの処理
				// 難易度をNORMALに変更
				difficulty_ = Difficulty::NORMAL;
			}
		}
		else if (difficulty_ == Difficulty::NORMAL) {
			if (input_->TriggerKey(DIK_SPACE)) {
				// 難易度がNORMALのときスペースキーが押されたときの処理
				// ステートをEXITに変更
				state_ = State::EXIT;
			}
			if (input_->TriggerKey(DIK_RIGHT)) {
				// 難易度がNORMALのとき右キーが押されたときの処理
				// 難易度をHARDに変更
				difficulty_ = Difficulty::HARD;
			}
			if (input_->TriggerKey(DIK_LEFT)) {
				// 難易度がNORMALのとき左キーが押されたときの処理
				// 難易度をEASYに変更
				difficulty_ = Difficulty::EASY;
			}
		}
		else if (difficulty_ == Difficulty::HARD) {
			if (input_->TriggerKey(DIK_SPACE)) {
				// 難易度がHARDのときスペースキーが押されたときの処理
				// ステートをEXITに変更
				state_ = State::EXIT;
			}
			if (input_->TriggerKey(DIK_LEFT)) {
				// 難易度がHARDのとき左キーが押されたときの処理
				// 難易度をNORMALに変更
				difficulty_ = Difficulty::NORMAL;
			}
		}

		break;
	case State::EXIT:
		// 終了処理
		 // 0 → 1へ進行
		transitionT_ += 1.0f / duration_;

		// clamp
		transitionT_ = std::clamp(transitionT_, 0.0f, 1.0f);

		// EaseOutQuint
		float t = 1.0f - powf(1.0f - transitionT_, 5.0f);

		// progress反映
		progress_ = 1.0f - t;

		// 終了
		if (transitionT_ >= 1.0f)
		{
			sceneManager_->SetDifficulty(static_cast<int>(difficulty_));
			sceneManager_->ChengeScene("GameScene");
		}

		break;
	}
	transitionSprite_.SetProgress(progress_);
}

void TitleScene::Draw()
{

	//Spriteの描画準備。Spriteの描画に共通のグラフィックスコマンドを積む
	spritePlatform_->PreDraw();
	transitionSprite_.Draw();
}

void TitleScene::Finalize()
{

}
