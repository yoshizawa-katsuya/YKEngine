#pragma once
#include "BaseCharacter.h"

class ElectricRange : public BaseCharacter
{
public:
    //初期化
    void Initialize(BaseModel* model, const Vector3& position) override;
    //更新
    void Update() override;
    //描画
    void Draw(Camera* camera) override;
    //Imgui
    void DebugImGui();
public:
    Vector3 GetPosition() const { return electricPosition; }
    Vector3 GetScale() const { return scale_; }
    void SetColor(const Vector4& color);
    void SetFollowTarget(const Vector3* targetPos);
private:
    void ClampScale();
private:
    Vector4 color_ = { 1.0f, 0.902f, 0.0f, 0.45f };
    Vector3 scale_ = { 0.7f, 0.7f, 0.7f };
    Vector3 electricPosition; 
    Vector3 offset = { 0, 0, 0 };
    const float minScale_ = 0.2f;
    const float maxScale_ = 1.8f;
    const float step_ = 0.2f;
    const Vector3* targetPosition_ = nullptr; 
};

