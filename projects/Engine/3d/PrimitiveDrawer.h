#pragma once
#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <dxcapi.h>
#include <wrl.h>
#include <string>
#include <cstdlib>
#include <array>
#include <memory>
class DirectXCommon;

/// <summary>
/// 描画モード。
/// ブレンドモードや、オフスクリーンレンダリングの種類を指定する。
/// </summary>
enum class DrawMode {
	kBlendModeNone, //ブレンドなし
	kBlendModeNormal, //kNormalBlend
	kBlendModeAdd,	//加算
	kBlendModeSubtract,	//減算
	kBlendModeMultiply,	//乗算
	kBlendModeScreen,	//スクリーン

	kBlendModeNoneSprite,
	kBlendModeNormalSprite,
	kBackGroundSprite,

	kBlendModeAddParticle,
	kBlendModeNormalParticle,

	kBlendModeAddBackDrawParticle,	//背面描画用加算
	kBlendModeNormalBackDrawParticle,	//背面描画用通常

	kLineMode,

	kSphereMode,

	kSkinModelMode,

	kBlendModeNormalinstancing,

	//オフスクリーンレンダリング
	kOffScreenRendering,
	kGrayScaleRendering,
	kVignetteRendering,
	kBoxFilterRendering,
	kGaussianFilterRendering,
	kLuminanceOutlineRendering,
	kOutlineRendering,
	kRadialBlurRendering,
	kDissolveRendering,
	kRandomRendering,

	kSkyboxMode,

	kCount,	//利用してはいけない
};

//TODO:クラス名を変更する
/// <summary>
/// プリミティブ描画クラス。
/// パイプラインステートを管理する。
/// </summary>
class PrimitiveDrawer
{
public:

	/// <summary>
	/// パイプラインセット
	/// </summary>
	/// <param name="rootSignature">ルートシグネチャ</param>
	/// <param name="graphicsPipelineState">グラフィックスパイプラインステート</param>
	struct PipelineSet
	{
		Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
		Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState;
	};

	/// <summary>
	/// 初期化。
	/// </summary>
	/// <param name="dxCommon">DirectX共通クラス</param>
	void Initialize(DirectXCommon* dxCommon);

	/// <summary>
	/// コマンドリストにパイプラインをセットする。
	/// </summary>
	/// <param name="commandList">コマンドリスト</param>
	/// <param name="blendMode">描画モード</param>
	void SetPipelineSet(ID3D12GraphicsCommandList* commandList, DrawMode blendMode);

private:

	/// <summary>
	/// グラフィックスパイプラインの生成。
	/// </summary>
	/// <param name="blendMode">描画モード</param>
	/// <param name="dxCommon">DirectX共通クラス</param>
	/// <returns>パイプラインセット</returns>
	std::unique_ptr<PipelineSet> CreateGraphicsPipeline(DrawMode blendMode, DirectXCommon* dxCommon);

	//パイプライン。ブレンドモードの数だけ用意する
	std::array<std::unique_ptr<PipelineSet>, (uint16_t)DrawMode::kCount> pipelineSets_;

};

