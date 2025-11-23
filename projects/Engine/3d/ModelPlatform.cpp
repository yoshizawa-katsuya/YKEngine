#include "ModelPlatform.h"
#include "Matrix.h"
#include "Camera.h"
#include "RigidModel.h"
#include "SkinModel.h"
#include "RootParams.h"

ModelPlatform* ModelPlatform::instance_ = nullptr;

ModelPlatform* ModelPlatform::GetInstance()
{
	if (instance_ == nullptr)
	{
		instance_ = new ModelPlatform();
	}
	return instance_;
}

void ModelPlatform::Finalize()
{
	//インスタンスを破棄
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

	for (uint32_t i = 0; i < kResourceNum_; i++) {
		//transformationMatrixのリソースを作る
		LineWVPResources_[i] = dxCommon_->CreateBufferResource(sizeof(LineWVP));
		//書き込むためのアドレスを取得
		LineWVPResources_[i]->Map(0, nullptr, reinterpret_cast<void**>(&LineWVPDatas_[i]));
		//単位行列を書き込んでおく
		LineWVPDatas_[i]->WVP1 = MakeIdentity4x4();
		LineWVPDatas_[i]->WVP2 = MakeIdentity4x4();
	}

	for (uint32_t i = 0; i < kResourceNum_; i++) {
		SphereWVPResources_[i] = dxCommon_->CreateBufferResource(sizeof(Matrix4x4));
		SphereWVPResources_[i]->Map(0, nullptr, reinterpret_cast<void**>(&SphereWVPDatas_[i]));
		*SphereWVPDatas_[i] = MakeIdentity4x4();
	}
	
	//ライトカウント
	lightCountResource_ = dxCommon_->CreateBufferResource(sizeof(LightCount));
	lightCountResource_->Map(0, nullptr, reinterpret_cast<void**>(&lightCount_));
	lightCount_->directional = 0;
	lightCount_->point = 0;
	lightCount_->spot = 0;

	//平行光源
	directionalLightResouce_ = dxCommon_->CreateBufferResource(sizeof(DirectionalLight::DirectionalLightData) * kNumMaxDirectionalLight_);
	directionalLightResouce_->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightDatas_));

	directionalLightSrvIndex_ = srvHeapManager_->Allocate();

	srvHeapManager_->CreateSRVforStructuredBuffer(directionalLightSrvIndex_, directionalLightResouce_.Get(), kNumMaxDirectionalLight_, sizeof(DirectionalLight::DirectionalLightData));

	//点光源
	pointLightResouce_ = dxCommon_->CreateBufferResource(sizeof(PointLight::PointLightData) * kNumMaxPointLight_);
	pointLightResouce_->Map(0, nullptr, reinterpret_cast<void**>(&pointLightDatas_));

	pointLightSrvIndex_ = srvHeapManager_->Allocate();

	srvHeapManager_->CreateSRVforStructuredBuffer(pointLightSrvIndex_, pointLightResouce_.Get(), kNumMaxPointLight_, sizeof(PointLight::PointLightData));

	//スポットライト
	spotLightResouce_ = dxCommon_->CreateBufferResource(sizeof(SpotLight::SpotLightData) * kNumMaxSpotLight_);
	spotLightResouce_->Map(0, nullptr, reinterpret_cast<void**>(&spotLightDatas_));

	spotLightSrvIndex_ = srvHeapManager_->Allocate();

	srvHeapManager_->CreateSRVforStructuredBuffer(spotLightSrvIndex_, spotLightResouce_.Get(), kNumMaxSpotLight_, sizeof(SpotLight::SpotLightData));

}

void ModelPlatform::EndFrame()
{

	lineIndex_ = 0;
	sphereIndex_ = 0;

}

void ModelPlatform::PreDraw()
{

	
	primitiveDrawer_->SetPipelineSet(dxCommon_->GetCommandList(), DrawMode::kBlendModeNormal);
	dxCommon_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	srvHeapManager_->SetGraphicsRootDescriptorTable(static_cast<size_t>(ModelRootParam::kDirectionalLight), directionalLightSrvIndex_);
	srvHeapManager_->SetGraphicsRootDescriptorTable(static_cast<size_t>(ModelRootParam::kPointLight), pointLightSrvIndex_);
	srvHeapManager_->SetGraphicsRootDescriptorTable(static_cast<size_t>(ModelRootParam::kSpotLight), spotLightSrvIndex_);
	camera_->SetCameraReaource(static_cast<uint32_t>(ModelRootParam::kCamera));

	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(static_cast<size_t>(ModelRootParam::kLightCount), lightCountResource_->GetGPUVirtualAddress());

}

void ModelPlatform::SkyBoxPreDraw()
{
	primitiveDrawer_->SetPipelineSet(dxCommon_->GetCommandList(), DrawMode::kSkyboxMode);
	dxCommon_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void ModelPlatform::SkinPreDraw()
{

	primitiveDrawer_->SetPipelineSet(dxCommon_->GetCommandList(), DrawMode::kSkinModelMode);
	dxCommon_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	srvHeapManager_->SetGraphicsRootDescriptorTable(static_cast<size_t>(SkinModelRootParam::kDirectionalLight), directionalLightSrvIndex_);
	srvHeapManager_->SetGraphicsRootDescriptorTable(static_cast<size_t>(SkinModelRootParam::kPointLight), pointLightSrvIndex_);
	srvHeapManager_->SetGraphicsRootDescriptorTable(static_cast<size_t>(SkinModelRootParam::kSpotLight), spotLightSrvIndex_);
	camera_->SetCameraReaource(static_cast<uint32_t>(SkinModelRootParam::kCamera));

	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(static_cast<size_t>(SkinModelRootParam::kLightCount), lightCountResource_->GetGPUVirtualAddress());

}

void ModelPlatform::InstancingPreDraw()
{

	primitiveDrawer_->SetPipelineSet(dxCommon_->GetCommandList(), DrawMode::kBlendModeNormalinstancing);
	dxCommon_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	srvHeapManager_->SetGraphicsRootDescriptorTable(static_cast<size_t>(ModelRootParam::kDirectionalLight), directionalLightSrvIndex_);
	srvHeapManager_->SetGraphicsRootDescriptorTable(static_cast<size_t>(ModelRootParam::kPointLight), pointLightSrvIndex_);
	srvHeapManager_->SetGraphicsRootDescriptorTable(static_cast<size_t>(ModelRootParam::kSpotLight), spotLightSrvIndex_);
	camera_->SetCameraReaource(static_cast<uint32_t>(ModelRootParam::kCamera));

	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(static_cast<size_t>(ModelRootParam::kLightCount), lightCountResource_->GetGPUVirtualAddress());

}

void ModelPlatform::LinePreDraw()
{

	primitiveDrawer_->SetPipelineSet(dxCommon_->GetCommandList(), DrawMode::kLineMode);
	dxCommon_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
	dxCommon_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);	//VBVを設定

}

void ModelPlatform::LineDraw(const Matrix4x4& worldMatrix1, const Matrix4x4& worldMatrix2, Camera* camera)
{
	//線の始点と終点のワールドビュー射影行列を計算
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
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(static_cast<size_t>(DebudLineRootParam::kWVP), LineWVPResources_[lineIndex_]->GetGPUVirtualAddress());
	
	//描画1(DrawCall/ドローコール)。
	dxCommon_->GetCommandList()->DrawInstanced(1, 1, 0, 0);

	lineIndex_++;

}

void ModelPlatform::SpherePreDraw()
{

	primitiveDrawer_->SetPipelineSet(dxCommon_->GetCommandList(), DrawMode::kSphereMode);
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
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(static_cast<size_t>(DebugSphereRootParam::kWVP), SphereWVPResources_[sphereIndex_]->GetGPUVirtualAddress());

	//描画1(DrawCall/ドローコール)。
	dxCommon_->GetCommandList()->DrawInstanced(1, 1, 0, 0);

	sphereIndex_++;
}

std::shared_ptr<BaseModel> ModelPlatform::CreateRigidModel(const std::string& directoryPath, const std::string& filename, const Vector4& color)
{
	//すでに同じ名前のモデルがあればそれを返す
	if (models_.contains(filename)) 
	{
		return models_[filename];
	}
	models_[filename] = std::make_shared<RigidModel>();
	models_[filename]->CreateModel(directoryPath, filename, color);

	return models_[filename];
}

std::shared_ptr<BaseModel> ModelPlatform::CreateSkinModel(const std::string& directoryPath, const std::string& filename, const Vector4& color)
{
	//すでに同じ名前のモデルがあればそれを返す
	if (models_.contains(filename))
	{
		return models_[filename];
	}
	models_[filename] = std::make_shared<SkinModel>();
	models_[filename]->CreateModel(directoryPath, filename, color);

	return models_[filename];
}

std::shared_ptr<BaseModel> ModelPlatform::CreateSphere(uint32_t textureHandle, const std::string& modelName)
{
	std::string name = "PrimitiveSphere" + modelName;
	//すでに同じ名前のモデルがあればそれを返す
	if (models_.contains(name))
	{
		return models_[name];
	}
	models_[name] = std::make_shared<RigidModel>();
	models_[name]->CreateSphere(textureHandle);

	return models_[name];
}

std::shared_ptr<BaseModel> ModelPlatform::CreateCube(uint32_t textureHandle, const std::string& modelName)
{
	std::string name = "PrimitiveCube" + modelName;
	//すでに同じ名前のモデルがあればそれを返す
	if (models_.contains(name))
	{
		return models_[name];
	}
	models_[name] = std::make_shared<RigidModel>();
	models_[name]->CreateCube(textureHandle);

	return models_[name];
}

std::shared_ptr<BaseModel> ModelPlatform::CreatePlane(uint32_t textureHandle, const std::string& modelName)
{
	std::string name = "PrimitivePlane" + modelName;
	//すでに同じ名前のモデルがあればそれを返す
	if (models_.contains(name)) 
	{
		return models_[name];
	}
	models_[name] = std::make_shared<RigidModel>();
	models_[name]->CreatePlane(textureHandle);

	return models_[name];
}

std::shared_ptr<BaseModel> ModelPlatform::CreateRing(uint32_t textureHandle, const std::string& modelName)
{
	std::string name = "PrimitiveRing" + modelName;
	//すでに同じ名前のモデルがあればそれを返す
	if (models_.contains(name))
	{
		return models_[name];
	}
	models_[name] = std::make_shared<RigidModel>();
	models_[name]->CreateRing(textureHandle);

	return models_[name];
}

std::shared_ptr<BaseModel> ModelPlatform::CreateCylinder(uint32_t textureHandle, const std::string& modelName)
{
	std::string name = "PrimitiveCylinder" + modelName;
	//すでに同じ名前のモデルがあればそれを返す
	if (models_.contains(name))
	{
		return models_[name];
	}
	models_[name] = std::make_shared<RigidModel>();
	models_[name]->CreateCylinder(textureHandle);

	return models_[name];
}

std::shared_ptr<BaseModel> ModelPlatform::CreateSkyBox(uint32_t textureHandle, const std::string& modelName)
{
	std::string name = "PrimitiveSkyBox" + modelName;
	//すでに同じ名前のモデルがあればそれを返す
	if (models_.contains(name)) 
	{
		return models_[name];
	}
	models_[name] = std::make_shared<RigidModel>();
	models_[name]->CreateSkyBox(textureHandle);

	return models_[name];
}

void ModelPlatform::LightPreUpdate()
{
	lightCount_->directional = 0;
	lightCount_->point = 0;
	lightCount_->spot = 0;
}

void ModelPlatform::DirectionalLightUpdate(const DirectionalLight::DirectionalLightData& directionalLight)
{
	directionalLightDatas_[lightCount_->directional] = directionalLight;
	directionalLightDatas_[lightCount_->directional].direction = Normalize(directionalLightDatas_[lightCount_->directional].direction);
	lightCount_->directional++;

	return;
}

void ModelPlatform::PointLightUpdate(const PointLight::PointLightData& pointLight)
{
	pointLightDatas_[lightCount_->point] = pointLight;
	lightCount_->point++;

	return;
}

void ModelPlatform::SpotLightUpdate(const SpotLight::SpotLightData& spotLight)
{
	spotLightDatas_[lightCount_->spot] = spotLight;
	spotLightDatas_[lightCount_->spot].direction = Normalize(spotLightDatas_[lightCount_->spot].direction);
	lightCount_->spot++;

	return;
}
