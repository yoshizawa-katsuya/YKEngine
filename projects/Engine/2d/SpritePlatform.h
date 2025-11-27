#pragma once
#include "DirectXCommon.h"
#include "PrimitiveDrawer.h"

/// <summary>
/// 2Dスプライト描画の基盤となるクラス。
/// スプライト描画の前後に必要な設定を行う。
/// シングルトンパターンで実装。
/// </summary>
class SpritePlatform
{
public:

	/// <summary>
	/// シングルトンインスタンスの取得。
	/// </summary>
	/// <returns>シングルトンインスタンス</returns>
	static SpritePlatform* GetInstance();

	/// <summary>
	/// 初期化。
	/// </summary>
	/// <param name="dxCommon">DirectX共通クラス</param>
	/// <param name="primitiveDrawer">プリミティブ描画クラス</param>
	void Initialize(DirectXCommon* dxCommon, PrimitiveDrawer* primitiveDrawer);

	/// <summary>
	/// 背景描画前の共通設定。
	/// </summary>
	void PreBackGroundDraw();

	/// <summary>
	/// スプライト描画前の共通設定。
	/// </summary>
	void PreDraw();

	DirectXCommon* GetDxCommon() const { return dxCommon_; }

private:

	SpritePlatform() = default;
	~SpritePlatform() = default;
	SpritePlatform(SpritePlatform&) = default;
	SpritePlatform& operator=(SpritePlatform&) = default;

	DirectXCommon* dxCommon_;

	PrimitiveDrawer* primitiveDrawer_;

};

