#include "PointLight.h"

void PointLight::Initialize()
{

	//データを書き込む
	//デフォルト値
	data_ = new PointLightData();
	data_->color = { 1.0f, 1.0f, 1.0f, 1.0f };
	data_->position = { 0.0f, 2.0f, 0.0f };
	data_->intensity = 1.0f;
	data_->radius = 3.0f;
	data_->decay = 1.0f;

}

/*
void PointLight::Draw()
{

	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(5, resource_->GetGPUVirtualAddress());

}
*/