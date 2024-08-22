#pragma once

#include <array>
#include <d3d12.h>
#include <string>
#include <wrl.h>
#include "DirectXCommon.h"
#include "DirectXTex/DirectXTex.h"
#include "SrvHeapManager.h"

class TextureManager
{
public:

	//シングルトンインスタンスの取得
	static TextureManager* GetInstance();

	//終了
	void Finalize();

	//初期化
	void Initialize(DirectXCommon* dxCommon, SrvHeapManager* srvHeapManager);

	
	//読み込み
	uint32_t Load(const std::string& fileName);

	
	// デスクリプタテーブルをセット
	void SetGraphicsRootDescriptorTable(uint32_t textureHandle);

	//メタデータを取得
	const DirectX::TexMetadata& GetMetaData(uint32_t textureHandle);

	//DiscriptorHeap作成の関数
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> CreateDescriptorHeap(
		Microsoft::WRL::ComPtr<ID3D12Device> device, D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescriptors, bool shaderVisiblr);

private:

	//Textureデータを読む
	void LoadTexture(const std::string& filePath);

	static TextureManager* instance_;

	TextureManager() = default;
	~TextureManager() = default;
	TextureManager(TextureManager&) = default;
	TextureManager& operator=(TextureManager&) = default;

	DirectXCommon* dxCommon_ = nullptr;
	SrvHeapManager* srvHeapManager_ = nullptr;

	uint32_t index_ = 0;

	

	/// <summary>
	/// テクスチャ
	/// </summary>
	struct Texture {
		std::string filePath;
		DirectX::TexMetadata metadata;
		// テクスチャリソース
		Microsoft::WRL::ComPtr<ID3D12Resource> resource;
		// シェーダリソースビューのハンドル(CPU)
		D3D12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
		// シェーダリソースビューのハンドル(CPU)
		D3D12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;
	};

	// テクスチャコンテナ
	std::array<Texture, SrvHeapManager::kMaxSrvDescriptors_> textures_;

};