#pragma once
#include "My3dObject.h"
#include "WorldTransform.h"
class Speaker
{
public:
	void Initialize(YKEngine::BaseModel* model);

	void Update();

	void Draw(YKEngine::Camera* camera);

	const YKEngine::WorldTransform& GetWorldTransform()const { return worldTransform_; }

	void SetTraslation(YKEngine::Vector3& translation) { worldTransform_.translation_ = translation; }

	void SetRotation(YKEngine::Vector3& rotation) { worldTransform_.rotation_ = rotation; }

private:
	YKEngine::WorldTransform worldTransform_;

	std::unique_ptr<YKEngine::My3dObject> object_;

	float beatTimer_ = 0.0f;

	float beatSpeed_ = 8.0f;     // BPM的な速さ
	float beatScale_ = 0.05f;    // 拡縮量

	YKEngine::Vector3 baseScale_ = { 1.0f,1.0f,1.0f };
};

