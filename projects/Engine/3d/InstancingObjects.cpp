#include "InstancingObjects.h"
#include "ModelPlatform.h"
#include "Camera.h"
#include "Matrix.h"

InstancingObjects::InstancingObjects()
	: dxCommon_(DirectXCommon::GetInstance())
	, srvHeapManager_(ModelPlatform::GetInstance()->GetSrvHeapManager())
	, numInstance_(0)
{
}

void InstancingObjects::Initialize(BaseModel* model, uint32_t maxInstances)
{
	assert(model);
	model_ = model;

	kNumMaxInstance_ = maxInstances;

	instancingResouce_ = dxCommon_->CreateBufferResource(sizeof(TransformationMatrix) * kNumMaxInstance_);
	instancingResouce_->Map(0, nullptr, reinterpret_cast<void**>(&instancingData_));

	//TODO: ゲームループなどで初期化するたびにsrvIndexが先に進んでいくので改善する
	instancingSrvIndex_ = srvHeapManager_->Allocate();

	srvHeapManager_->CreateSRVforStructuredBuffer(instancingSrvIndex_, instancingResouce_.Get(), kNumMaxInstance_, sizeof(TransformationMatrix));

}

void InstancingObjects::PreUpdate()
{
	numInstance_ = 0;
}

void InstancingObjects::WorldTransformUpdate(const WorldTransform& worldTransform)
{
	instancingData_[numInstance_].World = worldTransform.worldMatrix_;
	numInstance_++;

	return;
}

/*
void InstancingObjects::AddWorldTransform(const WorldTransform& worldTransform)
{

	instancingData_[numInstance_].World = worldTransform.worldMatrix_;
	numInstance_++;

	return;

}
*/
void InstancingObjects::CameraUpdate(Camera* camera)
{

	for (uint32_t index = 0; index < numInstance_; ++index) {
		instancingData_[index].WVP = Multiply(instancingData_[index].World, camera->GetViewProjection());
		instancingData_[index].WorldInverseTranspose = Transpose(Inverse(instancingData_[index].World));
	}

}

void InstancingObjects::Draw()
{

	srvHeapManager_->SetGraphicsRootDescriptorTable(1, instancingSrvIndex_);

	model_->InstancingDraw(numInstance_);

}
