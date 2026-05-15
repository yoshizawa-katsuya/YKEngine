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


	titleSprite_ = std::make_unique<YKEngine::Sprite>();
	titleSprite_->Initialize(YKEngine::TextureManager::GetInstance()->Load("Resources/title/title.png"));
	titleSprite_->SetPosition({ 640.0f,320.0f });
	titleSprite_->SetAnchorPoint({ 0.5f,0.5f });
}

void TitleScene::Update()
{

#ifdef USE_IMGUI

    static float titlePos[2] = { 0.0f, 0.0f };
    // 現在のスプライト位置を取得（初回のみ）
    if (titlePos[0] == 0.0f && titlePos[1] == 0.0f && titleSprite_) {
        auto pos = titleSprite_->GetPosition();
        titlePos[0] = pos.x;
        titlePos[1] = pos.y;
    }

    ImGui::Begin("Window");
    ImGui::Text("Title");
    ImGui::Text("%s",state_ == State::START ? "Press Space Key" : state_ == State::OPTIONS ? "Left/right arrow keys Select Difficulty Press Space Key" : "Go To GameScene...");
    ImGui::Text("State: %s", state_ == State::START ? "START" : state_ == State::OPTIONS ? "OPTIONS" : "EXIT");
    ImGui::Text("Difficulty: %s", difficulty_ == Difficulty::EASY ? "EASY" : difficulty_ == Difficulty::NORMAL ? "NORMAL" : "HARD");
    ImGui::SliderFloat2("TitleSprite Position", titlePos, -640.0f, 640.0f, "%.1f");
    ImGui::End();

    // ImGuiで値が変更されたらスプライトの位置を更新
    if (titleSprite_) {
        titleSprite_->SetPosition({ titlePos[0], titlePos[1] });
    }

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
		// 難易度をシーンマネージャに渡す
		sceneManager_->SetDifficulty(static_cast<int>(difficulty_));
		//シーン切り替え依頼
		sceneManager_->ChengeScene("GameScene");
		break;
	}

}

void TitleScene::Draw()
{

	//Spriteの描画準備。Spriteの描画に共通のグラフィックスコマンドを積む
	spritePlatform_->PreDraw();

	//タイトルスプライトの描画
	titleSprite_->Draw();

}

void TitleScene::Finalize()
{

}
