#pragma once
#include "Sprite.h"
#include "imgui.h"
#include "Vector2.h"
#include "Vector4.h"
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
    //タイトル描画
	void DrawTitle();

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
	//画面枠発光更新
    void UpdateFrameGlow();
	//ライフ点滅更新
    void UpdateLifeBlink();
    //タイトルスペース更新
    void UpdateTitleSpace();
	//HSVからRGBに変換
    YKEngine::Vector4 HSVToRGB(float h, float s, float v);
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
    //タイトルスプライト
    std::unique_ptr<YKEngine::Sprite> titleSprite_;
    //タイトルスペース
    std::unique_ptr<YKEngine::Sprite>titlePushSprite_;
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
    //画面枠スプライト
    std::unique_ptr<YKEngine::Sprite>frameSprite_;
    //難易度スプライト
    std::unique_ptr<YKEngine::Sprite>easySprite_;
    std::unique_ptr<YKEngine::Sprite>normalSprite_;
    std::unique_ptr<YKEngine::Sprite>hardSprite_;
    //スコアテクスチャ
    std::array<uint32_t, 10> numberTextures_;
    //ジャッジエフェクトテクスチャ
    uint32_t goodTexture_;
    uint32_t greatTexture_;
    uint32_t perfectTexture_;
    //難易度テクスチャ
	uint32_t easyTexture_;
	uint32_t normalTexture_;
	uint32_t hardTexture_;
    //桁
    std::array<int, kMaxDigits> digits_{};
	//ジャッジエフェクト表示位置
    YKEngine::Vector2 judgePos_ = { 640.0f, 250.0f };
	//ライフ点滅表示位置
    YKEngine::Vector2 lifeBlinkPos_ = { 0.0f,0.0f };
	//スコア
    int score_ = 0;
	//表示スコア
    int displayScore_ = 0;
	//目標スコア
    int targetScore_ = 0;
    //現在ライフ
    int life_ = kMaxLife;
	//ライフ点滅用インデックス
    int blinkLifeIndex_ = -1;
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
	//画面枠発光用タイマー
    float frameGlowTimer_ = 0.0f;
	//ライフ点滅用タイマー
    float lifeBlinkTimer_ = 0.0f;
	//ライフ点滅スケール
    float lifeBlinkScale_ = 1.0f;
	//タイトルスペース点滅用タイマー
    float titleSpaceBlinkTimer_ = 0.0f;
	//画面枠発光フラグ
    bool isFrameGlow_ = false;
	//アニメーション中か
    bool isAnimating_ = false;
    //ポーズ表示フラグ
    bool isShowPause_ = false;
	//ジャッジエフェクト再生中フラグ
    bool isJudgePlaying_ = false;
	//ライフ点滅フラグ
    bool isLifeBlink_ = false;

};