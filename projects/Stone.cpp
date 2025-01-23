#include "Stone.h"
#include "Rigid3dObject.h"
#include "Camera.h"
#include "Vector.h"

// 初期化
void Stone::Initialize(BaseModel* model_) {
    object_ = std::make_unique<Rigid3dObject>();
    object_->Initialize(model_);
    worldTransform_.Initialize();
    worldTransform_.translation_ = { 0.0f, -2.0f, 0.0f }; 
    worldTransform_.scale_ = { 0.1f, 0.1f, 0.1f };        
    worldTransform_.rotation_ = { 0.0f, 3.0f, 0.0f };
    input_ = Input::GetInstance();
}

// 更新
void Stone::Update() {
    
    switch (state_) {
    case State::Waiting: {
        //角度を決める
        if (input_->PushKey(DIK_LEFT)) {
            angle_ -= 1.0f;
            if (angle_ < 0.0f) angle_ = 0.0f; // 0度以下に制限
        }
        if (input_->PushKey(DIK_RIGHT)) {
            angle_ += 1.0f;
            if (angle_ > 180.0f) angle_ = 180.0f; // 180度以上に制限
        }

        // 角度を0度から180度の範囲に制限
        angle_ = std::clamp(angle_, 0.0f, 180.0f);

        if (input_->TriggerKey(DIK_SPACE)) {
            // 強さ決定フェーズに移行
            state_ = State::PowerSetting;
            power_ = 0.0f;
            powerIncreasing_ = true;
        }
        break;
    }
    case State::PowerSetting: {
        // 発射強さを決める
        if (powerIncreasing_) {
            power_ += powerChangeRate_;
            if (power_ >= 1.0f) {
                power_ = 1.0f;
                powerIncreasing_ = false;
            }
        }
        else {
            power_ -= powerChangeRate_;
            if (power_ <= 0.0f) {
                power_ = 0.0f;
                powerIncreasing_ = true;
            }
        }

        if (input_->TriggerKey(DIK_SPACE)) {
       
            speed_ = power_ * 0.15f;
            state_ = State::Flying;
        }
        break;
    }
    case State::Flying: {
        // ストーンの移動
        if (speed_ > 0.0f) {
            float radian = angle_ * (3.14159265f / 180.0f);
            worldTransform_.translation_.x += speed_ * cos(radian);
            worldTransform_.translation_.y += speed_ * sin(radian);

            //摩擦による速度減少
            float frictionCoefficient = 0.07f; //摩擦係数
            speed_ -= speed_ * frictionCoefficient;

            if (speed_ < 0.001f) speed_ = 0.0f;
        }
        else {
            state_ = State::Stopped;
        }
        break;
    }

    case State::Stopped: {
        break;
    }
    }
    input_->Update();
    worldTransform_.UpdateMatrix();

#ifdef _DEBUG
    ImGui::Begin("Stone");
    ImGui::Text("State: %s",
        state_ == State::Waiting ? "Waiting" :
        state_ == State::PowerSetting ? "PowerSetting" : "Flying");
    ImGui::Text("Angle: %.2f", angle_);
    ImGui::Text("Power: %.2f", power_);
    ImGui::DragFloat3("Translate", &worldTransform_.translation_.x, 0.01f);
    ImGui::End();
#endif
}

// 描画
void Stone::Draw(Camera* camera) {
    object_->CameraUpdate(camera);
    object_->WorldTransformUpdate(worldTransform_);
    object_->Draw();
}

//衝突
void Stone::HandleCollision(Stone& other) {
   
    Vector3 collisionDirection = other.GetPosition() - this->GetPosition();

    float length = Length(collisionDirection);

    if (length == 0.0f) return; //同じ位置の場合は何もしない

    Normalize(collisionDirection);

    float pushBackDistance = 0.1f; //移動量
    other.SetPosition(other.GetPosition() + collisionDirection * pushBackDistance);

    //発射したストーンの速度を減少
    speed_ *= 0.5f; 
}


Stone::AABB Stone::GetAABB()const {
    AABB aabb;
    Vector3 halfScale = worldTransform_.scale_;//仮

    aabb.min = {
        worldTransform_.translation_.x - halfScale.x,
        worldTransform_.translation_.y - halfScale.y,
        worldTransform_.translation_.z - halfScale.z
    };
    aabb.max = {
        worldTransform_.translation_.x + halfScale.x,
        worldTransform_.translation_.y + halfScale.y,
        worldTransform_.translation_.z + halfScale.z
    };
    return aabb;
}

//当たり判定
bool Stone::CheckCollision(const Stone& stone1, const Stone& stone2) {
    const AABB aabb1 = stone1.GetAABB();
    const AABB aabb2 = stone2.GetAABB();

    return (aabb1.min.x <= aabb2.max.x && aabb1.max.x >= aabb2.min.x) &&
        (aabb1.min.y <= aabb2.max.y && aabb1.max.y >= aabb2.min.y) &&
        (aabb1.min.z <= aabb2.max.z && aabb1.max.z >= aabb2.min.z);
}