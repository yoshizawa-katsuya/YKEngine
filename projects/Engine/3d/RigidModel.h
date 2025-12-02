#pragma once
#include "BaseModel.h"

/// <summary>
/// リジッドモデル。
/// メッシュの形状は変化しない。
/// </summary>
class RigidModel : public BaseModel
{
public:

	/// <summary>
	/// デストラクタ。
	/// </summary>
	~RigidModel() override;

	/// <summary>
	/// 球モデルを作成する。
	/// </summary>
	/// <param name="textureHandle">テクスチャハンドル</param>
	void CreateSphere(uint32_t textureHandle) override;

	/// <summary>
	/// 立方体モデルを作成する。
	/// </summary>
	/// <param name="textureHandle">テクスチャハンドル</param>
	void CreateCube(uint32_t textureHandle) override;

	/// <summary>
	/// 平面モデルを作成する。
	/// </summary>
	/// <param name="textureHandle">テクスチャハンドル</param>
	void CreatePlane(uint32_t textureHandle) override;

	/// <summary>
	/// リングモデルを作成する。
	/// </summary>
	/// <param name="textureHandle">テクスチャハンドル</param>
	void CreateRing(uint32_t textureHandle) override;

	/// <summary>
	/// 円柱モデルを作成する。
	/// </summary>
	/// <param name="textureHandle">テクスチャハンドル</param>
	void CreateCylinder(uint32_t textureHandle) override;

	/// <summary>
	/// スカイボックスモデルを作成する。
	/// </summary>
	/// <param name="textureHandle">テクスチャハンドル</param>
	void CreateSkyBox(uint32_t textureHandle) override;

private:


};

