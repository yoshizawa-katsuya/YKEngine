#pragma once
#include "DirectXCommon.h"
#include "PrimitiveDrawer.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include <array>
#include "SrvHeapManager.h"
#include "BaseModel.h"
class Camera;

class ModelPlatform
{
public:

	//シングルトンインスタンスの取得
	static ModelPlatform* GetInstance();

	//終了
	void Finalize();

	//初期化
	void Initialize(DirectXCommon* dxCommon, PrimitiveDrawer* primitiveDrawer, SrvHeapManager* srvHeapManager);

	void EndFrame();

	//共通描画設定
	void PreDraw();

	void SkinPreDraw();

	void InstancingPreDraw();

	void LinePreDraw();

	void LineDraw(const Matrix4x4& worldMatrix1, const Matrix4x4& worldMatrix2, Camera* camera);

	void SpherePreDraw();

	void SphereDraw(const Matrix4x4& worldMatrix, Camera* camera);

	std::shared_ptr<BaseModel> CreateRigidModel(const std::string& directoryPath, const std::string& filename, const Vector4& color = { 1.0f, 1.0f, 1.0f, 1.0f });

	std::shared_ptr<BaseModel> CreateSkinModel(const std::string& directoryPath, const std::string& filename, const Vector4& color = { 1.0f, 1.0f, 1.0f, 1.0f });

	std::shared_ptr<BaseModel> CreateSphere(uint32_t textureHandle);

	std::shared_ptr<BaseModel> CreatePlane(uint32_t textureHandle);

	DirectXCommon* GetDxCommon() const { return dxCommon_; }

	SrvHeapManager* GetSrvHeapManager() const { return srvHeapManager_; }

	//void SetDirectionalLight(DirectionalLight* directionalLight) { directionalLight_ = directionalLight; }

	void LightPreUpdate();

	void DirectionalLightUpdate(const DirectionalLight::DirectionalLightData& directionalLight);

	//void SetPointLight(PointLight* pointLight) { pointLight_ = pointLight; }

	void PointLightUpdate(const PointLight::PointLightData& pointLight);

	//void SetSpotLight(SpotLight* spotLight) { spotLight_ = spotLight; }

	void SpotLightUpdate(const SpotLight::SpotLightData& spotLight);

	void SetCamera(Camera* camera) { camera_ = camera; }

private:

	struct LineWVP
	{
		Matrix4x4 WVP1;
		Matrix4x4 WVP2;
	};

	struct LightCount
	{
		uint32_t directional;
		uint32_t point;
		uint32_t spot;
		float padding;
	};

	ModelPlatform() = default;
	~ModelPlatform() = default;
	ModelPlatform(ModelPlatform&) = default;
	ModelPlatform& operator=(ModelPlatform&) = default;

	DirectXCommon* dxCommon_;

	SrvHeapManager* srvHeapManager_;

	PrimitiveDrawer* primitiveDrawer_;

	//DirectionalLight* directionalLight_ = nullptr;

	std::unordered_map<std::string, std::shared_ptr<BaseModel>> models_;

	LightCount* lightCount_;
	Microsoft::WRL::ComPtr<ID3D12Resource> lightCountResource_;

	Microsoft::WRL::ComPtr<ID3D12Resource> directionalLightResouce_;
	DirectionalLight::DirectionalLightData* directionalLightDatas_ = nullptr;
	uint32_t kNumMaxDirectionalLight_ = 100;
	//uint32_t numDirectionalLight_ = 0;
	uint32_t directionalLightSrvIndex_;

	//PointLight* pointLight_ = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Resource> pointLightResouce_;
	PointLight::PointLightData* pointLightDatas_ = nullptr;
	uint32_t kNumMaxPointLight_ = 100;
	uint32_t pointLightSrvIndex_;

	//SpotLight* spotLight_ = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Resource> spotLightResouce_;
	SpotLight::SpotLightData* spotLightDatas_ = nullptr;
	uint32_t kNumMaxSpotLight_ = 100;
	uint32_t spotLightSrvIndex_;


	Camera* camera_;

	//VertexResourceを生成
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource_;

	Vector4* vertexData_ = nullptr;
	//頂点バッファビューを作成する
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};

	//static const uint32_t resourceNum_ = 2048;
	static const uint32_t resourceNum_ = 1;

	//TransformationMatrix用のリソースを作る 線分用
	std::array<Microsoft::WRL::ComPtr<ID3D12Resource>, resourceNum_> LineWVPResources_;
	//Microsoft::WRL::ComPtr<ID3D12Resource> LineWVPResource_;
	//データを書き込む
	std::array<LineWVP*, resourceNum_> LineWVPDatas_;
	//LineWVP* LineWVPData_ = nullptr;

	uint32_t lineIndex_ = 0;

	//TransformationMatrix用のリソースを作る。球用
	std::array<Microsoft::WRL::ComPtr<ID3D12Resource>, resourceNum_> SphereWVPResources_;
	//Microsoft::WRL::ComPtr<ID3D12Resource> WVPResource_;
	//データを書き込む
	std::array<Matrix4x4*, resourceNum_> SphereWVPDatas_;
	//Matrix4x4* WVPData_ = nullptr;

	uint32_t sphereIndex_ = 0;
	/*
	//TransformationMatrix用のリソースを作る。モデル用
	std::array<Microsoft::WRL::ComPtr<ID3D12Resource>, resourceNum_> ModelWVPResources_;
	//Microsoft::WRL::ComPtr<ID3D12Resource> WVPResource_;
	//データを書き込む
	std::array<TransformationMatrix*, resourceNum_> ModelWVPDatas_;
	//Matrix4x4* WVPData_ = nullptr;
	
	uint32_t modelIndex_ = 0;
	*/
};

