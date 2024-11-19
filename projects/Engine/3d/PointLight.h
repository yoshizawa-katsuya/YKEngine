#pragma once
#include "DirectXCommon.h"
#include <Struct.h>

class PointLight
{
public:

	//初期化
	void Initialize(DirectXCommon* dxCommon);

	//描画
	void Draw();

	Vector4& GetColor() { return data_->color; }
	const Vector4& GetColor() const { return data_->color; }

	Vector3& GetPosition() { return data_->position; }
	const Vector3& GetPosition() const { return data_->position; }

	float& GetIntensity() { return data_->intensity; }
	float GetIntensity() const { return data_->intensity; }

	float& GetRadius() { return data_->radius; }
	float GetRadius() const { return data_->radius; }

	float& GetDecay() { return data_->decay; }
	float GetDecay() const { return data_->decay; }

	void SetColor(const Vector4& color) { data_->color = color; }

	void SetPosition(const Vector3& position) { data_->position = position; }

	void SetIntensity(float intensity) { data_->intensity = intensity; }

	void SetRadius(float radius) { data_->radius = radius; }

	void SetDecay(float decay) { data_->decay = decay; }

private:

	struct PointLightData {
		Vector4 color;	//ライトの色
		Vector3 position;	//ライトの位置
		float intensity;	//輝度
		float radius;	//ライトの届く最大距離
		float decay;	//減衰率
		float padding[2];
	};

	DirectXCommon* dxCommon_;

	//平行光源用のResourceを作成
	Microsoft::WRL::ComPtr<ID3D12Resource> resource_;
	//データを書き込む
	PointLightData* data_;

};

