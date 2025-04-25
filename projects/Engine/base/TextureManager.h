#pragma once

#include <unordered_map>
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

	//描画後処理
	void PostDraw();

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
	void LoadTexture(const std::string& filePath, uint32_t index);

	//データを転送
	Microsoft::WRL::ComPtr<ID3D12Resource> UploadTextureData(ID3D12Resource* textureResource, const DirectX::ScratchImage& mipImages);

	TextureManager() = default;
	~TextureManager() = default;
	TextureManager(TextureManager&) = default;
	TextureManager& operator=(TextureManager&) = default;

	DirectXCommon* dxCommon_ = nullptr;
	SrvHeapManager* srvHeapManager_ = nullptr;
	

	/// <summary>
	/// テクスチャ
	/// </summary>
	struct Texture {
		DirectX::TexMetadata metadata;
		// テクスチャリソース
		Microsoft::WRL::ComPtr<ID3D12Resource> resource;
		// シェーダリソースビューのハンドル(CPU)
		D3D12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
		// シェーダリソースビューのハンドル(CPU)
		D3D12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;
	};

	// テクスチャコンテナ
	//std::array<Texture, SrvHeapManager::kMaxSrvDescriptors_> textures_;
	std::unordered_map<uint32_t, Texture> textures_;
	std::unordered_map<std::string, uint32_t> textureHandles_;

	std::list<Microsoft::WRL::ComPtr<ID3D12Resource>> intermediateResources_;

	std::mutex mutex_;
};