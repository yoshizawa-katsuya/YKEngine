#pragma once
#include "BaseModel.h"

class Base3dObject
{
public:

	Base3dObject();

	virtual ~Base3dObject() = 0;

	virtual void Initialize(BaseModel* model);

	virtual void Update(const WorldTransform& worldTransform, Camera* camera);

	virtual void Draw();

	virtual void Draw(uint32_t textureHandle);

	const BaseModel& GetModel() const { return *model_; }
	BaseModel& GetModel() { return *model_; }

protected:

	DirectXCommon* dxCommon_;

	//TransformationMatrix用のリソースを作る。モデル用
	Microsoft::WRL::ComPtr<ID3D12Resource> TransformationResource_;
	//データを書き込む
	TransformationMatrix* TransformationData_;
	//Matrix4x4* WVPData_ = nullptr;

	BaseModel* model_;

};

