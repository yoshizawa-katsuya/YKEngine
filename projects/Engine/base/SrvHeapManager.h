#pragma once
#include "DirectXCommon.h"
#include <mutex>
#include <queue>

namespace YKEngine
{

/// <summary>
/// SRVヒープ管理クラス。
/// SRVヒープの生成、SRVの生成、SRVヒープの管理を行う。
/// SRVヒープは1つのみ生成し、SRVは最大512個まで生成可能。
/// </summary>
class SrvHeapManager
{
public:

	/// <summary>
	/// デストラクタ。
	/// </summary>
	~SrvHeapManager();

	/// <summary>
	/// 初期化。
	/// </summary>
	/// <param name="dxCommon">DirectXCommonのポインタ</param>
	void Initialize(DirectXCommon* dxCommon);

	/// <summary>
	/// 描画前処理。
	/// </summary>
	void PreDraw();

	/// <summary>
	/// グラフィックスコマンドリストにSRVヒープをセット。
	/// </summary>
	/// <param name="RootParameterIndex">ルートパラメータのインデックス</param>
	/// <param name="srvIndex">SRVのインデックス</param>
	void SetGraphicsRootDescriptorTable(UINT RootParameterIndex, uint32_t srvIndex);

	/// <summary>
	/// SRVの割り当て。
	/// </summary>
	/// <returns>割り当てたSRVのインデックス</returns>
	uint32_t Allocate();

	/// <summary>
	/// srvIndexの解放処理。
	/// </summary>
	/// <param name="srvIndex">解放したいsrvIndex</param>
	void Free(uint32_t srvIndex);

	/// <summary>
	/// SRVヒープの空き確認。
	/// </summary>
	/// <returns>空きがあればtrue、なければfalse</returns>
	bool Check();

	/// <summary>
	/// SRV生成（Texture2D用）
	/// </summary>
	/// <param name="srvIndex">SRVのインデックス</param>
	/// <param name="pResource">SRVを作成するリソース</param>
	/// <param name="metadata">テクスチャのメタデータ</param>
	void CreateSRVforTexture2D(uint32_t srvIndex, ID3D12Resource* pResource, DirectX::TexMetadata& metadata);

	/// <summary>
	/// SRV生成（StructuredBuffer用）
	/// </summary>
	/// <param name="srvIndex">SRVのインデックス</param>
	/// <param name="pResource">SRVを作成するリソース</param>
	/// <param name="numElements">要素数</param>
	/// <param name="structureByteStride">構造体のバイトサイズ</param>
	void CreateSRVforStructuredBuffer(uint32_t srvIndex, ID3D12Resource* pResource, UINT numElements, UINT structureByteStride);

	/// <summary>
	/// SRV生成（RenderTexture用）
	/// </summary>
	/// <param name="srvIndex">SRVのインデックス</param>
	/// <param name="pResource">SRVを作成するリソース</param>
	void CreateSRVforRenderTexture(uint32_t srvIndex, ID3D12Resource* pResource);

	/// <summary>
	/// SRV生成（DepthTexture用）
	/// </summary>
	/// <param name="srvIndex">SRVのインデックス</param>
	/// <param name="pResource">SRVを作成するリソース</param>
	void CreateSRVforDepthTexture(uint32_t srvIndex, ID3D12Resource* pResource);

	ID3D12DescriptorHeap* GetDescriptorHeap() { return descriptorHeap_.Get(); }

	uint32_t GetDescriptorSize() { return descriptorSize_; }

	/// <summary>
	/// SRVの指定番号のCPUデスクリプタハンドルを取得
	/// </summary>
	/// <param name="index">SRVのインデックス</param>
	/// <returns>CPUデスクリプタハンドル</returns>
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(uint32_t index);

	/// <summary>
	/// SRVの指定番号のGPUデスクリプタハンドルを取得
	/// </summary>
	/// <param name="index">SRVのインデックス</param>
	/// <returns>GPUデスクリプタハンドル</returns>
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(uint32_t index);

	// 最大SRV数
	static const uint32_t kMaxSrvDescriptors_ = 512;

private:

	DirectXCommon* dxCommon_ = nullptr;


	//SRV用のデスクリプタ1個分のサイズ
	uint32_t descriptorSize_;
	//SRV用デスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap_;

	//次に使用するSRVインデックス
	uint32_t useIndex_ = 0;

	//SRVの空きリスト
	std::queue<uint32_t> freeList_;

	std::mutex mutex_;
};

} // namespace YKEngine