#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <string>
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

	kInstancingTriplanar,	//インスタンシングでトリプラナーを使用するモード

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
	enum class RootParameterType
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

	//スタティックサンプラーの種類。描画モードごとに異なるスタティックサンプラーを使用するため。
	enum class StaticSamplerType
	{
		Default,	//通常のサンプラー
		Outline,	//アウトライン用のサンプラー
		GeometryShader,	//GS用のサンプラー
	};

	/// <summary>
	/// スタティックサンプラーの設定。
	/// </summary>
	/// <param name="filter">フィルタ</param>
	/// <param name="reg">レジスタ番号 s0など</param>
	struct StaticSamplerConfig
	{
		D3D12_FILTER filter;	//フィルタ
		UINT reg;				//レジスタ番号 s0など
	};

	//入力レイアウトの種類。描画モードごとに異なる入力レイアウトを使用するため。
	enum class InputLayoutType
	{
		None,			//入力レイアウトなし
		Default,		//通常の入力レイアウト		
		GeometryShader,	//GSを使用する入力レイアウト
		SkinModel		//スキンモデル用の入力レイアウト
	};

	/// <summary>
	/// 入力レイアウトの設定。
	/// </summary>
	/// <param name="inputElementDescs">入力レイアウトの要素の説明</param>
	/// <param name="numElements">入力レイアウトの要素数</param>
	struct InputLayoutConfig
	{
		std::vector<D3D12_INPUT_ELEMENT_DESC> inputElementDescs;
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

	/// <summary>
	/// ブレンド設定。
	/// </summary>
	/// <param name="enable">ブレンドを有効にするかどうか</param>
	/// <param name="srcBlend">ソースブレンド</param>
	/// <param name="destBlend">デスティネーションブレンド</param>
	/// <param name="blendOp">ブレンドオペレーション</param>
	struct BlendConfig
	{
		BOOL enable;
		D3D12_BLEND srcBlend;
		D3D12_BLEND destBlend;
		D3D12_BLEND_OP blendOp;
	};

	enum class RasterizerType
	{
		CullBack,	//背面を表示しない
		CullNone	//すべて表示する
	};

	/// <summary>
	/// ラスタライザ設定。
	/// </summary>
	/// <param name="cullMode">カリングモード</param>
	struct RasterizerConfig
	{
		D3D12_CULL_MODE cullMode;
	};

	//深度ステンシルの種類。描画モードごとに異なる深度ステンシルを使用するため。
	enum class DepthType
	{
		EnableWrite,	//深度テストと深度書き込みを有効にする
		EnableNoWrite,	//深度テストを有効にするが、深度書き込みは行わない
		NoEnableWrite,	//深度テストを行わないが、深度書き込みは行う
		Disable			//深度テストと深度書き込みを無効にする
	};

	/// <summary>
	/// 深度設定。
	/// </summary>
	/// <param name="depthEnable">深度テストを有効にするかどうか</param>
	/// <param name="depthWriteMask">深度書き込みのマスク</param>
	struct DepthConfig
	{
		BOOL depthEnable;
		D3D12_DEPTH_WRITE_MASK depthWriteMask;
	};

	//パイプラインの設定。描画モードごとに異なるパイプラインを使用するため。
	struct PipelineConfig
	{
		const std::wstring vsFilePath;
		const std::wstring psFilePath;
		const std::wstring gsFilePath;

		RootParameterType rootParameter;
		StaticSamplerType staticSampler;

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
	Microsoft::WRL::ComPtr<ID3D12RootSignature> CreateRootSignature(ID3D12Device* device, RootParameterType paramType, StaticSamplerType staticSamplerType);

	/// <summary>
	/// スタティックサンプラーの生成。
	/// </summary>
	/// <param name="config">スタティックサンプラーの設定</param>
	/// <returns>スタティックサンプラーの説明</returns>
	D3D12_STATIC_SAMPLER_DESC CreateStaticSamplerDesc(StaticSamplerConfig config);

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

	/// <summary>
	/// モデル用のルートパラメータの生成。
	/// </summary>
	/// <param name="rootParameters">ルートパラメータ</param>
	void BuildModelRootParameters(std::vector<D3D12_ROOT_PARAMETER>& rootParameters);

	/// <summary>
	/// スプライト用のルートパラメータの生成。
	/// </summary>
	/// <param name="rootParameters">ルートパラメータ</param>
	void BuildSpriteRootParameters(std::vector<D3D12_ROOT_PARAMETER>& rootParameters);

	/// <summary>
	/// パーティクル用のルートパラメータの生成。
	/// </summary>
	/// <param name="rootParameters">ルートパラメータ</param>
	void BuildParticleRootParameters(std::vector<D3D12_ROOT_PARAMETER>& rootParameters);

	/// <summary>
	/// インスタンシングモデル用のルートパラメータの生成。
	/// </summary>
	/// <param name="rootParameters">ルートパラメータ</param>
	void BuildInstancingModelRootParameters(std::vector<D3D12_ROOT_PARAMETER>& rootParameters);

	/// <summary>
	/// ポストエフェクト用のルートパラメータの生成。
	/// </summary>
	/// <param name="rootParameters">ルートパラメータ</param>
	void BuildPostEffectRootParameters(std::vector<D3D12_ROOT_PARAMETER>& rootParameters);

	/// <summary>
	/// 線用のルートパラメータの生成。
	/// </summary>
	/// <param name="rootParameters">ルートパラメータ</param>
	void BuildLineRootParameters(std::vector<D3D12_ROOT_PARAMETER>& rootParameters);

	/// <summary>
	/// 球用のルートパラメータの生成。
	/// </summary>
	/// <param name="rootParameters">ルートパラメータ</param>
	void BuildSphereRootParameters(std::vector<D3D12_ROOT_PARAMETER>& rootParameters);

	/// <summary>
	/// スキンモデル用のルートパラメータの生成。
	/// </summary>
	/// <param name="rootParameters">ルートパラメータ</param>
	void BuildSkinModelRootParameters(std::vector<D3D12_ROOT_PARAMETER>& rootParameters);

	/// <summary>
	/// スカイボックス用のルートパラメータの生成。
	/// </summary>
	/// <param name="rootParameters">ルートパラメータ</param>
	void BuildSkyboxRootParameters(std::vector<D3D12_ROOT_PARAMETER>& rootParameters);

	/// <summary>
	/// ランダム用のルートパラメータの生成。
	/// </summary>
	/// <param name="rootParameters">ルートパラメータ</param>
	void BuildRandomRootParameters(std::vector<D3D12_ROOT_PARAMETER>& rootParameters);

	/// <summary>
	/// アウトライン用のルートパラメータの生成。
	/// </summary>
	/// <param name="rootParameters">ルートパラメータ</param>
	void BuildOutlineRootParameters(std::vector<D3D12_ROOT_PARAMETER>& rootParameters);

	/// <summary>
	/// ディゾルブ用のルートパラメータの生成。
	/// </summary>
	/// <param name="rootParameters">ルートパラメータ</param>
	void BuildDissolveRootParameters(std::vector<D3D12_ROOT_PARAMETER>& rootParameters);

	/// <summary>
	/// ルートパラメータのビルド関数のマップを取得する。
	/// </summary>
	/// <returns>ルートパラメータのビルド関数のマップ</returns>
	const std::unordered_map<RootParameterType, void(PipelineManager::*)(std::vector<D3D12_ROOT_PARAMETER>&)>& GetRootParameterBuilders() const;

	//デスクリプタレンジ。ルートシグネチャのビルド関数で使用するため、クラスのメンバ変数として保持する。
	D3D12_DESCRIPTOR_RANGE descriptorRange_;

	//平行光源用のデスクリプタレンジ。モデル用のルートシグネチャで使用するため、クラスのメンバ変数として保持する。
	D3D12_DESCRIPTOR_RANGE descriptorRangeDirectionalLight_;

	//点光源用のデスクリプタレンジ。モデル用のルートシグネチャで使用するため、クラスのメンバ変数として保持する。
	D3D12_DESCRIPTOR_RANGE descriptorRangePointLight_;

	//スポットライト用のデスクリプタレンジ。モデル用のルートシグネチャで使用するため、クラスのメンバ変数として保持する。
	D3D12_DESCRIPTOR_RANGE descriptorRangeSpotLight_;

	//環境マップ用のデスクリプタレンジ。モデル用とスカイボックス用のルートシグネチャで使用するため、クラスのメンバ変数として保持する。
	D3D12_DESCRIPTOR_RANGE descriptorRangeEnvironmentTexture_;

	//深度テクスチャ用のデスクリプタレンジ。アウトライン用のルートシグネチャで使用するため、クラスのメンバ変数として保持する。
	D3D12_DESCRIPTOR_RANGE descriptorRangeDepthTexture_;

	//マスクテクスチャ用のデスクリプタレンジ。ディゾルブ用のルートシグネチャで使用するため、クラスのメンバ変数として保持する。
	D3D12_DESCRIPTOR_RANGE descriptorMaskTexture_;

	//デスクリプタレンジの生成。
	void CreateDescriptorRanges();

	//パイプライン。ブレンドモードの数だけ用意する
	std::array<std::unique_ptr<PipelineSet>, (uint16_t)DrawMode::kCount> pipelineSets_;

	//スタティックサンプラーの種類とスタティックサンプラーの設定の対応表。
	static const std::unordered_map<StaticSamplerType, std::vector<StaticSamplerConfig>> staticSamplerTable_;

	//ブレンドモードとブレンド設定の対応表。
	static const std::unordered_map<BlendType, BlendConfig> blendTable_;

	//深度ステンシルの種類と深度設定の対応表。
	static const std::unordered_map<DepthType, DepthConfig> depthTable_;

	//ラスタライザの種類とラスタライザ設定の対応表。
	static const std::unordered_map<RasterizerType, RasterizerConfig> rasterizerTable_;

	//入力レイアウトの種類と入力レイアウトの設定の対応表。
	static const std::unordered_map<InputLayoutType, InputLayoutConfig> inputLayoutTable_;

	//描画モードとパイプラインの設定の対応表。
	static const std::unordered_map<DrawMode, PipelineConfig> pipelineTable_;

};

} // namespace YKEngine