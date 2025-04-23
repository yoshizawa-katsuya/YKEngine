#include "Base3dObject.h"
#include "Matrix.h"
#include "Camera.h"
#include "Animation.h"

Base3dObject::Base3dObject()
	: dxCommon_(DirectXCommon::GetInstance())
{
}

Base3dObject::~Base3dObject()
{
}

void Base3dObject::Initialize(BaseModel* model)
{

	assert(model);
	model_ = model;

	TransformationResource_ = dxCommon_->CreateBufferResource(sizeof(TransformationMatrix));
	TransformationResource_->Map(0, nullptr, reinterpret_cast<void**>(&TransformationData_));
	TransformationData_->World = MakeIdentity4x4();
	TransformationData_->WVP = MakeIdentity4x4();
	TransformationData_->WorldInverseTranspose = MakeIdentity4x4();

}

void Base3dObject::WorldTransformUpdate(const WorldTransform& worldTransform)
{

	TransformationData_->World = worldTransform.worldMatrix_;

}

void Base3dObject::AnimationUpdate(Animation* animation)
{
	TransformationData_->World = Multiply(TransformationData_->World, animation->Reproducing(model_));
}

void Base3dObject::CameraUpdate(Camera* camera)
{

	Matrix4x4 worldViewProjectionMatrix;
	if (camera) {
		worldViewProjectionMatrix = Multiply(TransformationData_->World, camera->GetViewProjection());
	}
	else {
		worldViewProjectionMatrix = TransformationData_->World;
	}

	TransformationData_->WVP = worldViewProjectionMatrix;
	TransformationData_->WorldInverseTranspose = Transpose(Inverse(TransformationData_->World));

}

void Base3dObject::Draw()
{
	//Transform用のCBufferの場所を設定
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(1, TransformationResource_->GetGPUVirtualAddress());

	model_->Draw();

}

void Base3dObject::Draw(uint32_t textureHandle)
{
	//Transform用のCBufferの場所を設定
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(1, TransformationResource_->GetGPUVirtualAddress());

	model_->Draw(textureHandle);

}
