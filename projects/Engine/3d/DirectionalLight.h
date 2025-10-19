#pragma once
#include "DirectXCommon.h"
#include <Struct.h>

/// <summary>
/// 平行光源。
/// 使用する場合、modelPtalformへデータをセットすること。
/// </summary>
class DirectionalLight
{
public:

	/// <summary>
	/// 平行光源データ構造体
	/// </summary>
	///	<param name="color">ライトの色</param>
	/// <param name="direction">ライトの向き</param>
	/// <param name="intensity">輝度</param>
	struct DirectionalLightData 
	{
		Vector4 color;	//ライトの色
		Vector3 direction;	//ライトの向き
		float intensity;	//輝度
	};

	/// <summary>
	/// 初期化。
	/// </summary>
	void Initialize();

	//描画
	//void Draw();

	DirectionalLightData& GetDirectionalLightData() { return *data_; }
	const DirectionalLightData& GetDirectionalLightData() const { return *data_; }

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

	//DirectXCommon* dxCommon_;

	//平行光源用のResourceを作成
	//Microsoft::WRL::ComPtr<ID3D12Resource> resource_;
	//データを書き込む
	DirectionalLightData* data_;

};