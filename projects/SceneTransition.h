#pragma once

#include <string>

#include "Vector2.h"
#include <memory>
#include "SpritePlatform.h"

#include "Sprite.h"
#include "SceneManager.h"

class SceneTransition
{
public:
    void Initialize(YKEngine::SpritePlatform* spritePlatform);

    // 画面遷移開始
    void Start(const std::string& targetScene,const std::string& texturePath);

    // 
	void Outro(const std::string& texturePath);

    // 更新
    void Update();

    // 描画
    void Draw();

    // 遷移中か
    bool IsTransitioning() const {return isTransitioning_;}

    // 遷移が終了したか
    bool IsFinished() const {return isFinished_;}

    bool ConsumeFinished();

    // 遷移先シーン
    const std::string& GetTargetScene() const { return targetScene_; }

private:
    // イージング
    float EaseOutCubic(float t);

private:
    enum class TransitionType
    {
        None,
        Intro,
        Outro
    };
    YKEngine::SpritePlatform* spritePlatform_ = nullptr;
    YKEngine::SceneManager* sceneManager_ = nullptr;

    std::unique_ptr<YKEngine::Sprite> transitionSprite_ = nullptr;

    // 遷移先
    std::string targetScene_;

    // 遷移用スプライト位置
    YKEngine::Vector2 position_{};

    // タイマー
    float timer_ = 0.0f;

    // 遷移時間
    float duration_ = 1.2f;

    // 状態
    bool isTransitioning_ = false;
    bool isFinished_ = false;

    TransitionType transitionType_ = TransitionType::None;
};