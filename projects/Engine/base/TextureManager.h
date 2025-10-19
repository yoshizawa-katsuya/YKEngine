#pragma once

#include <unordered_map>
#include <d3d12.h>
#include <string>
#include <wrl.h>
#include "DirectXCommon.h"
#include "DirectXTex/DirectXTex.h"
#include "SrvHeapManager.h"

/// <summary>
/// テクスチャ管理クラス。
/// シングルトン。
/// 複数のスレッドから同時にLoad関数が呼ばれる可能性があるため、
/// ミューテックスで排他制御を行う。
/// テクスチャはuint32_tのハンドルで管理する。
/// </summary>
class TextureManager
{
public:

	/// <summary>
	/// シングルトンインスタンスの取得。
	/// </summary>
	/// <returns>シングルトンインスタンス</returns>
	static TextureManager* GetInstance();

	/// <summary>
	/// 終了処理。
	/// </summary>
	void Finalize();

	/// <summary>
	/// 初期化。
	/// </summary>
	/// <param name="dxCommon">DirectX共通クラス</param>
	/// <param name="srvHeapManager">SRVヒープマネージャー</param>
	void Initialize(DirectXCommon* dxCommon, SrvHeapManager* srvHeapManager);

	/// <summary>
	/// 描画後処理。
	/// </summary>
	void PostDraw();

	/// <summary>
	/// テクスチャ読み込み。
	/// 過去に読み込んだことがある場合は、再度読み込まない。
	/// </summary>
	/// <param name="fileName">画像ファイル名</param>
	/// <returns>テクスチャハンドル</returns>
	uint32_t Load(const std::string& fileName);

	
	/// <summary>
	/// グラフィックスルートディスクリプタテーブルを設定。
	/// </summary>
	/// <param name="textureHandle">テクスチャハンドル</param>
	void SetGraphicsRootDescriptorTable(uint32_t textureHandle);

	/// <summary>
	/// 環境マップを設定。
	/// </summary>
	/// <param name="textureHandle">テクスチャハンドル</param>
	void SetEnvironmentMap(uint32_t textureHandle);

	/// <summary>
	/// テクスチャのメタデータを取得。
	/// </summary>
	/// <param name="textureHandle">テクスチャハンドル</param>
	/// <returns>メタデータ</returns>
	const DirectX::TexMetadata& GetMetaData(uint32_t textureHandle);

	/// <summary>
	/// デスクリプタヒープの生成。
	/// </summary>
	/// <param name="device">dx12デバイス</param>
	/// <param name="heapType">ヒープタイプ</param>
	/// <param name="numDescriptors">デスクリプタ数</param>
	/// <param name="shaderVisiblr">シェーダから見えるか</param>
	/// <returns>デスクリプタヒープ</returns>
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> CreateDescriptorHeap(
		Microsoft::WRL::ComPtr<ID3D12Device> device, D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescriptors, bool shaderVisiblr);

private:

	/// <summary>
	/// テクスチャ読み込み。
	/// </summary>
	/// <param name="filePath">画像ファイルパス</param>
	/// <param name="index">テクスチャインデックス</param>
	void LoadTexture(const std::string& filePath, uint32_t index);

	/// <summary>
	/// テクスチャデータアップロード用バッファリソースの作成。
	/// </summary>
	/// <param name="textureResource">テクスチャリソース</param>
	/// <param name="mipImages">ミップイメージ群</param>
	/// <returns>アップロード用バッファリソース</returns>
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
	/// <param name="metadata">メタデータ</param>
	/// <param name="resource">テクスチャリソース</param>
	/// <param name="cpuDescHandleSRV">シェーダリソースビューのハンドル(CPU)</param>
	/// <param name="gpuDescHandleSRV">シェーダリソースビューのハンドル(GPU)</param>
	struct Texture 
	{
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