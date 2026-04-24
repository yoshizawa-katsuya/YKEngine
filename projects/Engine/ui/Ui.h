#pragma once
#include "Sprite.h"
#include <array>
#include <memory>

class Ui
{
public:
    //初期化
    void Initialize();
    //更新
    void Update();
    //描画
    void Draw();

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
	//ライフUI更新
    void UpdateLifeUI();
    //ポーズメニュー更新
	void UpdatePauseMenu();

private:
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
    //ポーズスプライト
	std::unique_ptr<YKEngine::Sprite> pauseSprite_;
    //スコアテクスチャ
    std::array<uint32_t, 10> numberTextures_;
    //桁
    std::array<int, kMaxDigits> digits_{};
    //有ライフ
    uint32_t lifeFullTex_ = 0;
    //空ライフ
	uint32_t lifeEmptyTex_ = 0;
	//スコア
    int score_ = 0;
	//表示スコア
    int displayScore_ = 0;
	//目標スコア
    int targetScore_ = 0;
    //現在ライフ
    int life_ = kMaxLife;
    //ポーズ画面のスケール
    float pauseScale_ = 0.0f;
    //ホバー用タイマー
    float hoverTimer_ = 0.0f;
	//アニメーション中か
    bool isAnimating_ = false;
    //ポーズ表示フラグ
    bool isShowPause_ = false;
};