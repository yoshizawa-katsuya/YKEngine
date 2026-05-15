#pragma once
#include <Sprite.h>
#include <functional>
#include <string>

/// <summary>
/// 画面遷移クラス
/// </summary>
class Transition
{
public:
	// 遷移モード
    enum class Mode
    {
        None,
        FadeIn,
        FadeOut
    };
	// イージングの種類
    enum class EasingType
    {
        Linear,
        EaseInSine,
        EaseOutSine,
        EaseInOutSine,
        EaseInQuint,
        EaseOutQuint
    };
	// 初期化
    void Initialize();

	// 更新
    void Update();

	// 描画
    void Draw();

	// フェードイン開始
    void StartFadeIn(
        uint32_t textureHandle,
        uint32_t maskTextureHandle,
        float duration,
        EasingType easingType
    );

	// フェードアウト開始
    void StartFadeOut(
        uint32_t textureHandle,
        uint32_t maskTextureHandle,
        float duration,
        EasingType easingType
    );

	// 遷移が終了したか
    bool IsFinished() const;

	// 遷移がアクティブか
    bool IsActive() const;

private:

	// イージング関数を適用
    float ApplyEasing(float t);

private:

	// 遷移に使用するスプライト
    YKEngine::Sprite sprite_;

	// 遷移のモード
    Mode mode_ = Mode::None;

	// 遷移の進行度（0.0f～1.0f）
    float progress_ = 0.0f;

	// タイマー
    float timer_ = 0.0f;

	// 遷移の継続時間
    float duration_ = 1.0f;

	// 遷移が終了したか
    bool isFinished_ = false;

	// イージングの種類
    EasingType easingType_ = EasingType::Linear;
};