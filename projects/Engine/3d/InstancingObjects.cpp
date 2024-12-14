#include "InstancingObjects.h"

InstancingObject::InstancingObject()
	: dxCommon_(DirectXCommon::GetInstance())
{
}

void InstancingObject::Initialize(BaseModel* model, uint32_t maxInstances)
{
	assert(model);
	model_ = model;

	instanceDatas_.resize(maxInstances);
}
