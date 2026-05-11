#pragma once
#include "Sprite.h"
#include "imgui.h"
#include "Vector2.h"
#include <array>
#include <memory>

class Ui
{
public:
	//ポーズメニューの種類
    enum class PauseMenu {
        None,
        Retry,
        ToTitle
    };
	//ジャッジエフェクトの種類
    enum class JudgeType {
        None,
        Good,
        Great,
        Perfect
    };

    //初期化
    void Initialize();
    //更新
    void Update();
    //描画
    void Draw();
    //デバック
    void Debug();
    
	//ポーズメニューの取得
    PauseMenu GetPauseMenu()const;
private:
	//スコア加算処理(仮実装)
    void HandleInput();
	//スコア加算
    void AddScore(int value);
	//アニメーション更新
    void UpdateAnimation();
	//桁の更新
    void UpdateDigits();
    //ライフ減少(仮実装)
    void HandleLifeInput();
	//ライフ減少
    void DecreaseLife();
    //ポーズメニュー更新
	void UpdatePauseMenu();
	//ジャッジエフェクト入力処理
    void HandleJudgeInput();
	//ジャッジエフェクト開始
    void StartJudgeEffect(JudgeType type);
	//ジャッジエフェクト更新
    void UpdateJudgeEffect();
	//現在のジャッジエフェクトスプライト取得
    YKEngine::Sprite* GetCurrentJudgeSprite();

private:
	//ポーズメニューの種類
    PauseMenu pauseMenu_ = PauseMenu::None;
	//ジャッジエフェクトの種類
    JudgeType judgeType_ = JudgeType::None;
	//最大桁数
    static const int kMaxDigits = 6;
    //最大スコア
    static const int kMaxScore = 999999;
    //最大ライフ
    static const int kMaxLife = 3;
    //スコアスプライト
    std::array<std::unique_ptr<YKEngine::Sprite>, kMaxDigits> scoreSprites_;
	//ライフスプライト
	std::array<std::unique_ptr<YKEngine::Sprite>, kMaxLife> lifeSprites_;
    //ポーズUIスプライト
	std::unique_ptr<YKEngine::Sprite> pauseUiSprite_;
    //ポーズ画面スプライト
	std::unique_ptr<YKEngine::Sprite> pauseSprite_;
	//ジャッジエフェクトスプライト
    std::unique_ptr<YKEngine::Sprite> goodSprite_;
    std::unique_ptr<YKEngine::Sprite> greatSprite_;
    std::unique_ptr<YKEngine::Sprite> perfectSprite_;
    //スコアテクスチャ
    std::array<uint32_t, 10> numberTextures_;
    //桁
    std::array<int, kMaxDigits> digits_{};
	//ジャッジエフェクト表示位置
    YKEngine::Vector2 judgePos_ = { 640.0f, 250.0f };
	//スコア
    int score_ = 0;
	//表示スコア
    int displayScore_ = 0;
	//目標スコア
    int targetScore_ = 0;
    //現在ライフ
    int life_ = kMaxLife;
	//ジャッジエフェクトテクスチャ
    uint32_t goodTexture_;
	uint32_t greatTexture_;
	uint32_t perfectTexture_;
    //ポーズ画面のスケール
    float pauseScale_ = 0.0f;
    //ホバー用タイマー
    float hoverTimer_ = 0.0f;
	//ジャッジエフェクト用タイマー
    float judgeTimer_ = 0.0f;
	//ジャッジエフェクトスケール
    float judgeScale_ = 0.0f;
	//ジャッジエフェクトアルファ
    float judgeAlpha_ = 0.0f;
	//アニメーション中か
    bool isAnimating_ = false;
    //ポーズ表示フラグ
    bool isShowPause_ = false;
	//ジャッジエフェクト再生中フラグ
    bool isJudgePlaying_ = false;
};