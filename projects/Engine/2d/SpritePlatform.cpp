#include "SpritePlatform.h"

SpritePlatform* SpritePlatform::GetInstance()
{
	static SpritePlatform instance;
	return &instance;
}

void SpritePlatform::Finalize()
{
	
}

void SpritePlatform::Initialize(DirectXCommon* dxCommon, PrimitiveDrawer* primitiveDrawer)
{

	//引数で受け取ってメンバ変数に記録する
	dxCommon_ = dxCommon;
	primitiveDrawer_ = primitiveDrawer;


}

void SpritePlatform::PreBackGroundDraw()
{
	
	primitiveDrawer_->SetPipelineSet(dxCommon_->GetCommandList(), BlendMode::kBackGroundSprite);
	dxCommon_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

}

void SpritePlatform::PreDraw()
{

	primitiveDrawer_->SetPipelineSet(dxCommon_->GetCommandList(), BlendMode::kBlendModeNormalSprite);
	dxCommon_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

}
