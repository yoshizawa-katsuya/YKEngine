#include "Ui.h"
#include "TextureManager.h"
#include "Input.h"

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
    // ハートテクスチャ（仮パス）
   // lifeFullTex_ = YKEngine::TextureManager::GetInstance()->Load("Resources/ui/heart_full.png");
   // lifeEmptyTex_ = YKEngine::TextureManager::GetInstance()->Load("Resources/ui/heart_empty.png");

    // スプライト生成
    for (int i = 0; i < kMaxLife; i++) {
        lifeSprites_[i] = std::make_unique<YKEngine::Sprite>();
   //     lifeSprites_[i]->Initialize(lifeFullTex_);

      //  lifeSprites_[i]->SetPosition({ 50.0f + i * 40.0f, 100.0f });
    }
	//ポーズスプライト生成
    pauseSprite_ = std::make_unique<YKEngine::Sprite>();
    pauseSprite_->Initialize(YKEngine::TextureManager::GetInstance()->Load("Resources/ui/pause.png"));
	pauseSprite_->SetPosition({ 400.0f, 300.0f });
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
	//ライフ減少
    UpdateLifeUI();
	//ポーズメニュー更新
    UpdatePauseMenu();
}

//入力処理
void Ui::HandleInput() {
    
    if (YKEngine::Input::GetInstance()->TriggerKey(DIK_R)) {
        AddScore(100);
    }
    //ポーズ画面表示仮
    if (YKEngine::Input::GetInstance()->TriggerKey(DIK_P)) {
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

//描画
void Ui::Draw() {
	//スコア描画
    for (int i = 0; i < kMaxDigits; i++) {
        scoreSprites_[i]->Draw();
    }
    /*
    //ライフ描画
    for (int i = 0; i < kMaxLife; i++) {
        lifeSprites_[i]->Draw();
    }
    */
}
//ライフ減少(仮実装)
void Ui::HandleLifeInput() {
    if (YKEngine::Input::GetInstance()->TriggerKey(DIK_Q)) {
        DecreaseLife();
    }
}
//ライフ減少
void Ui::DecreaseLife() {
    if (life_ > 0) {
        life_--;
    }
}
//ライフUI更新
void Ui::UpdateLifeUI() {

    for (int i = 0; i < kMaxLife; i++) {

        if (i < life_) {
            lifeSprites_[i]->SetTexture(lifeFullTex_);
        } else {
            lifeSprites_[i]->SetTexture(lifeEmptyTex_);
        }
    }
}
//ポーズメニュー更新
void Ui::UpdatePauseMenu() {

}