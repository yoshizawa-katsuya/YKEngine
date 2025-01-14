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

    // 初期化
    void Initialize(BaseModel* model_);
    // 更新
    void Update();
    // 描画
    void Draw(Camera* camera);

    AABB GetAABB()const;

    //当たり判定
    bool CheckCollision(const Stone& stone1, const Stone& stone2);
private:
    enum class State {
        Waiting,     
        PowerSetting,
        Flying     
    };

    State state_ = State::Waiting; 
    WorldTransform worldTransform_; 
    std::unique_ptr<Base3dObject> object_; 
    Input* input_ = nullptr;

    float angle_ = 0.0f;            //発射角度
    float power_ = 0.0f;            //発射強さ
    float powerChangeRate_ = 0.05f; //発射強さの変動速度
    bool powerIncreasing_ = true;   //発射強さが増加中か減少中か
    float speed_ = 0.0f;            //発射時の速度
};
