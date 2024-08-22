#pragma once
#include "DirectXCommon.h"
#include "PrimitiveDrawer.h"
#include "DirectionalLight.h"

class ModelPlatform
{
public:

	//シングルトンインスタンスの取得
	static ModelPlatform* GetInstance();

	//終了
	void Finalize();

	//初期化
	void Initialize(DirectXCommon* dxCommon, PrimitiveDrawer* primitiveDrawer);

	//共通描画設定
	void PreDraw();

	DirectXCommon* GetDxCommon() const { return dxCommon_; }

	void SetDirectionalLight(DirectionalLight* directionalLight) { directionalLight_ = directionalLight; }

private:

	static ModelPlatform* instance_;

	ModelPlatform() = default;
	~ModelPlatform() = default;
	ModelPlatform(ModelPlatform&) = default;
	ModelPlatform& operator=(ModelPlatform&) = default;

	DirectXCommon* dxCommon_;

	PrimitiveDrawer* primitiveDrawer_;

	DirectionalLight* directionalLight_;
};

