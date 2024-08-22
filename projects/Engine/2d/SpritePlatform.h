#pragma once
#include "DirectXCommon.h"
#include "PrimitiveDrawer.h"

class SpritePlatform
{
public:

	//シングルトンインスタンスの取得
	static SpritePlatform* GetInstance();

	//終了
	void Finalize();

	//初期化
	void Initialize(DirectXCommon* dxCommon, PrimitiveDrawer* primitiveDrawer);

	//共通描画設定
	void PreDraw();

	DirectXCommon* GetDxCommon() const { return dxCommon_; }

private:

	static SpritePlatform* instance_;

	SpritePlatform() = default;
	~SpritePlatform() = default;
	SpritePlatform(SpritePlatform&) = default;
	SpritePlatform& operator=(SpritePlatform&) = default;

	DirectXCommon* dxCommon_;

	PrimitiveDrawer* primitiveDrawer_;

};

