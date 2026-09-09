#include "SceneTransition.h"

#include <algorithm>
#include <cmath>


using namespace YKEngine;

void SceneTransition::Initialize(YKEngine::SpritePlatform* spritePlatform)
{
    spritePlatform_ = spritePlatform;
}

void SceneTransition::Intro(const std::string& targetScene,const std::string& texturePath)
{
    targetScene_ = targetScene;

    timer_ = 0.0f;

    isTransitioning_ = true;
    isFinished_ = false;


    transitionType_ = TransitionType::Intro;

    // テクスチャからスプライト生成
	transitionSprite_ = std::make_unique<YKEngine::Sprite>();
	transitionSprite_->Initialize(TextureManager::GetInstance()->Load(texturePath));

    // 画面左外から開始
    position_ = { -1280.0f, 0.0f };

    transitionSprite_->SetPosition(position_);
    transitionSprite_->SetSize({ 1280.0f, 720.0f });
}

void SceneTransition::Outro(const std::string& texturePath)
{
    timer_ = 0.0f;

    isTransitioning_ = true;
    isFinished_ = false;


    transitionType_ = TransitionType::Outro;


    // テクスチャからスプライト生成
    transitionSprite_ = std::make_unique<YKEngine::Sprite>();
    transitionSprite_->Initialize(
        TextureManager::GetInstance()->Load(texturePath)
    );

    // 画面全体を覆った状態から開始
    position_ = { 0.0f, 0.0f };

    transitionSprite_->SetPosition(position_);
    transitionSprite_->SetSize({ 1280.0f, 720.0f });
}

void SceneTransition::Update()
{
    if (!isTransitioning_)
    {
        return;
    }

    constexpr float deltaTime = 1.0f / 60.0f;

    timer_ += deltaTime;

    float t = timer_ / duration_;
    t = std::clamp(t, 0.0f, 1.0f);

    float easedT = EaseOutCubic(t);

    switch (transitionType_)
    {
    case TransitionType::Intro:
    {
        // 左 → 中央
        position_.x =
            std::lerp(
                -1280.0f,
                0.0f,
                easedT
            );

        break;
    }

    case TransitionType::Outro:
    {
        // 中央 → 右
        position_.x =
            std::lerp(
                0.0f,
                1280.0f,
                easedT
            );

        break;
    }
    }

    transitionSprite_->SetPosition(position_);

    if (t >= 1.0f)
    {
        isTransitioning_ = false;
        isFinished_ = true;
        transitionType_ = TransitionType::None;
    }
}

void SceneTransition::Draw()
{
    if (!transitionSprite_)
    {
        return;
    }

    transitionSprite_->Draw();
}

bool SceneTransition::ConsumeFinished()
{
    if (!isFinished_)
    {
        return false;
    }

    isFinished_ = false;

    return true;
}

float SceneTransition::EaseOutCubic(float t)
{
    t = std::clamp(t, 0.0f, 1.0f);

    return 1.0f - std::pow(1.0f - t, 3.0f);
}

#if !defined(__cpp_lib_make_unique)
namespace std {
    template<class T, class... Args>
    std::unique_ptr<T> make_unique(Args&&... args) {
        return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
    }
}
#endif