#pragma once
#include "DirectXCommon.h"

//SRV管理
class SrvHeapManager
{
public:

	//初期化
	void Initialize(DirectXCommon* dxCommon);

	//描画前処理
	void PreDraw();

	// デスクリプタテーブルをセット
	void SetGraphicsRootDescriptorTable(UINT RootParameterIndex, uint32_t srvIndex);

	//確保関数
	uint32_t Allocate();

	//確保可能チェック
	bool Check();

	//SRV作成（テクスチャ用）
	void CreateSRVforTexture2D(uint32_t srvIndex, ID3D12Resource* pResource, DXGI_FORMAT Format, UINT MipLevels);

	//SRV生成（Structured Buffer用）
	void CreateSRVforStructuredBuffer(uint32_t srvIndex, ID3D12Resource* pResource, UINT numElements, UINT structureByteStride);

	ID3D12DescriptorHeap* GetDescriptorHeap() { return descriptorHeap_.Get(); }

	uint32_t GetDescriptorSize() { return descriptorSize_; }

	/// <summary>
	/// SRVの指定番号のCPUデスクリプタハンドルを取得
	/// </summary>
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(uint32_t index);

	/// <summary>
	/// SRVの指定番号のGPUデスクリプタハンドルを取得
	/// </summary>
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
	uint32_t useIndex = 0;

};