#include "PointLight.h"

void PointLight::Initialize(DirectXCommon* dxCommon)
{

	dxCommon_ = dxCommon;

	//点光源用のResourceを作成
	resource_ = dxCommon_->CreateBufferResource(sizeof(PointLightData));
	//データを書き込む
	//書き込むためのアドレスを取得
	resource_->Map(0, nullptr, reinterpret_cast<void**>(&data_));
	//デフォルト値
	data_->color = { 1.0f, 1.0f, 1.0f, 1.0f };
	data_->position = { 0.0f, 2.0f, 0.0f };
	data_->intensity = 0.0f;
	data_->radius = 3.0f;
	data_->decay = 1.0f;

}

void PointLight::Draw()
{

	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(5, resource_->GetGPUVirtualAddress());

}
