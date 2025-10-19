#pragma once
#include "DirectXCommon.h"
#include <Struct.h>

/// <summary>
/// 点光源。
/// 使用する場合、modelPtalformへデータをセットすること。
/// </summary>
class PointLight
{
public:

	/// <summary>
	/// 点光源のデータ構造体
	/// </summary>
	/// <param name="color">ライトの色</param>
	/// <param name="position">ライトの位置</param>
	/// <param name="intensity">輝度</param>
	/// <param name="radius">ライトの届く最大距離</param>
	/// <param name="decay">減衰率</param>
	struct PointLightData 
	{
		Vector4 color;	//ライトの色
		Vector3 position;	//ライトの位置
		float intensity;	//輝度
		float radius;	//ライトの届く最大距離
		float decay;	//減衰率
		float padding[2];
	};

	/// <summary>
	/// 初期化。
	/// </summary>
	void Initialize();

	//描画
	//void Draw();

	PointLightData& GetPointLightData() { return *data_; }
	const PointLightData& GetPointLightData() const { return *data_; }

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

	//データを書き込む
	PointLightData* data_;

};

