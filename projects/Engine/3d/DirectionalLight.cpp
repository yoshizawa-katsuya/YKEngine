#include "DirectionalLight.h"
#include "Vector.h"

void DirectionalLight::Initialize(DirectXCommon* dxCommon)
{

	dxCommon_ = dxCommon;

	//平行光源用のResourceを作成
	directionalLightResource_ = dxCommon_->CreateBufferResource(sizeof(DirectionalLightData));
	//データを書き込む
	//書き込むためのアドレスを取得
	directionalLightResource_->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData_));
	//デフォルト値
	directionalLightData_->color = { 1.0f, 1.0f, 1.0f, 1.0f };
	directionalLightData_->direction = { 0.0f, 0.0f, 1.0f };
	directionalLightData_->intensity = 1.0f;

}

void DirectionalLight::Draw()
{

	directionalLightData_->direction = Normalize(directionalLightData_->direction);

	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(3, directionalLightResource_->GetGPUVirtualAddress());


}
