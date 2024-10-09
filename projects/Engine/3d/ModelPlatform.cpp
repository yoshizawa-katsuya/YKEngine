#include "ModelPlatform.h"
#include "Matrix.h"
#include "Camera.h"

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

void ModelPlatform::Initialize(DirectXCommon* dxCommon, PrimitiveDrawer* primitiveDrawer, SrvHeapManager* srvHeapManager)
{

	//引数で受け取ってメンバ変数に記録する
	dxCommon_ = dxCommon;
	primitiveDrawer_ = primitiveDrawer;
	srvHeapManager_ = srvHeapManager;

	//VertexResourceを生成
	vertexResource_ = dxCommon_->CreateBufferResource(sizeof(Vector4));

	//リソースの先頭のアドレスから使う
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点3つ分のサイズ
	vertexBufferView_.SizeInBytes = UINT(sizeof(Vector4));
	//1頂点当たりのサイズ
	vertexBufferView_.StrideInBytes = sizeof(Vector4);

	//頂点リソースにデータを書き込む
	//書き込むためのアドレスを取得
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));

	*vertexData_ = {0.0f, 0.0f, 0.0f, 1.0f};

	/*
	//transformationMatrixのリソースを作る
	LineWVPResource_ = dxCommon_->CreateBufferResource(sizeof(LineWVP));
	//データを書き込む
	//書き込むためのアドレスを取得
	LineWVPResource_->Map(0, nullptr, reinterpret_cast<void**>(&LineWVPData_));
	//単位行列を書き込んでおく
	LineWVPData_->WVP1 = MakeIdentity4x4();
	LineWVPData_->WVP2 = MakeIdentity4x4();
	*/

	for (uint32_t i = 0; i < resourceNum_; i++) {
		//transformationMatrixのリソースを作る
		LineWVPResources_[i] = dxCommon_->CreateBufferResource(sizeof(LineWVP));
		//書き込むためのアドレスを取得
		LineWVPResources_[i]->Map(0, nullptr, reinterpret_cast<void**>(&LineWVPDatas_[i]));
		//単位行列を書き込んでおく
		LineWVPDatas_[i]->WVP1 = MakeIdentity4x4();
		LineWVPDatas_[i]->WVP2 = MakeIdentity4x4();
	}

	/*
	WVPResource_ = dxCommon_->CreateBufferResource(sizeof(Matrix4x4));
	WVPResource_->Map(0, nullptr, reinterpret_cast<void**>(&WVPData_));
	*WVPData_ = MakeIdentity4x4();
	*/

	for (uint32_t i = 0; i < resourceNum_; i++) {
		SphereWVPResources_[i] = dxCommon_->CreateBufferResource(sizeof(Matrix4x4));
		SphereWVPResources_[i]->Map(0, nullptr, reinterpret_cast<void**>(&SphereWVPDatas_[i]));
		*SphereWVPDatas_[i] = MakeIdentity4x4();
	}

	for (uint32_t i = 0; i < resourceNum_; i++) {
		ModelWVPResources_[i] = dxCommon_->CreateBufferResource(sizeof(TransformationMatrix));
		ModelWVPResources_[i]->Map(0, nullptr, reinterpret_cast<void**>(&ModelWVPDatas_[i]));
		ModelWVPDatas_[i]->World = MakeIdentity4x4();
		ModelWVPDatas_[i]->WVP = MakeIdentity4x4();
	}
}

void ModelPlatform::EndFrame()
{

	lineIndex_ = 0;
	sphereIndex_ = 0;
	modelIndex_ = 0;

}

void ModelPlatform::PreDraw()
{

	
	primitiveDrawer_->SetPipelineSet(dxCommon_->GetCommandList(), BlendMode::kBlendModeNone);
	dxCommon_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	directionalLight_->Draw();

}

void ModelPlatform::SkinPreDraw()
{

	primitiveDrawer_->SetPipelineSet(dxCommon_->GetCommandList(), BlendMode::kSkinModelMode);
	dxCommon_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	directionalLight_->Draw();

}

void ModelPlatform::ModelDraw(const Matrix4x4& WVP, const Matrix4x4& World, Camera* camera)
{

	ModelWVPDatas_[modelIndex_]->WVP = WVP;
	ModelWVPDatas_[modelIndex_]->World = World;
	ModelWVPDatas_[modelIndex_]->WorldInverseTranspose = Transpose(Inverse(World));

	//wvp用のCBufferの場所を設定
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(1, ModelWVPResources_[modelIndex_]->GetGPUVirtualAddress());

	modelIndex_++;

}

void ModelPlatform::LinePreDraw()
{

	primitiveDrawer_->SetPipelineSet(dxCommon_->GetCommandList(), BlendMode::kLineMode);
	dxCommon_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
	dxCommon_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);	//VBVを設定

}

void ModelPlatform::LineDraw(const Matrix4x4& worldMatrix1, const Matrix4x4& worldMatrix2, Camera* camera)
{

	

	Matrix4x4 worldViewProjectionMatrix1;
	Matrix4x4 worldViewProjectionMatrix2;
	if (camera) {
		const Matrix4x4& viewProjectionMatrix = camera->GetViewProjection();
		worldViewProjectionMatrix1 = Multiply(worldMatrix1, viewProjectionMatrix);
		worldViewProjectionMatrix2 = Multiply(worldMatrix2, viewProjectionMatrix);
	}
	else {
		worldViewProjectionMatrix1 = worldMatrix1;
		worldViewProjectionMatrix2 = worldMatrix2;
	}

	LineWVPDatas_[lineIndex_]->WVP1 = worldViewProjectionMatrix1;
	LineWVPDatas_[lineIndex_]->WVP2 = worldViewProjectionMatrix2;

	//wvp用のCBufferの場所を設定
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(0, LineWVPResources_[lineIndex_]->GetGPUVirtualAddress());
	
	//描画1(DrawCall/ドローコール)。3頂点で1つのインスタンス。インスタンスについては今後
		//commandList_->DrawIndexedInstanced((kSubdivision * kSubdivision * 6), 1, 0, 0, 0);
	dxCommon_->GetCommandList()->DrawInstanced(1, 1, 0, 0);

	lineIndex_++;

}

void ModelPlatform::SpherePreDraw()
{

	primitiveDrawer_->SetPipelineSet(dxCommon_->GetCommandList(), BlendMode::kSphereMode);
	dxCommon_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
	dxCommon_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);	//VBVを設定
	
}

void ModelPlatform::SphereDraw(const Matrix4x4& worldMatrix, Camera* camera)
{

	Matrix4x4 worldViewProjectionMatrix;
	if (camera) {
		const Matrix4x4& viewProjectionMatrix = camera->GetViewProjection();
		worldViewProjectionMatrix = Multiply(worldMatrix, viewProjectionMatrix);
	}
	else {
		worldViewProjectionMatrix = worldMatrix;
	}

	*SphereWVPDatas_[sphereIndex_] = worldViewProjectionMatrix;

	//wvp用のCBufferの場所を設定
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(0, SphereWVPResources_[sphereIndex_]->GetGPUVirtualAddress());

	//描画1(DrawCall/ドローコール)。3頂点で1つのインスタンス。インスタンスについては今後
		//commandList_->DrawIndexedInstanced((kSubdivision * kSubdivision * 6), 1, 0, 0, 0);
	dxCommon_->GetCommandList()->DrawInstanced(1, 1, 0, 0);

	sphereIndex_++;
}
