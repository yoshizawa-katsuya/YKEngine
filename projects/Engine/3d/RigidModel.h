#pragma once
#include "BaseModel.h"

class RigidModel : public BaseModel
{
public:

	~RigidModel() override;

	void CreateSphere(uint32_t textureHandle) override;

	void Update() override;

private:


};

