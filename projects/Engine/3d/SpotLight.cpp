#include "SpotLight.h"
#include <numbers>

void SpotLight::Initialize()
{

	data_ = new SpotLightData();
	data_->color = { 1.0f, 1.0f, 1.0f, 1.0f };
	data_->position = { 2.0f, 1.25f, 0.0f };
	data_->distance = 7.0f;
	data_->direction = Normalize(Vector3{ -1.0f, -1.0, 0.0f });
	data_->intensity = 1.0f;
	data_->decay = 2.0f;
	data_->cosAngle = std::cos(std::numbers::pi_v<float> / 3.0f);
	data_->cosFalloffStart = 1.0f;
}
/*
void SpotLight::Draw()
{

	data_->direction = Normalize(data_->direction);

	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(6, resource_->GetGPUVirtualAddress());
}
*/