#include "Stone.h"
#include "Rigid3dObject.h"
#include "Camera.h"
#include "Vector.h"

// 初期化
void Stone::Initialize(BaseModel* model_) {
    object_ = std::make_unique<Rigid3dObject>();
    object_->Initialize(model_);
    worldTransform_.Initialize();
    worldTransform_.translation_ = { 0.0f, 0.0f, 0.0f };
    worldTransform_.scale_ = { 0.3f, 0.3f, 0.3f };
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
            if (angle_ < 0.0f) angle_ = 0.0f;
        }
        if (input_->PushKey(DIK_RIGHT)) {
            angle_ += 1.0f;
            if (angle_ > 180.0f) angle_ = 180.0f;
        }

        //角度を0度から180度の範囲に制限
        angle_ = std::clamp(angle_, 0.0f, 180.0f);

        if (input_->TriggerKey(DIK_SPACE)) {
            state_ = State::PowerSetting;
            power_ = 0.0f;
            powerIncreasing_ = true;
        }
        break;
    }
    case State::PowerSetting: {

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

            speed_ = maxSpeed * pow(power_, 2.0f);

            state_ = State::Flying;
        }
        break;
    }
    case State::Flying: {
        if (speed_ > 0.0f) {
            float radian = angle_ * (3.14159265f / 180.0f);
            //イージング
            float easingFactor = 1.0f - pow(1.0f - (1.0f - speed_ / maxSpeed), 3.0f);

            worldTransform_.translation_.x += speed_ * cos(radian) * easingFactor;
            worldTransform_.translation_.y += speed_ * sin(radian) * easingFactor;

            //速度を徐々に減少
            speed_ *= 0.98f;

            if (speed_ < 0.001f) {
                speed_ = 0.0f;
                state_ = State::Stopped;
            }
        }
        break;
    }


    case State::Stopped: {
        break;
    }
    }
    worldTransform_.UpdateMatrix();

#ifdef _DEBUG
    if (state_ != State::Stopped) {
        ImGui::Begin("Stone");
        ImGui::Text("State: %s",
            state_ == State::Waiting ? "Waiting" :
            state_ == State::PowerSetting ? "PowerSetting" : "Flying");
        ImGui::Text("Angle: %.2f", angle_);
        ImGui::Text("Power: %.2f", power_);
        ImGui::DragFloat3("Translate", &worldTransform_.translation_.x, 0.01f);
        ImGui::End();
    }
#endif
}
// 描画
void Stone::Draw(Camera* camera) {
    object_->CameraUpdate(camera);
    object_->WorldTransformUpdate(worldTransform_);
    object_->Draw();
}

void Stone::HandleCollision(Stone& other) {
    if (this->GetState() != State::Flying || other.GetState() != State::Stopped) {
        return;
    }

    Vector3 collisionDirection = other.GetPosition() - this->GetPosition();
    float distance = Length(collisionDirection);

    if (distance == 0.0f) {
        return;
    }

    collisionDirection = Normalize(collisionDirection);

    //速度のいくつかを他のストーンに渡す
    float transferSpeed = this->GetSpeed() * 0.7f;
    //衝突後に残る速度
    float remainingSpeed = this->GetSpeed() * 0.3f;

    this->SetSpeed(remainingSpeed);
    other.SetSpeed(transferSpeed);
    other.SetState(State::Flying);
    float moveDistance = transferSpeed * 1.5f;
    //衝突されたストーンを適切な方向へ移動
    other.SetPosition(other.GetPosition() + collisionDirection * transferSpeed);

    //摩擦による速度減少
    float frictionCoefficient = 0.07f;
    float newSpeed = other.GetSpeed() - (other.GetSpeed() * frictionCoefficient);
    if (newSpeed < 0.0f) newSpeed = 0.0f;
    other.SetSpeed(newSpeed);
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