#include "InstancingObjects.h"
#include "ModelPlatform.h"
#include "Camera.h"
#include "Matrix.h"

InstancingObject::InstancingObject()
	: dxCommon_(DirectXCommon::GetInstance())
	, srvHeapManager_(ModelPlatform::GetInstance()->GetSrvHeapManager())
	, numInstance_(0)
{
}

void InstancingObject::Initialize(BaseModel* model, uint32_t maxInstances)
{
	assert(model);
	model_ = model;

	kNumMaxInstance_ = maxInstances;

	instancingResouce_ = dxCommon_->CreateBufferResource(sizeof(TransformationMatrix) * kNumMaxInstance_);
	instancingResouce_->Map(0, nullptr, reinterpret_cast<void**>(&instancingData_));

	instancingSrvIndex_ = srvHeapManager_->Allocate();

	srvHeapManager_->CreateSRVforStructuredBuffer(instancingSrvIndex_, instancingResouce_.Get(), kNumMaxInstance_, sizeof(TransformationMatrix));

}

uint32_t InstancingObject::AddWorldTransform(const WorldTransform& worldTransform)
{

	uint32_t index = numInstance_;
	numInstance_++;

	instancingData_[index].World = worldTransform.worldMatrix_;

	return index;

}

void InstancingObject::CameraUpdate(Camera* camera)
{

	for (uint32_t index = 0; index < numInstance_; ++index) {
		instancingData_[index].WVP = Multiply(instancingData_[index].World, camera->GetViewProjection());
		instancingData_[index].WorldInverseTranspose = Transpose(Inverse(instancingData_[index].World));
	}

}

void InstancingObject::Draw()
{

	srvHeapManager_->SetGraphicsRootDescriptorTable(1, instancingSrvIndex_);

	model_->InstancingDraw(numInstance_);

}
