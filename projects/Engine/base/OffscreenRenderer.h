#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <cstdint>
#include "Vector4.h"
class SrvHeapManager;
class PrimitiveDrawer;
class DirectXCommon;

class OffscreenRenderer
{
public:

	//シングルトンインスタンスの取得
	static OffscreenRenderer* GetInstance();

	void Initialize(SrvHeapManager* srvHeapManager);

	void PreDrawRenderTexture();

	void PostDrawRenderTexture(PrimitiveDrawer* primitiveDrawer, SrvHeapManager* srvHeapManager);

private:

	OffscreenRenderer() = default;
	~OffscreenRenderer() = default;
	OffscreenRenderer(OffscreenRenderer&) = delete;
	const OffscreenRenderer& operator=(OffscreenRenderer&) = delete;

	//RenderTexture作成
	void CreateRenderTexture();

	void CreateRenderTextureSRV(SrvHeapManager* srvHeapManager);

	void CreateRenderTextureResource(int32_t width, int32_t height, DXGI_FORMAT format, const Vector4& clearColor);

	DirectXCommon* dxCommon_ = nullptr;
	ID3D12GraphicsCommandList* commandList_;
	D3D12_VIEWPORT* viewport_;
	D3D12_RECT* scissorRect_;

	Microsoft::WRL::ComPtr<ID3D12Resource> renderTextureResource_ = nullptr;
	uint32_t renderTextureSRVIndex_;
	D3D12_CPU_DESCRIPTOR_HANDLE renderTextureRtvHandle_;
};

