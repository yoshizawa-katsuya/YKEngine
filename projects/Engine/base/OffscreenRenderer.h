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

/// <summary>
/// レンダーテクスチャの種類。
/// </summary>
enum class RenderTextureType
{
	// オフスクリーンレンダリング用のテクスチャ
	OffscreenRender,
	GrayScale,
	Vignette,
	BoxFilter,
	GaussianFilter,
	LuminanceOutline,
	Outline,
	RadialBlur,
	Dissolve,
	Random,
};

/// <summary>
/// オフスクリーンレンダラー。
/// オフスクリーンレンダリングを行い、様々なエフェクトを適用する。
/// レンダーテクスチャの種類をRenderTextureTypeで指定する。
/// オフスクリーンレンダリングを使用する場合、sceneのUpdateでSetUseOffscreenRender(true)を呼び出す。
/// </summary>
class OffscreenRenderer
{
public:

	/// <summary>
	/// シングルトンインスタンスの取得。
	/// </summary>
	/// <returns>シングルトンインスタンス</returns>
	static OffscreenRenderer* GetInstance();

	/// <summary>
	/// 初期化。
	/// </summary>
	/// <param name="dxCommon">DirectX共通クラス</param>
	void Initialize(SrvHeapManager* srvHeapManager);

	/// <summary>
	/// 終了処理。
	/// </summary>
	void Finalize();

	/// <summary>
	/// レンダーテクスチャへの描画前処理。
	/// </summary>
	void PreDrawRenderTexture();

	/// <summary>
	/// レンダーテクスチャへの描画後処理。
	/// </summary>
	/// <param name="primitiveDrawer">プリミティブ描画クラス</param>
	/// <param name="srvHeapManager">SRVヒープマネージャー</param>
	void PostDrawRenderTexture(PrimitiveDrawer* primitiveDrawer, SrvHeapManager* srvHeapManager);

	/// <summary>
	/// アウトラインマテリアルデータの更新。
	/// </summary>
	/// <param name="projectionMatrix">プロジェクション行列</param>
	void UpdateOutlineMaterialData(const Matrix4x4& projectionMatrix);

	void SetMaskTexture(uint32_t texturehandle) { maskTextureHandle_ = texturehandle; }

	bool* GetUseOffscreenRenderPtr() { return &useOffscreenRender_; }
	bool GetUseOffscreenRender() { return useOffscreenRender_; }

	/// <summary>
	/// オフスクリーンレンダリングを使用するかどうかを設定。
	/// ScenenのUpdateで呼び出す。
	/// </summary>
	/// <param name="use">使用する場合はtrue、使用しない場合はfalse</param>
	void SetUseOffscreenRender(bool use) { useOffscreenRender_ = use; }

	//レンダーテクスチャの種類を設定
	void SetRenderTextureType(RenderTextureType type) { renderTextureType_ = type; }

private:

	// シングルトンインスタンス
	static OffscreenRenderer* instance_;

	OffscreenRenderer() = default;
	~OffscreenRenderer() = default;
	OffscreenRenderer(OffscreenRenderer&) = delete;
	const OffscreenRenderer& operator=(OffscreenRenderer&) = delete;

	/// <summary>
	/// レンダーテクスチャの作成。
	/// </summary>
	void CreateRenderTexture();

	/// <summary>
	/// レンダーテクスチャのSRV作成。
	/// </summary>
	/// <param name="srvHeapManager">SRVヒープマネージャー</param>
	void CreateRenderTextureSRV(SrvHeapManager* srvHeapManager);

	/// <summary>
	/// レンダーテクスチャリソースの作成。
	/// </summary>
	/// <param name="width">ウィンドウの幅</param>
	/// <param name="height">ウィンドウの高さ</param>
	/// <param name="format">フォーマット</param>
	/// <param name="clearColor">クリアカラー。背景。</param>
	void CreateRenderTextureResource(int32_t width, int32_t height, DXGI_FORMAT format, const Vector4& clearColor);

	/// <summary>
	/// 深度テクスチャのSRV作成。
	/// </summary>
	/// <param name="srvHerpManager">SRVヒープマネージャー</param>
	void CreateDepthTextureSRV(SrvHeapManager* srvHerpManager);

	/// <summary>
	/// アウトラインマテリアル。
	/// </summary>
	/// <param name="projectionInverseMatrix">プロジェクション逆行列</param>
	struct OutlineMaterial
	{
		Matrix4x4 projectionInverseMatrix; // プロジェクション逆行列
	};

	/// <summary>
	/// ランダムマテリアル。
	/// </summary>
	/// <param name="time">乱数生成のための時間</param>
	struct RandomMaterial
	{
		float time;
	};

	DirectXCommon* dxCommon_ = nullptr;
	ID3D12GraphicsCommandList* commandList_;
	D3D12_VIEWPORT* viewport_;
	D3D12_RECT* scissorRect_;

	Microsoft::WRL::ComPtr<ID3D12Resource> renderTextureResource_ = nullptr;
	uint32_t renderTextureSRVIndex_;
	D3D12_CPU_DESCRIPTOR_HANDLE renderTextureRtvHandle_;

	uint32_t depthTextureSRVIndex_;

	//アウトラインマテリアル
	Microsoft::WRL::ComPtr<ID3D12Resource> outlineMaterialResource_ = nullptr;
	OutlineMaterial* outlinematerialData_ = nullptr;

	//ランダムマテリアル
	Microsoft::WRL::ComPtr<ID3D12Resource> randomMaterialResource_ = nullptr;
	RandomMaterial* randomMaterialData_ = nullptr;

	//マスクテクスチャのハンドル
	uint32_t maskTextureHandle_;

	RenderTextureType renderTextureType_ = RenderTextureType::OffscreenRender;

	const uint32_t kRenderTextureTypeCount_ = 10; // RenderTextureTypeの数

	std::vector<DrawMode> renderTextureDrawModes_;

	// オフスクリーンレンダリングを使用するかどうか
	bool useOffscreenRender_ = false;
};

