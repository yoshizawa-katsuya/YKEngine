#pragma once
#include "BaseModel.h"

class InstancingObjects
{
public:

	InstancingObjects();
	
	//RigidModelのみに対応
	void Initialize(BaseModel* model, uint32_t maxInstances);

	void AddWorldTransform(const WorldTransform& worldTransform);

	void CameraUpdate(Camera* camera);

	void Draw();

private:

	DirectXCommon* dxCommon_;
	SrvHeapManager* srvHeapManager_;

	//TransformationMatrix用のリソースを作る。モデル用
	Microsoft::WRL::ComPtr<ID3D12Resource> instancingResouce_;
	//データを書き込む
	//std::vector<TransformationMatrix*> instanceDatas_;
	TransformationMatrix* instancingData_;
	//Matrix4x4* WVPData_ = nullptr;
	uint32_t kNumMaxInstance_;
	uint32_t numInstance_;
	uint32_t instancingSrvIndex_;


	BaseModel* model_;
};

