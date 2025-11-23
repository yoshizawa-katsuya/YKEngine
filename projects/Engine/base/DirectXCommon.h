#pragma once

#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <dxcapi.h>
#include <wrl.h>
#include <string>
#include <array>
#include <chrono>

#include "WinApp.h"
#include "DirectXTex/DirectXTex.h"

/// <summary>
/// DirectX共通機能。
/// DirectX初期化、コマンドリスト、スワップチェイン、描画前後処理など。
/// 他のクラスでDirectXの機能を使用する場合は、ここからデバイスやコマンドリストを取得して使用する。
/// </summary>
class DirectXCommon
{
public:

	/// <summary>
	/// シングルトンインスタンスの取得。
	/// </summary>
	/// <returns>シングルトンインスタンス</returns>
	static DirectXCommon* GetInstance();

	/// <summary>
	/// 終了処理。
	/// </summary>
	void Finalize();

	/// <summary>
	/// 初期化。
	/// </summary>
	/// <param name="winApp">ウィンドウズアプリケーション管理</param>
	void Initialize(WinApp* winApp);

	/// <summary>
	/// 描画前処理。
	/// </summary>
	void PreDraw();

	/// <summary>
	/// 描画後処理。
	/// </summary>
	void PostDraw();
	
	/// <summary>
	/// シェーダーのコンパイラ。
	/// </summary>
	/// <param name="filePath">翻訳するShaderファイルへのパス</param>
	///  <param name="profile">コンパイラに使用するProfile</param>
	Microsoft::WRL::ComPtr<IDxcBlob> CompilerShader(
		//CompilerするShaderファイルへのパス
		const std::wstring& filePath,
		//Compilerに使用するProfile
		const wchar_t* profile);

	/// <summary>
	/// BufferResourceを作る。
	/// </summary>
	/// <param name="sizeInBytes">リソースのサイズ(バイト単位)</param>
	/// <returns>BufferResource</returns>
	Microsoft::WRL::ComPtr<ID3D12Resource> CreateBufferResource(size_t sizeInBytes);

	/// <summary>
	/// TextureResourceを作る。
	/// </summary>
	/// <param name="metadata">テクスチャのメタデータ</param>
	/// <returns>TextureResource</returns>
	Microsoft::WRL::ComPtr<ID3D12Resource> CreateTextureResource(const DirectX::TexMetadata& metadata);

	/// <summary>
	/// デスクリプタヒープの生成。
	/// </summary>
	/// <param name="heapType">ヒープのタイプ</param>
	/// <param name="numDescriptors">デスクリプタ数</param>
	/// <param name="shaderVisiblr">シェーダーから見えるかどうか</param>
	/// <returns>デスクリプタヒープ</returns>
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescriptors, bool shaderVisiblr);


	// デバイスの取得
	ID3D12Device* GetDevice() const { return device_.Get(); }

	//描画コマンドリストの取得
	ID3D12GraphicsCommandList* GetCommandList() const { return commandList_.Get(); }


	IDxcUtils* GetDxcUtils() const { return dxcUtils_.Get(); }

	IDxcCompiler3* GetDxcCompiler() const { return dxcCompiler_.Get(); }

	IDxcIncludeHandler* GetIncludeHandler() const { return includeHandler_.Get(); }

	uint32_t GetBackBufferCount() const { return backBuffers; }

	/// <summary>
	/// RTVの指定番号のCPUデスクリプタハンドルを取得
	/// </summary>
	/// <returns>RTVのCPUデスクリプタハンドル</returns>
	D3D12_CPU_DESCRIPTOR_HANDLE GetRTVCPUDescriptorHandle(uint32_t index);

	/// <summary>
	/// RTVの指定番号のGPUデスクリプタハンドルを取得
	/// </summary>
	/// <returns>RTVのGPUデスクリプタハンドル</returns>
	D3D12_GPU_DESCRIPTOR_HANDLE GetRTVGPUDescriptorHandle(uint32_t index);

	

	/// <summary>
	/// DSVの指定番号のCPUデスクリプタハンドルを取得
	/// </summary>
	/// <returns>DSVのCPUデスクリプタハンドル</returns>
	D3D12_CPU_DESCRIPTOR_HANDLE GetDSVCPUDescriptorHandle(uint32_t index);

	/// <summary>
	/// DSVの指定番号のGPUデスクリプタハンドルを取得
	/// </summary>
	/// <returns>DSVのGPUデスクリプタハンドル</returns>
	D3D12_GPU_DESCRIPTOR_HANDLE GetDSVGPUDescriptorHandle(uint32_t index);

	//前のフレームからの経過時間を取得
	float GetDeltaTime() { return deltaTime_; }

	/// <summary>
	/// 経過時間リセット。
	/// </summary>
	void ResetDeltaTime() { deltaTimeReset_ = true; }

	D3D12_VIEWPORT* GetViewport() { return &viewport_; }

	D3D12_RECT* GetScissorRect() { return &scissorRect_; }

	ID3D12Resource* GetDepthStencilResource() const { return depthStencilResource_.Get(); }

private:
	// シングルトンインスタンス
	static DirectXCommon* instance_;

	DirectXCommon() = default;
	~DirectXCommon() = default;
	DirectXCommon(DirectXCommon&) = delete;
	const DirectXCommon& operator=(DirectXCommon&) = delete;

	/// <summary>
	/// DXGIデバイスの初期化。
	/// </summary>
	void InitializeDXGIDevice();

	/// <summary>
	/// コマンド関連の初期化。
	/// </summary>
	void InitializeCommand();

	/// <summary>
	/// スワップチェインの生成。
	/// </summary>
	void CreateSwapChain();

	/// <summary>
	/// 最終レンダーターゲットの生成。
	/// </summary>
	void CreateFinalRenderTargets();

	/// <summary>
	/// 深度バッファの生成。
	/// </summary>
	void CreateDepthBuffer();

	/// <summary>
	/// デスクリプタヒープの生成。
	/// </summary>
	void CreateDescriptorHeaps();

	/// <summary>
	/// フェンスの生成。
	/// </summary>
	void CreateFence();
	
	/// <summary>
	/// ビューポートの初期化。
	/// </summary>
	void CreateViewport();

	/// <summary>
	/// シザー矩形の初期化。
	/// </summary>
	void CreateScissorRect();

	/// <summary>
	/// DXCコンパイラの初期化。
	/// </summary>
	void CreateDXCCompiler();

	/// <summary>
	/// FPS固定初期化。
	/// </summary>
	void InitializeFixFPS();

	/// <summary>
	/// FPS固定更新。
	/// </summary>
	void UpdateFixFPS();

	/// <summary>
	/// DepthStencilテクスチャリソースの生成。
	/// </summary>
	/// <param name="width">幅</param>
	/// <param name="height">高さ</param>
	/// <returns>DepthStencilテクスチャリソース</returns>
	Microsoft::WRL::ComPtr<ID3D12Resource> CreateDepthStencilTextureResource(int32_t width, int32_t height);


	/// <summary>
	/// DescriptorHandleを取得する関数。CPU
	/// </summary>
	/// <param name="descriptorHeap">デスクリプタヒープ</param>
	/// <param name="descriptorSize">デスクリプタサイズ</param>
	/// <param name="index">インデックス</param>
	/// <returns>CPUデスクリプタハンドル</returns>
	static D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap, uint32_t descriptorSize, uint32_t index);

	/// <summary>
	/// DescriptorHandleを取得する関数。GPU
	/// </summary>
	/// <param name="descriptorHeap">デスクリプタヒープ</param>
	/// <param name="descriptorSize">デスクリプタサイズ</param>
	/// <param name="index">インデックス</param>
	/// <returns>GPUデスクリプタハンドル</returns>
	static D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap, uint32_t descriptorSize, uint32_t index);

	// ウィンドウズアプリケーション管理
	WinApp* winApp_ = nullptr;

	Microsoft::WRL::ComPtr<IDXGIFactory7> dxgiFactory_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Device> device_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue_ = nullptr;
	Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain_ = nullptr;

	static constexpr UINT kBufferCount = 2;

	std::array<Microsoft::WRL::ComPtr<ID3D12Resource>, kBufferCount> swapChainResources_;
	std::array<D3D12_CPU_DESCRIPTOR_HANDLE, kBufferCount> rtvHandles_;

	Microsoft::WRL::ComPtr<ID3D12Resource> depthStencilResource_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvDescriptorHeap_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvDescriptorHeap_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Fence> fence_ = nullptr;
	uint64_t fenceValue_ = 0;
	HANDLE fenceEvent_;
	Microsoft::WRL::ComPtr<IDxcUtils> dxcUtils_ = nullptr;
	Microsoft::WRL::ComPtr<IDxcCompiler3> dxcCompiler_ = nullptr;
	Microsoft::WRL::ComPtr<IDxcIncludeHandler> includeHandler_ = nullptr;

	uint32_t backBuffers = 2;

	uint32_t descriptorSizeRTV_;
	uint32_t descriptorSizeDSV_;

	D3D12_VIEWPORT viewport_{};

	D3D12_RECT scissorRect_{};

	//記録時間(FPS固定用)
	std::chrono::steady_clock::time_point referance_;

	// 秒単位の経過時間
	float deltaTime_ = 0.0f;
	bool deltaTimeReset_ = false;

	// オフスクリーンレンダリングを使用するかどうか
	bool* useOffscreenRender_;
};

