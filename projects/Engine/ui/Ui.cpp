#include "Ui.h"
#include "TextureManager.h"
#include "Input.h"
#include <algorithm>

// 初期化
void Ui::Initialize() {

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
		lifeSprites_[i]->SetPosition({ 60.0f + i * 50.0f, 120.0f });
		lifeSprites_[i]->SetSize({ 40.0f, 40.0f });
    }

	//ポーズUIスプライト生成
    pauseUiSprite_ = std::make_unique<YKEngine::Sprite>();
    pauseUiSprite_->Initialize(YKEngine::TextureManager::GetInstance()->Load("Resources/ui/pause.png"));
	pauseUiSprite_->SetPosition({ 1130.0f, 55.0f });
	pauseUiSprite_->SetSize({ 400.0f, 70.0f });
    pauseUiSprite_->SetAnchorPoint({ 0.5f, 0.5f });
    //ポーズ画面スプライト生成
    pauseSprite_ = std::make_unique<YKEngine::Sprite>();
	pauseSprite_->Initialize(YKEngine::TextureManager::GetInstance()->Load("Resources/ui/white.png"));
    pauseSprite_->SetPosition({ 640.0f, 360.0f });
    pauseSprite_->SetSize({ 0.0f, 0.0f });
	pauseSprite_->SetAnchorPoint({ 0.5f, 0.5f });
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

    //ポーズUI描画
    pauseUiSprite_->Draw();

    //ポーズ画面描画
    if (pauseScale_ > 0.01f) {
        pauseSprite_->Draw();
    }
}
//デバック
void Ui::Debug() {
    //ポーズUI
    if (pauseUiSprite_) {
        ImGui::Begin("Pause UI Sprite");
        ImVec2 pos = { pauseUiSprite_->GetPosition().x, pauseUiSprite_->GetPosition().y };
        if (ImGui::DragFloat2("Position", (float*)&pos, 1.0f)) {
            pauseUiSprite_->SetPosition({ pos.x, pos.y });
        }
        ImVec2 scale = { pauseUiSprite_->GetSize().x , pauseUiSprite_->GetSize().y};
        if (ImGui::DragFloat2("Scale", (float*)&scale, 1.0f, 0.0f,10000.0)) {
            pauseUiSprite_->SetSize({ scale.x, scale.y });
        }
        float rotation = pauseUiSprite_->GetRotation();
        if (ImGui::DragFloat("Rotation", &rotation, 1.0f, -360.0f, 360.0f)) {
            pauseUiSprite_->SetRotation(rotation);
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
        ImGui::End();
    }
}

Ui::PauseMenu Ui::GetPauseMenu() const {
    return pauseMenu_;
}

//入力処理
void Ui::HandleInput() {
    
    if (YKEngine::Input::GetInstance()->TriggerKey(DIK_S)) {
        AddScore(100);
    }
    //ポーズ画面表示仮
    if (YKEngine::Input::GetInstance()->TriggerKey(DIK_Q)) {
        isShowPause_ = !isShowPause_;
    }
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

//アニメーション更新
void Ui::UpdateAnimation() {

    if (isAnimating_) {
        if (displayScore_ < targetScore_) {
            displayScore_ += 10;

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
//ライフ減少
void Ui::DecreaseLife() {
    if (life_ > 0) {
        life_--;
    }
}
//ポーズメニュー更新
void Ui::UpdatePauseMenu() {

    const float speed = 0.1f;

    if (isShowPause_) {
        pauseScale_ += speed;

        if (YKEngine::Input::GetInstance()->TriggerKey(DIK_R)) {
			pauseMenu_ = PauseMenu::Retry;
        }
        if (YKEngine::Input::GetInstance()->TriggerKey(DIK_T)) {
			pauseMenu_ = PauseMenu::ToTitle;
        }

    } else {
        pauseScale_ -= speed;
    }

    pauseScale_ = std::clamp(pauseScale_, 0.0f, 1.0f);

    float eased = pauseScale_ * pauseScale_;

    float baseW = 640.0f; 
    float baseH = 360.0f;

    pauseSprite_->SetSize({ baseW * eased, baseH * eased });
}