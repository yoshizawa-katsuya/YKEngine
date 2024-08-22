#pragma once
#include <wrl.h>
#include "DirectXCommon.h"
#include <Struct.h>

class DirectionalLight
{
public:

	//初期化
	void Initialize(DirectXCommon* dxCommon);

	//描画
	void Draw();

	Vector4& GetColor() { return directionalLightData_->color; }
	const Vector4& GetColor() const { return directionalLightData_->color; }

	Vector3& GetDirection() { return directionalLightData_->direction; }
	const Vector3& GetDirection() const { return directionalLightData_->direction; }

	float& GetIntensity() { return directionalLightData_->intensity; }
	float GetIntensity() const { return directionalLightData_->intensity; }

private:

	struct DirectionalLightData {
		Vector4 color;	//ライトの色
		Vector3 direction;	//ライトの向き
		float intensity;	//輝度
	};

	DirectXCommon* dxCommon_;

	//平行光源用のResourceを作成
	Microsoft::WRL::ComPtr<ID3D12Resource> directionalLightResource_;
	//データを書き込む
	DirectionalLightData* directionalLightData_;

};

