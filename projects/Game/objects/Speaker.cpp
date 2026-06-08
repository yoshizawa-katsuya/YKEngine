#include "Speaker.h"

using namespace YKEngine;

void Speaker::Initialize(YKEngine::BaseModel* model) {
	object_ = std::make_unique<My3dObject>();
	object_->Initialize(model);

	worldTransform_.Initialize();

	baseScale_ = worldTransform_.scale_;
}

void Speaker::Update() {
	beatTimer_ += 1.0f / 60.0f;

	float beat = std::sin(beatTimer_ * beatSpeed_);

	float scale =
		(beat >= 0.0f)
		? 1.0f + beat * 0.15f   // 拡大は15%
		: 1.0f + beat * 0.03f;  // 縮小は3%

	worldTransform_.scale_ = {
		baseScale_.x * scale,
		baseScale_.y * scale,
		baseScale_.z * scale
	};


	worldTransform_.UpdateMatrix();
	object_->WorldTransformUpdate(worldTransform_);
}

void Speaker::Draw(Camera* camera) {

	object_->CameraUpdate(camera);
	object_->Draw();

}
