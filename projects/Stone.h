#pragma once
#include "Base3dObject.h"
#include "WorldTransform.h"
#include "Input.h"
#include <imgui.h>
#include <cmath>
#include <algorithm>

class Stone {
public:
    struct AABB {
        Vector3 min;
        Vector3 max;
    };

    enum class State {
        Waiting,
        PowerSetting,
        Flying,
        Stopped
    };

    // 初期化
    void Initialize(BaseModel* model_);
    // 更新
    void Update();
    // 描画
    void Draw(Camera* camera);

    void HandleCollision(Stone& other);

    //速度のセッター
    void SetSpeed(float speed) { speed_ = speed; }
    //状態のセッター
    void SetState(State state) { state_ = state; }
    //座標のセッター
    void SetPosition(const Vector3& position) { worldTransform_.translation_ = position; }

    AABB GetAABB()const;
    State GetState()const { return state_; }
    float GetSpeed()const { return speed_; }
    Vector3 GetPosition()const { return worldTransform_.translation_; }
    //当たり判定
    bool CheckCollision(const Stone& stone1, const Stone& stone2);
private:

    State state_ = State::Waiting;
    WorldTransform worldTransform_;
    std::unique_ptr<Base3dObject> object_;
    Input* input_ = nullptr;

    float angle_ = 0.0f;            //発射角度
    float power_ = 0.0f;            //発射強さ
    float powerChangeRate_ = 0.005f; //発射強さの変動速度
    bool powerIncreasing_ = true;   //発射強さが増加中か減少中か
    float speed_ = 0.0f;            //発射時の速度
    float maxSpeed = 0.05f;
};