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

	if (materialData_) 
	{
		//マテリアルのCBufferの場所を設定
		dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
		model_->Draw(true);
		return;
	}
	
	model_->Draw(false);

}

void Base3dObject::Draw(uint32_t textureHandle)
{
	//Transform用のCBufferの場所を設定
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(1, TransformationResource_->GetGPUVirtualAddress());

	if (materialData_)
	{
		//マテリアルのCBufferの場所を設定
		dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
		model_->Draw(textureHandle, true);
		return;
	}

	model_->Draw(textureHandle, false);

}

void Base3dObject::SetUVTransform(const Vector3& scale, const Vector3& rotate, const Vector3& translate)
{
	if (!materialData_)
	{
		CreateMaterialData();
	}
	Matrix4x4 uvTransformMatrix = MakeAffineMatrix(scale, rotate, translate);
	materialData_->uvTransform = uvTransformMatrix;
}

void Base3dObject::SetUVTransform(const EulerTransform& uvTransform)
{
	if (!materialData_)
	{
		CreateMaterialData();
	}
	Matrix4x4 uvTransformMatrix = MakeAffineMatrix(uvTransform);
	materialData_->uvTransform = uvTransformMatrix;
}

void Base3dObject::SetEnableLighting(bool enableLighting)
{
	if (!materialData_)
	{
		CreateMaterialData();
	}
	materialData_->enableLighting = enableLighting;
}

void Base3dObject::CreateMaterialData()
{
	//マテリアル用のリソースを作る。今回はcolor1つ分のサイズを用意する
	materialResource_ = dxCommon_->CreateBufferResource(sizeof(Material));
	//マテリアルにデータを書き込む
	//書き込むためのアドレスを取得
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	//白を書き込む
	materialData_->color = {1.0f, 1.0f, 1.0f, 1.0f};
	materialData_->enableLighting = true;
	materialData_->uvTransform = MakeIdentity4x4();
	materialData_->shininess = 40.0f;
}