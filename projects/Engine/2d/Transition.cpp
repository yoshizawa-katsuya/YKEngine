#include "Transition.h"
#include <algorithm>
#include <cmath>

void Transition::Initialize()
{
}

void Transition::StartFadeIn(
    uint32_t textureHandle,
    uint32_t maskTextureHandle,
    float duration,
    EasingType easingType
)
{
	// スプライトの初期化
    sprite_.Initialize(textureHandle);

	// マスクテクスチャの設定
    sprite_.SetMaskTexture(maskTextureHandle);

	// モードをフェードインに設定
    mode_ = Mode::FadeIn;

	// 遷移の継続時間を設定
    duration_ = duration;

	// イージングの種類を設定
    easingType_ = easingType;

	// タイマーをリセット
    timer_ = 0.0f;

	// 遷移の進行度を初期化（フェードインは最初は完全に覆われている状態）
    progress_ = 1.0f;

	// 遷移が終了していない状態に設定
    isFinished_ = false;
    
	// スプライトに初期の進行度を設定
    sprite_.SetProgress(progress_);
}

void Transition::StartFadeOut(
    uint32_t textureHandle,
    uint32_t maskTextureHandle,
    float duration,
    EasingType easingType
)
{
	// スプライトの初期化
    sprite_.Initialize(textureHandle);

	// マスクテクスチャの設定
    sprite_.SetMaskTexture(maskTextureHandle);

	// モードをフェードアウトに設定
    mode_ = Mode::FadeOut;

	// イージングの種類を設定
    duration_ = duration;

	// タイマーをリセット
    timer_ = 0.0f;

	// 遷移の進行度を初期化（フェードアウトは最初は完全に見えている状態）
    progress_ = 0.0f;

	// 遷移が終了していない状態に設定
    isFinished_ = false;

	// スプライトに初期の進行度を設定
    sprite_.SetProgress(progress_);
}

void Transition::Update()
{
	// モードがNoneの場合や遷移が終了している場合は更新しない
    if (mode_ == Mode::None || isFinished_) {
        return;
    }

	// タイマーを更新（仮に60FPSで更新されることを想定）
    timer_ += 1.0f / 60.0f;

	// タイマーを遷移の継続時間で割って0.0f～1.0fの範囲に正規化
    float t = timer_ / duration_;

	// 0.0f～1.0fの範囲にクランプ
    t = std::clamp(t, 0.0f, 1.0f);

	// イージング関数を適用
    float eased = ApplyEasing(t);

	// フェードインとフェードアウトで進行度の計算が異なる
    if (mode_ == Mode::FadeIn) {

        progress_ = 1.0f - eased;
    }
    else {

        progress_ = eased;
    }
	// スプライトに進行度を設定
    sprite_.SetProgress(progress_);

	// 遷移が終了したかどうかをチェック
    if (t >= 1.0f) {

        isFinished_ = true;
    }
}

void Transition::Draw()
{
	// モードがNoneの場合は描画しない
    if (mode_ == Mode::None) {
        return;
    }
	// スプライトを描画
    sprite_.Draw();
}

bool Transition::IsFinished() const
{
    return isFinished_;
}

bool Transition::IsActive() const
{
    return mode_ != Mode::None;
}

float Transition::ApplyEasing(float t)
{
	// イージング関数を適用して、tを変換
    switch (easingType_)
    {
    case EasingType::Linear:
        return t;

    case EasingType::EaseInSine:
        return 1.0f -
            cosf((t * 3.14159265f) / 2.0f);

    case EasingType::EaseOutSine:
        return
            sinf((t * 3.14159265f) / 2.0f);

    case EasingType::EaseInOutSine:
        return
            -(cosf(3.14159265f * t) - 1.0f) / 2.0f;

    case EasingType::EaseInQuint:
        return powf(t, 5.0f);

    case EasingType::EaseOutQuint:
        return 1.0f - powf(1.0f - t, 5.0f);
    }

    return t;
}