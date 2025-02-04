#pragma once
#include <cstdlib>
#include <memory>
#include <map>
#include <vector>
#include "BaseScene.h"
#include "Audio.h"
#include "Input.h"
#include "ModelPlatform.h"
#include "SpritePlatform.h"
#include "BaseModel.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Sprite.h"
#include "Player.h"
#include "Camera.h"
#include "DebugCamera.h"
#include "ParticleEmitter.h"
#include "Stone.h"
#include "Star.h"

class GameScene : public BaseScene
{
public:
    ~GameScene() override;

    void Initialize() override;

    void Update() override;

    void Draw() override;

    void Finalize() override;

    bool isItemBetween(const Stone& stone1, const Stone& stone2, const Star& item);
private:
    // デバイス
    DirectXCommon* dxCommon_;

    Audio* audio_;
    Input* input_;

    SpritePlatform* spritePlatform_;
    ModelPlatform* modelPlatform_;

    Camera* mainCamera_ = nullptr;

    std::unique_ptr<Camera> camera_;
    std::unique_ptr<Camera> camera2_;

    std::unique_ptr<DebugCamera> debugCamera_;

    bool isActiveDebugCamera_ = false;

    // 平行光源
    std::unique_ptr<DirectionalLight> directionalLight_;

    // 点光源
    std::unique_ptr<PointLight> pointLight_;

    // スポットライト
    std::unique_ptr<SpotLight> spotLight_;

    std::unique_ptr<BaseModel> modelPlayer_;
    // ストーンモデル
    std::unique_ptr<BaseModel> modelStone_;
    //スターモデル
    std::unique_ptr<BaseModel>modelStar_;

    uint32_t textureHandle_;

    // プレイヤー
    std::unique_ptr<Player> player_;

    // ストーンのリスト
    std::vector<std::unique_ptr<Stone>> stones_;
    //スター
   std::vector< std::unique_ptr<Star>>star_;
};
