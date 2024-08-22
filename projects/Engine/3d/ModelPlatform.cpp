#include "ModelPlatform.h"

ModelPlatform* ModelPlatform::instance_ = nullptr;

ModelPlatform* ModelPlatform::GetInstance()
{
	if (instance_ == nullptr) {
		instance_ = new ModelPlatform;
	}
	return instance_;
}

void ModelPlatform::Finalize()
{
	delete instance_;
	instance_ = nullptr;
}

void ModelPlatform::Initialize(DirectXCommon* dxCommon, PrimitiveDrawer* primitiveDrawer)
{

	//引数で受け取ってメンバ変数に記録する
	dxCommon_ = dxCommon;
	primitiveDrawer_ = primitiveDrawer;
	
}

void ModelPlatform::PreDraw()
{

	
	primitiveDrawer_->SetPipelineSet(dxCommon_->GetCommandList(), BlendMode::kBlendModeNone);
	dxCommon_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	directionalLight_->Draw();

}