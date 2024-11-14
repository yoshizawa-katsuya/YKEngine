#include "Base3dObject.h"
#include "Matrix.h"
#include "Camera.h"

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

void Base3dObject::Update(const WorldTransform& worldTransform, Camera* camera)
{

	Matrix4x4 worldViewProjectionMatrix;
	if (camera) {
		const Matrix4x4& viewProjectionMatrix = camera->GetViewProjection();
		worldViewProjectionMatrix = Multiply(worldTransform.worldMatrix_, viewProjectionMatrix);
	}
	else {
		worldViewProjectionMatrix = worldTransform.worldMatrix_;
	}

	TransformationData_->WVP = worldViewProjectionMatrix;
	TransformationData_->World = worldTransform.worldMatrix_;
	TransformationData_->WorldInverseTranspose = Transpose(Inverse(worldTransform.worldMatrix_));

}

void Base3dObject::Update(const WorldTransform& worldTransform, Camera* camera, Animation* animation)
{
	Update(worldTransform, camera);
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
