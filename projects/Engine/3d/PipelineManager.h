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
#include <unordered_map>

namespace YKEngine
{

class DirectXCommon;

/// <summary>
/// 描画モード。
/// ブレンドモードや、オフスクリーンレンダリングの種類を指定する。
/// </summary>
enum class DrawMode 
{
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

/// <summary>
/// パイプラインステートを管理する。
/// </summary>
class PipelineManager
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

	//ルートパラメータの種類。描画モードごとに異なるルートパラメータを使用するため。
	enum class RootSignatureType
	{
		Model,
		Sprite,
		Particle,
		InstancingModel,
		PostEffect,
		Line,
		Sphere,
		SkinModel,
		Skybox,
		Random,
		Outline,
		Dissolve
	};

	//入力レイアウトの種類。描画モードごとに異なる入力レイアウトを使用するため。
	enum class InputLayoutType
	{
		None,			//入力レイアウトなし
		Default,		//通常の入力レイアウト		
		GeometryShader,	//GSを使用する入力レイアウト
		SkinModel		//スキンモデル用の入力レイアウト
	};

	//ブレンドモードの種類。描画モードごとに異なるブレンドモードを使用するため。
	enum class BlendType
	{
		None,
		Normal,
		Add,
		Subtract,
		Multiply,
		Screen
	};

	enum class RasterizerType
	{
		CullBack,	//背面を表示しない
		CullNone	//すべて表示する
	};

	//深度ステンシルの種類。描画モードごとに異なる深度ステンシルを使用するため。
	enum class DepthType
	{
		EnableWrite,	//深度テストと深度書き込みを有効にする
		EnableNoWrite,	//深度テストを有効にするが、深度書き込みは行わない
		NoEnableWrite,	//深度テストを行わないが、深度書き込みは行う
		NoStencil,		//深度テスト、深度書き込みも行わず、ステンシルも使用しない
		Disable			//深度テストと深度書き込みを無効にする
	};

	//パイプラインの設定。描画モードごとに異なるパイプラインを使用するため。
	struct PipelineConfig
	{
		const std::wstring vsFilePath;
		const std::wstring psFilePath;
		const std::wstring gsFilePath;

		RootSignatureType rootSignature;

		InputLayoutType inputLayout;

		BlendType blend;

		RasterizerType rasterizer;

		DepthType depth;

		D3D12_PRIMITIVE_TOPOLOGY_TYPE topology;
	};

	/// <summary>
	/// グラフィックスパイプラインの生成。
	/// </summary>
	/// <param name= "pipelineConfig">パイプラインの設定</param>
	/// <param name="dxCommon">DirectX共通クラス</param>
	/// <returns>パイプラインセット</returns>
	std::unique_ptr<PipelineSet> CreateGraphicsPipeline(const PipelineConfig& pipelineConfig, DirectXCommon* dxCommon);

	/// <summary>
	/// ルートシグネチャの生成。
	/// </summary>
	/// <param name="device">D3D12デバイス</param>
	/// <param name="type">ルートシグネチャの種類</param>
	/// <returns>ルートシグネチャ</returns>
	Microsoft::WRL::ComPtr<ID3D12RootSignature> CreateRootSignature(ID3D12Device* device, RootSignatureType type);

	/// <summary>
	/// ブレンド設定の生成。
	/// </summary>
	/// <param name="type">ブレンドモードの種類</param>
	/// <returns>ブレンド設定</returns>
	D3D12_BLEND_DESC CreateBlendDesc(BlendType type);

	/// <summary>
	/// 深度設定の生成。
	/// </summary>
	/// <param name="type">深度ステンシルの種類</param>
	/// <returns>深度設定</returns>
	D3D12_DEPTH_STENCIL_DESC CreateDepthDesc(DepthType type);

	/// <summary>
	/// インプットレイアウトの生成。
	/// </summary>
	/// <param name="type">インプットレイアウトの種類</param>
	/// <param name="inputElementDescs">入力レイアウトの要素の説明</param>
	/// <returns>インプットレイアウト</returns>
	D3D12_INPUT_LAYOUT_DESC CreateInputLayout(InputLayoutType type, std::vector<D3D12_INPUT_ELEMENT_DESC>& inputElementDescs);

	/// <summary>
	/// ラスタライザの生成。
	/// </summary>
	/// <param name="type">ラスタライザの種類</param>
	/// <returns>ラスタライザ</returns>
	D3D12_RASTERIZER_DESC CreateRasterizerDesc(RasterizerType type);

	/// <summary>
	/// デスクリプタレンジの生成。
	/// </summary>
	D3D12_DESCRIPTOR_RANGE CreateDescriptorRange(uint32_t BaseShaderRegister);

	//パイプライン。ブレンドモードの数だけ用意する
	std::array<std::unique_ptr<PipelineSet>, (uint16_t)DrawMode::kCount> pipelineSets_;

	//描画モードとパイプラインの設定の対応表。
	static const std::unordered_map<DrawMode, PipelineConfig> pipelineTable_;

};

} // namespace YKEngine