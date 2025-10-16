#pragma once
#include "BaseModel.h"

/// <summary>
/// リジッドモデル。
/// メッシュの形状は変化しない。
/// </summary>
class RigidModel : public BaseModel
{
public:

	~RigidModel() override;

	void CreateSphere(uint32_t textureHandle) override;

	void CreateCube(uint32_t textureHandle) override;

	void CreatePlane(uint32_t textureHandle) override;

	void CreateRing(uint32_t textureHandle) override;

	void CreateCylinder(uint32_t textureHandle) override;

	void CreateSkyBox(uint32_t textureHandle) override;

private:


};

