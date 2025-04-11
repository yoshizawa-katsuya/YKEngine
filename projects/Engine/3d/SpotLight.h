#pragma once
#include "DirectXCommon.h"
#include <Struct.h>

class SpotLight
{
public:

	struct SpotLightData {
		Vector4 color;	//ライトの色
		Vector3 position;	//ライトの位置
		float intensity;	//輝度
		Vector3 direction;	//ライトの向き
		float distance;	//ライトの届く最大距離
		float decay;	//減衰率
		float cosAngle;	//スポットライトの余弦
		float cosFalloffStart;	//falloff開始の角度
		float padding;
	};

	//初期化
	void Initialize();

	//描画
	//void Draw();

	SpotLightData& GetSpotLightData() { return *data_; }
	const SpotLightData& GetSpotLightData() const { return *data_; }

	Vector4& GetColor() { return data_->color; }
	const Vector4& GetColor() const { return data_->color; }

	Vector3& GetPosition() { return data_->position; }
	const Vector3& GetPosition() const { return data_->position; }

	float& GetIntensity() { return data_->intensity; }
	float GetIntensity() const { return data_->intensity; }

	Vector3& GetDirection() { return data_->direction; }
	const Vector3& GetDirection() const { return data_->direction; }

	float& GetDistance() { return data_->distance; }
	float GetDistance() const { return data_->distance; }

	float& GetDecay() { return data_->decay; }
	float GetDecay() const { return data_->decay; }

	float& GetCosAngle() { return data_->cosAngle; }
	float GetCosAngle() const { return data_->cosAngle; }

	float& GetCosFalloffStart() { return data_->cosFalloffStart; }
	float GetCosFalloffStart() const { return data_->cosFalloffStart; }

	void SetColor(const Vector4& color) { data_->color = color; }

	void SetPosition(const Vector3& position) { data_->position = position; }

	void SetIntensity(float intensity) { data_->intensity = intensity; }

	void SetDirection(const Vector3& direction) { data_->direction = direction; }

	void SetDistance(float distance) { data_->distance = distance; }

	void SetDecay(float decay) { data_->decay = decay; }

	void SetCosAngle(float cosAngle) { data_->cosAngle = cosAngle; }

	void SetCosFalloffStart(float cosFalloffStart) { data_->cosFalloffStart = cosFalloffStart; }

private:

	//データを書き込む
	SpotLightData* data_;

};

