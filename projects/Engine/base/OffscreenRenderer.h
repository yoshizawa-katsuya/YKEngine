#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <cstdint>
#include "Vector4.h"
#include <vector>
#include "Struct.h"
class SrvHeapManager;
class PrimitiveDrawer;
class DirectXCommon;
enum class DrawMode;

enum class RenderTextureType
{
	// オフスクリーンレンダリング用のテクスチャ
	OffscreenRender,
	GrayScale,
	Vignette,
	BoxFilter,
	GaussianFilter,
	Outline,
};

class OffscreenRenderer
{
public:

	//シングルトンインスタンスの取得
	static OffscreenRenderer* GetInstance();

	void Initialize(SrvHeapManager* srvHeapManager);

	void PreDrawRenderTexture();

	void PostDrawRenderTexture(PrimitiveDrawer* primitiveDrawer, SrvHeapManager* srvHeapManager);

	void UpdateOutlineMaterialData(const Matrix4x4& projectionMatrix);

	bool* GetUseOffscreenRenderPtr() { return &useOffscreenRender_; }
	bool GetUseOffscreenRender() { return useOffscreenRender_; }

	//オフスクリーンレンダリングを使用する場合sceneのUpdateでtrueにする
	void SetUseOffscreenRender(bool use) { useOffscreenRender_ = use; }

	//レンダーテクスチャの種類を設定
	void SetRenderTextureType(RenderTextureType type) { renderTextureType_ = type; }

private:

	OffscreenRenderer() = default;
	~OffscreenRenderer() = default;
	OffscreenRenderer(OffscreenRenderer&) = delete;
	const OffscreenRenderer& operator=(OffscreenRenderer&) = delete;

	//RenderTexture作成
	void CreateRenderTexture();

	void CreateRenderTextureSRV(SrvHeapManager* srvHeapManager);

	void CreateRenderTextureResource(int32_t width, int32_t height, DXGI_FORMAT format, const Vector4& clearColor);

	void CreateDepthTextureSRV(SrvHeapManager* srvHerpManager);

	struct OutlineMaterial
	{
		Matrix4x4 projectionInverseMatrix; // プロジェクション逆行列
	};

	DirectXCommon* dxCommon_ = nullptr;
	ID3D12GraphicsCommandList* commandList_;
	D3D12_VIEWPORT* viewport_;
	D3D12_RECT* scissorRect_;

	Microsoft::WRL::ComPtr<ID3D12Resource> renderTextureResource_ = nullptr;
	uint32_t renderTextureSRVIndex_;
	D3D12_CPU_DESCRIPTOR_HANDLE renderTextureRtvHandle_;

	uint32_t depthTextureSRVIndex_;

	//マテリアル
	Microsoft::WRL::ComPtr<ID3D12Resource> MaterialResource_ = nullptr;
	OutlineMaterial* outlinematerialData_ = nullptr;

	RenderTextureType renderTextureType_ = RenderTextureType::OffscreenRender;

	const uint32_t renderTextureTypeCount_ = 6; // RenderTextureTypeの数

	std::vector<DrawMode> renderTextureDrawModes_;

	// オフスクリーンレンダリングを使用するかどうか
	bool useOffscreenRender_ = false;
};

