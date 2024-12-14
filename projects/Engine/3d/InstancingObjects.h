#pragma once
#include "BaseModel.h"

class InstancingObject
{
public:

	InstancingObject();
	
	void Initialize(BaseModel* model, uint32_t maxInstances);

	void AddWorldTransform(const TransformationMatrix& instancedata);

private:

	DirectXCommon* dxCommon_;

	//TransformationMatrix用のリソースを作る。モデル用
	Microsoft::WRL::ComPtr<ID3D12Resource> instancingResouce_;
	//データを書き込む
	std::vector<TransformationMatrix*> instanceDatas_;
	//Matrix4x4* WVPData_ = nullptr;

	BaseModel* model_;
};

