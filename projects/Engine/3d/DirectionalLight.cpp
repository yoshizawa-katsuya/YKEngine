#include "DirectionalLight.h"
#include "Vector.h"

void DirectionalLight::Initialize()
{

	//dxCommon_ = dxCommon;

	//平行光源用のResourceを作成
	//resource_ = dxCommon_->CreateBufferResource(sizeof(DirectionalLightData));
	//データを書き込む
	//書き込むためのアドレスを取得
	//resource_->Map(0, nullptr, reinterpret_cast<void**>(&data_));
	//デフォルト値
	data_ = new DirectionalLightData();
	data_->color = { 1.0f, 1.0f, 1.0f, 1.0f };
	data_->direction = { -0.3f, -1.0f, -0.3f };
	data_->intensity = 1.0f;

}
/*
void DirectionalLight::Draw()
{

	data_->direction = Normalize(data_->direction);

	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(3, resource_->GetGPUVirtualAddress());


}
*/