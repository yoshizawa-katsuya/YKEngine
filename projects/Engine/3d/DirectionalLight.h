#pragma once
#include "DirectXCommon.h"
#include <Struct.h>

class DirectionalLight
{
public:

	//初期化
	void Initialize(DirectXCommon* dxCommon);

	//描画
	void Draw();

	Vector4& GetColor() { return data_->color; }
	const Vector4& GetColor() const { return data_->color; }

	Vector3& GetDirection() { return data_->direction; }
	const Vector3& GetDirection() const { return data_->direction; }

	float& GetIntensity() { return data_->intensity; }
	float GetIntensity() const { return data_->intensity; }

	void SetColor(const Vector4& color) { data_->color = color; }

	void SetDirection(const Vector3& direction) { data_->direction = direction; }

	void SetIntensity(float intensity) { data_->intensity = intensity; }

private:

	struct DirectionalLightData {
		Vector4 color;	//ライトの色
		Vector3 direction;	//ライトの向き
		float intensity;	//輝度
	};

	DirectXCommon* dxCommon_;

	//平行光源用のResourceを作成
	Microsoft::WRL::ComPtr<ID3D12Resource> resource_;
	//データを書き込む
	DirectionalLightData* data_;

};

