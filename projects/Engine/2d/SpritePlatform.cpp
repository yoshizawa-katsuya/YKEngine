#include "SpritePlatform.h"

SpritePlatform* SpritePlatform::instance_ = nullptr;

SpritePlatform* SpritePlatform::GetInstance()
{
	if (instance_ == nullptr) {
		instance_ = new SpritePlatform;
	}
	return instance_;
}

void SpritePlatform::Finalize()
{
	delete instance_;
	instance_ = nullptr;
}

void SpritePlatform::Initialize(DirectXCommon* dxCommon, PrimitiveDrawer* primitiveDrawer)
{

	//引数で受け取ってメンバ変数に記録する
	dxCommon_ = dxCommon;
	primitiveDrawer_ = primitiveDrawer;


}

void SpritePlatform::PreDraw()
{

	primitiveDrawer_->SetPipelineSet(dxCommon_->GetCommandList(), BlendMode::kBlendModeNoneSprite);
	dxCommon_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

}
