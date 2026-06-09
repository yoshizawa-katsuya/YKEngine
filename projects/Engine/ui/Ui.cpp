#include "Ui.h"
#include "TextureManager.h"
#include "Input.h"
#include <algorithm>
#include <Lerp.h>

// 初期化
void Ui::Initialize() {

	//タイトルスプライト生成
	titleSprite_ = std::make_unique<YKEngine::Sprite>();
	titleSprite_->Initialize(YKEngine::TextureManager::GetInstance()->Load("Resources/title/title.png"));
	titleSprite_->SetPosition({ 640.0f,320.0f });
	titleSprite_->SetAnchorPoint({ 0.5f,0.5f });

    //タイトルスペース生成
	titlePushSprite_ = std::make_unique<YKEngine::Sprite>();
	titlePushSprite_->Initialize(YKEngine::TextureManager::GetInstance()->Load("Resources/title/space.png"));
	titlePushSprite_->SetPosition({ 640.0f, 500.0f });
	titlePushSprite_->SetAnchorPoint({ 0.5f, 0.5f });
	
	//スコアテクスチャ読み込み
	for (int i = 0; i < 10; i++) {
		numberTextures_[i] =
			YKEngine::TextureManager::GetInstance()->Load("Resources/score/" + std::to_string(i) + ".png");
	}

	//スコア生成
	for (int i = 0; i < kMaxDigits; i++) {
		scoreSprites_[i] = std::make_unique<YKEngine::Sprite>();
		scoreSprites_[i]->Initialize(numberTextures_[0]);
		scoreSprites_[i]->SetPosition({ 50.0f + i * 32.0f, 50.0f });
	}

	//ライフ生成
	uint32_t lifeTexture = YKEngine::TextureManager::GetInstance()->Load("Resources/ui/life.png");

	for (int i = 0; i < kMaxLife; i++) {
		lifeSprites_[i] = std::make_unique<YKEngine::Sprite>();
		lifeSprites_[i]->Initialize(lifeTexture);
		YKEngine::Vector2 pos = { 60.0f + i * 50.0f, 120.0f };

		lifeSprites_[i]->SetPosition(pos);
		lifeSprites_[i]->SetSize({ 40.0f, 40.0f });
	}

	//操作説明スプライト生成
	ctrlSprite_ = std::make_unique<YKEngine::Sprite>();
	ctrlSprite_->Initialize(YKEngine::TextureManager::GetInstance()->Load("Resources/ui/ctrl.png"));

	//ポーズUIスプライト生成
	pauseUiSprite_ = std::make_unique<YKEngine::Sprite>();
	pauseUiSprite_->Initialize(YKEngine::TextureManager::GetInstance()->Load("Resources/ui/pause.png"));
	pauseUiSprite_->SetPosition({ 1130.0f, 70.0f });
	pauseUiSprite_->SetSize({ 300.0f, 70.0f });
	pauseUiSprite_->SetAnchorPoint({ 0.5f, 0.5f });
	//ポーズ画面スプライト生成
	pauseSprite_ = std::make_unique<YKEngine::Sprite>();
	pauseSprite_->Initialize(YKEngine::TextureManager::GetInstance()->Load("Resources/ui/pauseMenu.png"));
	pauseSprite_->SetPosition({ 640.0f, 360.0f });
	pauseSprite_->SetSize({ 0.0f, 0.0f });
	pauseSprite_->SetAnchorPoint({ 0.5f, 0.5f });
	// ポーズ中スプライト生成
	pausingSprite_ = std::make_unique<YKEngine::Sprite>();
	pausingSprite_->Initialize(YKEngine::TextureManager::GetInstance()->Load("Resources/ui/pauseText.png"));
	pausingSprite_->SetPosition({ 640.0f, 200.0f });
	pausingSprite_->SetSize({ 0.0f, 0.0f });
	pausingSprite_->SetAnchorPoint({ 0.5f, 0.5f });
	// ゲームに戻るスプライト生成
	resumeSprite_ = std::make_unique<YKEngine::Sprite>();
	resumeSprite_->Initialize(YKEngine::TextureManager::GetInstance()->Load("Resources/ui/resume.png"));
	resumeSprite_->SetPosition({ 640.0f, 300.0f });
	resumeSprite_->SetSize({ 0.0f, 0.0f });
	resumeSprite_->SetAnchorPoint({ 0.5f, 0.5f });
	// リトライスプライト生成
	retryUISprite_ = std::make_unique<YKEngine::Sprite>();
	retryUISprite_->Initialize(YKEngine::TextureManager::GetInstance()->Load("Resources/ui/pauseRetry.png"));
	retryUISprite_->SetPosition({ 640.0f, 400.0f });
	retryUISprite_->SetSize({ 0.0f, 0.0f });
	retryUISprite_->SetAnchorPoint({ 0.5f, 0.5f });
	// タイトルに戻るスプライト生成
	toTitleSprite_ = std::make_unique<YKEngine::Sprite>();
	toTitleSprite_->Initialize(YKEngine::TextureManager::GetInstance()->Load("Resources/ui/title.png"));
	toTitleSprite_->SetPosition({ 640.0f, 500.0f });
	toTitleSprite_->SetSize({ 0.0f, 0.0f });
	toTitleSprite_->SetAnchorPoint({ 0.5f, 0.5f });
	//ジャッジエフェクトテクスチャ読み込み
	goodTexture_ =
		YKEngine::TextureManager::GetInstance()->Load("Resources/ui/good.png");

	greatTexture_ =
		YKEngine::TextureManager::GetInstance()->Load("Resources/ui/great.png");

	perfectTexture_ =
		YKEngine::TextureManager::GetInstance()->Load("Resources/ui/perfect.png");
	//ジャッジエフェクトスプライト生成
	goodSprite_ = std::make_unique<YKEngine::Sprite>();
	goodSprite_->Initialize(goodTexture_);
	goodSprite_->SetAnchorPoint({ 0.5f,0.5f });

	greatSprite_ = std::make_unique<YKEngine::Sprite>();
	greatSprite_->Initialize(greatTexture_);
	greatSprite_->SetAnchorPoint({ 0.5f,0.5f });

	perfectSprite_ = std::make_unique<YKEngine::Sprite>();
	perfectSprite_->Initialize(perfectTexture_);
	perfectSprite_->SetAnchorPoint({ 0.5f,0.5f });

	//画面枠テクスチャ
	uint32_t frameTexture =
		YKEngine::TextureManager::GetInstance()->Load("Resources/ui/framebar.png");

	//画面枠スプライト生成
	frameSprite_ = std::make_unique<YKEngine::Sprite>();
	frameSprite_->Initialize(frameTexture);
	frameSprite_->SetPosition({ 640.0f,360.0f });
	frameSprite_->SetSize({ 1280.0f,720.0f });
	frameSprite_->SetAnchorPoint({ 0.5f,0.5f });
	frameSprite_->SetColor({ 1,1,1,0 });

	//難易度テクスチャ読み込み
	easyTexture_ = YKEngine::TextureManager::GetInstance()->Load("Resources/title/easy.png");
	normalTexture_ = YKEngine::TextureManager::GetInstance()->Load("Resources/title/normal.png");
	hardTexture_ = YKEngine::TextureManager::GetInstance()->Load("Resources/title/hard.png");
	//難易度スプライト生成
	easySprite_ = std::make_unique<YKEngine::Sprite>();
	easySprite_->Initialize(easyTexture_);
	easySprite_->SetPosition({ 640.0f, 130.0f });
	easySprite_->SetAnchorPoint({ 0.5f,0.5f });

	normalSprite_ = std::make_unique<YKEngine::Sprite>();
	normalSprite_->Initialize(normalTexture_);
	normalSprite_->SetPosition({ 640.0f, 310.0f });
	normalSprite_->SetAnchorPoint({ 0.5f,0.5f });

	hardSprite_ = std::make_unique<YKEngine::Sprite>();
	hardSprite_->Initialize(hardTexture_);
	hardSprite_->SetPosition({ 640.0f, 490.0f });
	hardSprite_->SetAnchorPoint({ 0.5f,0.5f });

	//リトライ・バックタイトルテクスチャ読み込み
	retryTexture_ = YKEngine::TextureManager::GetInstance()->Load("Resources/ui/retry.png");
	titleTexture_ = YKEngine::TextureManager::GetInstance()->Load("Resources/ui/backtitle.png");
	//リトライスプライト生成
	retrySprite_ = std::make_unique<YKEngine::Sprite>();
	retrySprite_->Initialize(retryTexture_);
	retrySprite_->SetPosition({ 380.0f, 600.0f });
	retrySprite_->SetAnchorPoint({ 0.5f,0.5f });
	//バックタイトルスプライト生成
	backtitleSprite_ = std::make_unique<YKEngine::Sprite>();
	backtitleSprite_->Initialize(titleTexture_);
	backtitleSprite_->SetPosition({ 890.0f, 600.0f });
	backtitleSprite_->SetAnchorPoint({ 0.5f,0.5f });
	//ゲームオーバーテクスチャ読み込み
	gameoverTexture_ = YKEngine::TextureManager::GetInstance()->Load("Resources/gameover/gameover.png");
	//ゲームオーバースプライト生成
	gameoverSprite_ = std::make_unique<YKEngine::Sprite>();
	gameoverSprite_->Initialize(gameoverTexture_);
	//ゲームクリアテクスチャ読み込み
	gameclearTexture_ = YKEngine::TextureManager::GetInstance()->Load("Resources/gameclear/gameclear.png");
	//ゲームクリアスプライト生成
	gameclearSprite_ = std::make_unique<YKEngine::Sprite>();
	gameclearSprite_->Initialize(gameclearTexture_);
}
//更新
void Ui::Update() {
	//入力処理
	HandleInput();
	//アニメーション更新
	UpdateAnimation();
	//桁更新
	UpdateDigits();
	//ライフ減少処理
	HandleLifeInput();
	//ポーズメニュー更新
	UpdatePauseMenu();
	//ジャッジエフェクト入力処理
	HandleJudgeInput();
	//ジャッジエフェクト更新
	UpdateJudgeEffect();
	//画面枠発光更新
	UpdateFrameGlow();
	//ライフ更新
	UpdateLifeBlink();
	//タイトルスペース更新
	UpdateTitleSpace();
	//難易度選択のスケール更新
	difficultyScaleTimer_ += 1.0f / 60.0f;
	//ゲームオーバー選択のスケール更新
	ScaleTimer_ += 1.0f / 60.0f;
	//デバック
	Debug();
}
//描画
void Ui::Draw() {

	//スコア描画
	for (int i = 0; i < kMaxDigits; i++) {
		scoreSprites_[i]->Draw();
	}

	//ライフ描画
	for (int i = 0; i < life_; i++) {
		lifeSprites_[i]->Draw();
	}

	//操作説明描画
	ctrlSprite_->Draw();

	//ポーズUI描画
	pauseUiSprite_->Draw();

	//ポーズ画面描画
	if (pauseScale_ > 0.01f) {
		pauseSprite_->Draw();
		pausingSprite_->Draw();
		resumeSprite_->Draw();
		retryUISprite_->Draw();
		toTitleSprite_->Draw();
	}
	//ジャッジエフェクト描画
	if (isJudgePlaying_) {
		auto sprite = GetCurrentJudgeSprite();
		if (sprite) {
			sprite->Draw();
		}
	}
	//画面枠描画
	if (isFrameGlow_) {
		frameSprite_->Draw();
	}
}
//タイトル描画
void Ui::DrawTitle() {
	//タイトル描画
	titleSprite_->Draw();
	//タイトルスペース描画
	titlePushSprite_->Draw();
}
//難易度選択描画
void Ui::DrawSelect() {
	float scale = 1.0f + std::sin(difficultyScaleTimer_ * 6.0f) * 0.1f;

	//EASY
	if (selectedDifficulty_ == 0) {
		easySprite_->SetSize({ 512.0f * scale,128.0f * scale });
	}
	else {
		easySprite_->SetSize({ 512.0f,128.0f });
	}
	//NORMAL
	if (selectedDifficulty_ == 1) {
		normalSprite_->SetSize({ 512.0f * scale,128.0f * scale });
	}
	else {
		normalSprite_->SetSize({ 512.0f,128.0f });
	}
	//HARD
	if (selectedDifficulty_ == 2) {
		hardSprite_->SetSize({ 512.0f * scale,128.0f * scale });
	}
	else {
		hardSprite_->SetSize({ 512.0f,128.0f });
	}

	easySprite_->Draw();
	normalSprite_->Draw();
	hardSprite_->Draw();
}
//ゲームオーバー画面描画
void Ui::DrawGameOver() {

	float scale = 1.0f + std::sin(ScaleTimer_ * 6.0f) * 0.1f;

	if (gameOverSelect_ == 0) {
		retrySprite_->SetSize({ 400.0f * scale,100.0f * scale });

		backtitleSprite_->SetSize({ 320.0f,80.0f });
	}
	else {
		retrySprite_->SetSize({ 320.0f,80.0f });

		backtitleSprite_->SetSize({ 400.0f * scale,100.0f * scale });
	}
	//描画
	retrySprite_->Draw();
	backtitleSprite_->Draw();
	gameoverSprite_->Draw();
}
//ゲームクリア画面描画
void Ui::DrawGameClear() {
	float scale = 1.0f + std::sin(ScaleTimer_ * 6.0f) * 0.1f;

	if (gameClearSelect_ == 0) {
		retrySprite_->SetSize({ 400.0f * scale,100.0f * scale });

		backtitleSprite_->SetSize({ 320.0f,80.0f });
	} else {
		retrySprite_->SetSize({ 320.0f,80.0f });

		backtitleSprite_->SetSize({ 400.0f * scale,100.0f * scale });
	}

	//描画
	retrySprite_->Draw();
	backtitleSprite_->Draw();
	gameclearSprite_->Draw();
}

//スコア加算
void Ui::AddScore(int value) {
	score_ += value;
	//スコア上限
	if (score_ > kMaxScore) {
		score_ = kMaxScore;
	}

	targetScore_ = score_;
	isAnimating_ = true;
}
//ライフ減少
void Ui::DecreaseLife() {
	if (life_ <= 0)return;

	blinkLifeIndex_ = life_ - 1;

	isLifeBlink_ = true;
	lifeBlinkTimer_ = 0.0f;
}
//ジャッジエフェクト開始
void Ui::StartJudgeEffect(JudgeType type) {
	judgeType_ = type;

	isJudgePlaying_ = true;
	judgeTimer_ = 0.0f;
	judgeScale_ = 0.3f;
	judgeAlpha_ = 1.0f;
	judgePos_ = { 640.0f,250.0f };
	isFrameGlow_ = true;
	frameGlowTimer_ = 0.0f;
}
//画面枠発光停止
void Ui::StopFrameGlow() {
	isFrameGlow_ = false;

	if (frameSprite_) {
		frameSprite_->SetColor({ 1,1,1,0 });
	}
}
//ゲームスコア加算
void Ui::AddGameScore(int value){
	AddScore(value);
}
//ライフ減少
void Ui::DamageLife() {
	DecreaseLife();
}
//ジャッジエフェクト再生
void Ui::PlayJudgeEffect(JudgeType type) {
	StartJudgeEffect(type);
}

//デバック
void Ui::Debug() {

#ifdef USE_IMGUI

	//ポーズUI
	if (pauseUiSprite_) {
		ImGui::Begin("Pause UI Sprite");
		ImVec2 pos = { pauseUiSprite_->GetPosition().x, pauseUiSprite_->GetPosition().y };
		if (ImGui::DragFloat2("Position", (float*)&pos, 1.0f)) {
			pauseUiSprite_->SetPosition({ pos.x, pos.y });
		}
		ImVec2 scale = { pauseUiSprite_->GetSize().x , pauseUiSprite_->GetSize().y };
		if (ImGui::DragFloat2("Scale", (float*)&scale, 1.0f, 0.0f, 10000.0)) {
			pauseUiSprite_->SetSize({ scale.x, scale.y });
		}
		float rotation = pauseUiSprite_->GetRotation();
		if (ImGui::DragFloat("Rotation", &rotation, 1.0f, -360.0f, 360.0f)) {
			pauseUiSprite_->SetRotation(rotation);
		}
		ImGui::End();
	}

    // タイトルスペースUI
    if (titlePushSprite_) {
        ImGui::Begin("Title Push Space Sprite");
        ImVec2 pos = { titlePushSprite_->GetPosition().x, titlePushSprite_->GetPosition().y };
        if (ImGui::DragFloat2("Position", (float*)&pos, 1.0f)) {
            titlePushSprite_->SetPosition({ pos.x, pos.y });
        }
        ImVec2 scale = { titlePushSprite_->GetSize().x , titlePushSprite_->GetSize().y };
        if (ImGui::DragFloat2("Scale", (float*)&scale, 1.0f, 0.0f, 10000.0f)) {
            titlePushSprite_->SetSize({ scale.x, scale.y });
        }
        float rotation = titlePushSprite_->GetRotation();
        if (ImGui::DragFloat("Rotation", &rotation, 1.0f, -360.0f, 360.0f)) {
            titlePushSprite_->SetRotation(rotation);
        }
        ImGui::End();
    }

	// ライフUI
	ImGui::Begin("Life UI");
	static ImVec2 lifePos = { 50.0f, 120.0f };
	static ImVec2 lifeSize = { 40.0f, 40.0f };
	bool changed = false;
	if (ImGui::DragFloat2("Life Position", (float*)&lifePos, 1.0f)) {
		changed = true;
	}
	if (ImGui::DragFloat2("Life Size", (float*)&lifeSize, 1.0f, 0.0f, 1000.0f)) {
		changed = true;
	}
	if (changed) {
		for (int i = 0; i < kMaxLife; i++) {
			lifeSprites_[i]->SetPosition({ lifePos.x + i * (lifeSize.x + 10.0f), lifePos.y });
			lifeSprites_[i]->SetSize({ lifeSize.x, lifeSize.y });
		}
	}
	ImGui::End();

	//ポーズ画面
	if (pauseSprite_) {
		ImGui::Begin("Pause Sprite");
		ImGui::Text("PauseMenu = %d", (int)pauseMenu_);
		ImGui::Text("PauseIndex = %d", pauseSelectIndex_);
		ImGui::Text("IsShowPause = %d", isShowPause_);
		ImVec2 pos = { pauseSprite_->GetPosition().x, pauseSprite_->GetPosition().y };
		if (ImGui::DragFloat2("Position", (float*)&pos, 1.0f)) {
			pauseSprite_->SetPosition({ pos.x, pos.y });
		}
		ImVec2 scale = { pauseSprite_->GetSize().x, pauseSprite_->GetSize().y };
		if (ImGui::DragFloat2("Scale", (float*)&scale, 1.0f, 0.0f, 1000.0f)) {
			pauseSprite_->SetSize({ scale.x, scale.y });
		}
		float rotation = pauseSprite_->GetRotation();
		if (ImGui::DragFloat("Rotation", &rotation, 1.0f, -360.0f, 360.0f)) {
			pauseSprite_->SetRotation(rotation);
		}
		ImVec2 pausingPos = { pausingSprite_->GetPosition().x, pausingSprite_->GetPosition().y };
		if (ImGui::DragFloat2("Pausing Position", (float*)&pausingPos, 1.0f)) {
			pausingSprite_->SetPosition({ pausingPos.x, pausingPos.y });
		}
		ImVec2 pausingScale = { pausingSprite_->GetSize().x, pausingSprite_->GetSize().y };
		if (ImGui::DragFloat2("Pausing Scale", (float*)&pausingScale, 1.0f, 0.0f, 1000.0f)) {
			pausingSprite_->SetSize({ pausingScale.x, pausingScale.y });
		}
		float pausingRotation = pausingSprite_->GetRotation();
		if (ImGui::DragFloat("Pausing Rotation", &pausingRotation, 1.0f, -360.0f, 360.0f)) {
			pausingSprite_->SetRotation(pausingRotation);
		}
		ImVec2 resumePos = { resumeSprite_->GetPosition().x, resumeSprite_->GetPosition().y };
		if (ImGui::DragFloat2("Resume Position", (float*)&resumePos, 1.0f)) {
			resumeSprite_->SetPosition({ resumePos.x, resumePos.y });
		}
		ImVec2 resumeScale = { resumeSprite_->GetSize().x, resumeSprite_->GetSize().y };
		if (ImGui::DragFloat2("Resume Scale", (float*)&resumeScale, 1.0f, 0.0f, 1000.0f)) {
			resumeSprite_->SetSize({ resumeScale.x, resumeScale.y });
		}
		float resumeRotation = resumeSprite_->GetRotation();
		if (ImGui::DragFloat("Resume Rotation", &resumeRotation, 1.0f, -360.0f, 360.0f)) {
			resumeSprite_->SetRotation(resumeRotation);
		}
		ImVec2 toTitlePos = { toTitleSprite_->GetPosition().x, toTitleSprite_->GetPosition().y };
		if (ImGui::DragFloat2("To Title Position", (float*)&toTitlePos, 1.0f)) {
			toTitleSprite_->SetPosition({ toTitlePos.x, toTitlePos.y });
		}
		ImVec2 toTitleScale = { toTitleSprite_->GetSize().x, toTitleSprite_->GetSize().y };
		if (ImGui::DragFloat2("To Title Scale", (float*)&toTitleScale, 1.0f, 0.0f, 1000.0f)) {
			toTitleSprite_->SetSize({ toTitleScale.x, toTitleScale.y });
		}
		float toTitleRotation = toTitleSprite_->GetRotation();
		if (ImGui::DragFloat("To Title Rotation", &toTitleRotation, 1.0f, -360.0f, 360.0f)) {
			toTitleSprite_->SetRotation(toTitleRotation);
		}
		ImGui::End();
	}

	// 画面枠の位置とサイズを操作
	if (frameSprite_) {
		ImGui::Begin("Frame Sprite");
		ImVec2 pos = { frameSprite_->GetPosition().x, frameSprite_->GetPosition().y };
		if (ImGui::DragFloat2("Position", (float*)&pos, 1.0f)) {
			frameSprite_->SetPosition({ pos.x, pos.y });
		}
		ImVec2 scale = { frameSprite_->GetSize().x, frameSprite_->GetSize().y };
		if (ImGui::DragFloat2("Scale", (float*)&scale, 1.0f, 0.0f, 2000.0f)) {
			frameSprite_->SetSize({ scale.x, scale.y });
		}
		ImGui::End();
	}

	// 難易度スプライトの位置とサイズを操作
	if (easySprite_ && normalSprite_ && hardSprite_) {
		ImGui::Begin("Difficulty Sprites");

		// easy
		ImVec2 easyPos = { easySprite_->GetPosition().x, easySprite_->GetPosition().y };
		if (ImGui::DragFloat2("Easy Position", (float*)&easyPos, 1.0f)) {
			easySprite_->SetPosition({ easyPos.x, easyPos.y });
		}
		ImVec2 easySize = { easySprite_->GetSize().x, easySprite_->GetSize().y };
		if (ImGui::DragFloat2("Easy Size", (float*)&easySize, 1.0f, 0.0f, 1000.0f)) {
			easySprite_->SetSize({ easySize.x, easySize.y });
		}

		// normal
		ImVec2 normalPos = { normalSprite_->GetPosition().x, normalSprite_->GetPosition().y };
		if (ImGui::DragFloat2("Normal Position", (float*)&normalPos, 1.0f)) {
			normalSprite_->SetPosition({ normalPos.x, normalPos.y });
		}
		ImVec2 normalSize = { normalSprite_->GetSize().x, normalSprite_->GetSize().y };
		if (ImGui::DragFloat2("Normal Size", (float*)&normalSize, 1.0f, 0.0f, 1000.0f)) {
			normalSprite_->SetSize({ normalSize.x, normalSize.y });
		}

		// hard
		ImVec2 hardPos = { hardSprite_->GetPosition().x, hardSprite_->GetPosition().y };
		if (ImGui::DragFloat2("Hard Position", (float*)&hardPos, 1.0f)) {
			hardSprite_->SetPosition({ hardPos.x, hardPos.y });
		}
		ImVec2 hardSize = { hardSprite_->GetSize().x, hardSprite_->GetSize().y };
		if (ImGui::DragFloat2("Hard Size", (float*)&hardSize, 1.0f, 0.0f, 1000.0f)) {
			hardSprite_->SetSize({ hardSize.x, hardSize.y });
		}

		ImGui::End();
	}

	// ゲームオーバー用スプライト（リトライ / バックタイトル）の位置・サイズ・回転を操作
	if (retrySprite_ || backtitleSprite_) {
		ImGui::Begin("Game Over Sprites");

		if (retrySprite_) {
			ImVec2 pos = { retrySprite_->GetPosition().x, retrySprite_->GetPosition().y };
			if (ImGui::DragFloat2("Retry Position", (float*)&pos, 1.0f)) {
				retrySprite_->SetPosition({ pos.x, pos.y });
			}
			ImVec2 size = { retrySprite_->GetSize().x, retrySprite_->GetSize().y };
			if (ImGui::DragFloat2("Retry Size", (float*)&size, 1.0f, 0.0f, 2000.0f)) {
				retrySprite_->SetSize({ size.x, size.y });
			}
			float rot = retrySprite_->GetRotation();
			if (ImGui::DragFloat("Retry Rotation", &rot, 1.0f, -360.0f, 360.0f)) {
				retrySprite_->SetRotation(rot);
			}
		}

		if (backtitleSprite_) {
			ImVec2 pos = { backtitleSprite_->GetPosition().x, backtitleSprite_->GetPosition().y };
			if (ImGui::DragFloat2("BackTitle Position", (float*)&pos, 1.0f)) {
				backtitleSprite_->SetPosition({ pos.x, pos.y });
			}
			ImVec2 size = { backtitleSprite_->GetSize().x, backtitleSprite_->GetSize().y };
			if (ImGui::DragFloat2("BackTitle Size", (float*)&size, 1.0f, 0.0f, 2000.0f)) {
				backtitleSprite_->SetSize({ size.x, size.y });
			}
			float rot = backtitleSprite_->GetRotation();
			if (ImGui::DragFloat("BackTitle Rotation", &rot, 1.0f, -360.0f, 360.0f)) {
				backtitleSprite_->SetRotation(rot);
			}
		}

		ImGui::End();
	}
#endif // USE_IMGUI
}
//ポーズメニューの取得
Ui::PauseMenu Ui::GetPauseMenu() const {
	return pauseMenu_;
}

//入力処理
void Ui::HandleInput() {

	//ポーズ画面表示仮
	if (YKEngine::Input::GetInstance()->TriggerKey(DIK_Q)) {
		isShowPause_ = !isShowPause_;
		if (isShowPause_) {
			pauseMenu_ = PauseMenu::None;
		}
	}
	if (isShowPause_) {
		if (YKEngine::Input::GetInstance()->TriggerKey(DIK_SPACE)) {

			switch (pauseSelectIndex_) {

			case 0:
				pauseMenu_ = PauseMenu::Resume;
				isShowPause_ = false;
				break;

			case 1:
				pauseMenu_ = PauseMenu::Retry;
				break;

			case 2:
				pauseMenu_ = PauseMenu::ToTitle;
				break;
			}
		}
	}
}

//アニメーション更新
void Ui::UpdateAnimation() {

	if (isAnimating_) {
		if (displayScore_ < targetScore_) {
			displayScore_ += 15;

			if (displayScore_ > targetScore_) {
				displayScore_ = targetScore_;
			}
		} else {
			isAnimating_ = false;
		}
	} else {
		displayScore_ = score_;
	}
}

//桁更新
void Ui::UpdateDigits() {

	int temp = displayScore_;

	//桁分解
	for (int i = kMaxDigits - 1; i >= 0; i--) {
		digits_[i] = temp % 10;
		temp /= 10;
	}

	//テクスチャ反映
	for (int i = 0; i < kMaxDigits; i++) {
		scoreSprites_[i]->SetTexture(numberTextures_[digits_[i]]);
	}
}
//ライフ減少(仮実装)
void Ui::HandleLifeInput() {
	if (YKEngine::Input::GetInstance()->TriggerKey(DIK_L)) {
		DecreaseLife();
	}
}
//ポーズメニュー更新
void Ui::UpdatePauseMenu() {
	if (isShowPause_) {
		if (pauseSelectIndex_ < 0 || pauseSelectIndex_ > 2) {
			assert(false);
		}
		if (YKEngine::Input::GetInstance()->TriggerKey(DIK_UP)) {
			pauseSelectIndex_--;

			if (pauseSelectIndex_ < 0) {
				pauseSelectIndex_ = 2;
			}
		}

		if (YKEngine::Input::GetInstance()->TriggerKey(DIK_DOWN)) {
			pauseSelectIndex_++;

			if (pauseSelectIndex_ > 2) {
				pauseSelectIndex_ = 0;
			}
		}
		
	}
	pauseSelectAnimTimer_ += 1.0f / 60.0f;

	float offsetX = sinf(pauseSelectAnimTimer_ * 4.0f) * 8.0f;
	float selectScale = 1.0f +sinf(pauseSelectAnimTimer_ * 5.0f) * 0.08f;

	const float speed = 0.1f;

	if (isShowPause_) {
		pauseScale_ += speed;

		/*if (YKEngine::Input::GetInstance()->TriggerKey(DIK_R)) {
			pauseMenu_ = PauseMenu::Retry;
		}
		if (YKEngine::Input::GetInstance()->TriggerKey(DIK_T)) {
			pauseMenu_ = PauseMenu::ToTitle;
		}*/

	} else {
		pauseScale_ -= speed;
	}

	pauseScale_ = std::clamp(pauseScale_, 0.0f, 1.0f);

	float eased = pauseScale_ * pauseScale_;

	float baseW = 400.0f;
	float baseH = 640.0f;

	pauseSprite_->SetSize({ baseW * eased, baseH * eased });
	pausingSprite_->SetSize({ 256.0f * eased, 65.0f * eased });

	YKEngine::Vector2 resumeBaseSize = { 288.0f,49.0f };
	YKEngine::Vector2 retryBaseSize = { 192.0f,49.0f };
	YKEngine::Vector2 titleBaseSize = { 336.0f,49.0f };

	// 全員通常状態
	resumeSprite_->SetSize({resumeBaseSize.x * eased,resumeBaseSize.y * eased});

	retryUISprite_->SetSize({retryBaseSize.x * eased,retryBaseSize.y * eased});

	toTitleSprite_->SetSize({titleBaseSize.x * eased,titleBaseSize.y * eased});

	switch (pauseSelectIndex_) {

	case 0:
		resumeSprite_->SetSize({
			resumeBaseSize.x * eased * selectScale,
			resumeBaseSize.y * eased * selectScale
			});
		break;

	case 1:
		retryUISprite_->SetSize({
			retryBaseSize.x * eased * selectScale,
			retryBaseSize.y * eased * selectScale
			});
		break;

	case 2:
		toTitleSprite_->SetSize({
			titleBaseSize.x * eased * selectScale,
			titleBaseSize.y * eased * selectScale
			});
		break;
	}

	pausingSprite_->SetPosition(Lerp(center_, pausingPos_, eased));

	resumeSprite_->SetPosition(Lerp(center_, resumePos_, eased));

	retryUISprite_->SetPosition(Lerp(center_, retryPos_, eased));

	toTitleSprite_->SetPosition(Lerp(center_, titlePos_, eased));
}

//ジャッジエフェクト入力処理
void Ui::HandleJudgeInput() {
	if (YKEngine::Input::GetInstance()->TriggerKey(DIK_5)) {
		StartJudgeEffect(JudgeType::Good);
	}
	if (YKEngine::Input::GetInstance()->TriggerKey(DIK_6)) {
		StartJudgeEffect(JudgeType::Great);
	}
	if (YKEngine::Input::GetInstance()->TriggerKey(DIK_7)) {
		StartJudgeEffect(JudgeType::Perfect);
	}
}
//ジャッジエフェクト更新
void Ui::UpdateJudgeEffect() {
	if (!isJudgePlaying_) return;

	auto sprite = GetCurrentJudgeSprite();
	if (!sprite) return;

	judgeTimer_ += 0.016f;

	if (judgeTimer_ < 0.08f) {
		judgeScale_ += 0.18f;
	}

	else if (judgeTimer_ < 0.18f) {
		judgeScale_ -= 0.06f;
	}

	else {
		judgePos_.y -= 3.0f;
		judgeAlpha_ -= 0.035f;
	}

	if (judgeAlpha_ <= 0.0f) {
		isJudgePlaying_ = false;
		return;
	}

	sprite->SetPosition(judgePos_);
	sprite->SetSize({
		500.0f * judgeScale_,
		200.0f * judgeScale_
		});
	sprite->SetColor({ 1,1,1,judgeAlpha_ });
}
//HSVからRGBに変換
YKEngine::Vector4 Ui::HSVToRGB(float h, float s, float v) {
	float c = v * s;
	float x = c * (1 - fabsf(fmodf(h / 60.0f, 2) - 1));
	float m = v - c;

	float r = 0;
	float g = 0;
	float b = 0;

	if (h < 60) {
		r = c; g = x; b = 0;
	}
	else if (h < 120) {
		r = x; g = c; b = 0;
	}
	else if (h < 180) {
		r = 0; g = c; b = x;
	} else if (h < 240) {
		r = 0; g = x; b = c;
	} else if (h < 300) {
		r = x; g = 0; b = c;
	} else {
		r = c; g = 0; b = x;
	}

	return {
		r + m,
		g + m,
		b + m,
		1.0f
	};
}
//画面枠発光更新
void Ui::UpdateFrameGlow() {

	if (!isFrameGlow_) return;

	frameGlowTimer_ += 1.0f;

	float hue = 0.0f;

	switch (judgeType_) {

	case JudgeType::Good:

		hue =
			90.0f +
			sinf(frameGlowTimer_ * 0.03f) * 40.0f;

		break;

	case JudgeType::Great:

		hue =
			180.0f +
			sinf(frameGlowTimer_ * 0.03f) * 50.0f;

		break;

	case JudgeType::Perfect:
	
		hue =
			fmodf(frameGlowTimer_ * 2.0f, 360.0f);

		break;

	default:
		return;
	}

	YKEngine::Vector4 color =
		HSVToRGB(hue, 1.0f, 1.0f);

	frameSprite_->SetColor(color);
}
//ライフ減少アニメーション更新
void Ui::UpdateLifeBlink() {

	if (!isLifeBlink_) return;

	lifeBlinkTimer_ += 0.016f;

	auto& sprite = lifeSprites_[blinkLifeIndex_];

	float wave =
		(sinf(lifeBlinkTimer_ * 18.0f) + 1.0f) * 0.5f;

	//補間を滑らか
	wave = wave * wave;

	//Alpha補間
	float alpha =
		0.1f + wave * 0.9f;

	//Alphaだけ変更
	sprite->SetColor({
		1,1,1,alpha
		});

	//終了
	if (lifeBlinkTimer_ >= 0.5f) {

		sprite->SetColor({ 1,1,1,1 });

		life_--;

		isLifeBlink_ = false;
	}
}
//タイトルスペース更新
void Ui::UpdateTitleSpace() {

	titleSpaceBlinkTimer_ += 0.016f; 

	//サイン波でアルファ値変化
	float wave = (sinf(titleSpaceBlinkTimer_ * 3.0f) + 1.0f) * 0.5f; 
	float alpha = 0.1f + wave * 0.9f;

	if (titlePushSprite_) {
		auto color = titlePushSprite_->GetColor();
		color.w = alpha;
		titlePushSprite_->SetColor(color);
	}
}
//難易度選択の設定
void Ui::SetSelectedDifficulty(int index) {
	selectedDifficulty_ = index;
}
//現在のジャッジエフェクトスプライト取得
YKEngine::Sprite* Ui::GetCurrentJudgeSprite() {
	switch (judgeType_) {
	case JudgeType::Good:
		return goodSprite_.get();

	case JudgeType::Great:
		return greatSprite_.get();

	case JudgeType::Perfect:
		return perfectSprite_.get();

	default:
		return nullptr;
	}
}
//ゲームオーバー選択の設定
void Ui::SetGameOverSelect(int index) {
	gameOverSelect_ = index;
}
//ゲームクリア選択の設定
void Ui::SetGameClearSelect(int index) {
	gameClearSelect_ = index;
}