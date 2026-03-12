#include "dx12.h"
#include "PipelineManager.h"
#include <cassert>
#include <format>
#include "DirectXCommon.h"
#include "RootParams.h"

using namespace YKEngine;

void PipelineManager::Initialize(DirectXCommon* dxCommon)
{
	//DrawMode分のパイプラインを作成
	pipelineSets_.at(static_cast<uint16_t>(DrawMode::kBlendModeNone)) = CreateGraphicsPipeline(DrawMode::kBlendModeNone, dxCommon);

	pipelineSets_.at(static_cast<uint16_t>(DrawMode::kBlendModeNormal)) = CreateGraphicsPipeline(DrawMode::kBlendModeNormal, dxCommon);

	pipelineSets_.at(static_cast<uint16_t>(DrawMode::kBlendModeAdd)) = CreateGraphicsPipeline(DrawMode::kBlendModeAdd, dxCommon);

	pipelineSets_.at(static_cast<uint16_t>(DrawMode::kBlendModeSubtract)) = CreateGraphicsPipeline(DrawMode::kBlendModeSubtract, dxCommon);
	
	pipelineSets_.at(static_cast<uint16_t>(DrawMode::kBlendModeMultiply)) = CreateGraphicsPipeline(DrawMode::kBlendModeMultiply, dxCommon);

	pipelineSets_.at(static_cast<uint16_t>(DrawMode::kBlendModeScreen)) = CreateGraphicsPipeline(DrawMode::kBlendModeScreen, dxCommon);

	pipelineSets_.at(static_cast<uint16_t>(DrawMode::kBlendModeNoneSprite)) = CreateGraphicsPipeline(DrawMode::kBlendModeNoneSprite, dxCommon);

	pipelineSets_.at(static_cast<uint16_t>(DrawMode::kBlendModeNormalSprite)) = CreateGraphicsPipeline(DrawMode::kBlendModeNormalSprite, dxCommon);

	pipelineSets_.at(static_cast<uint16_t>(DrawMode::kBackGroundSprite)) = CreateGraphicsPipeline(DrawMode::kBackGroundSprite, dxCommon);

	pipelineSets_.at(static_cast<uint16_t>(DrawMode::kBlendModeAddParticle)) = CreateGraphicsPipeline(DrawMode::kBlendModeAddParticle, dxCommon);

	pipelineSets_.at(static_cast<uint16_t>(DrawMode::kBlendModeNormalParticle)) = CreateGraphicsPipeline(DrawMode::kBlendModeNormalParticle, dxCommon);
	
	pipelineSets_.at(static_cast<uint16_t>(DrawMode::kBlendModeAddBackDrawParticle)) = CreateGraphicsPipeline(DrawMode::kBlendModeAddBackDrawParticle, dxCommon);

	pipelineSets_.at(static_cast<uint16_t>(DrawMode::kBlendModeNormalBackDrawParticle)) = CreateGraphicsPipeline(DrawMode::kBlendModeNormalBackDrawParticle, dxCommon);

	pipelineSets_.at(static_cast<uint16_t>(DrawMode::kLineMode)) = CreateGraphicsPipeline(DrawMode::kLineMode, dxCommon);

	pipelineSets_.at(static_cast<uint16_t>(DrawMode::kSphereMode)) = CreateGraphicsPipeline(DrawMode::kSphereMode, dxCommon);

	pipelineSets_.at(static_cast<uint16_t>(DrawMode::kSkinModelMode)) = CreateGraphicsPipeline(DrawMode::kSkinModelMode, dxCommon);

	pipelineSets_.at(static_cast<uint16_t>(DrawMode::kBlendModeNormalinstancing)) = CreateGraphicsPipeline(DrawMode::kBlendModeNormalinstancing, dxCommon);

	pipelineSets_.at(static_cast<uint16_t>(DrawMode::kSkyboxMode)) = CreateGraphicsPipeline(DrawMode::kSkyboxMode, dxCommon);

	pipelineSets_.at(static_cast<uint16_t>(DrawMode::kOffScreenRendering)) = CreateGraphicsPipeline(DrawMode::kOffScreenRendering, dxCommon);

	pipelineSets_.at(static_cast<uint16_t>(DrawMode::kGrayScaleRendering)) = CreateGraphicsPipeline(DrawMode::kGrayScaleRendering, dxCommon);

	pipelineSets_.at(static_cast<uint16_t>(DrawMode::kVignetteRendering)) = CreateGraphicsPipeline(DrawMode::kVignetteRendering, dxCommon);

	pipelineSets_.at(static_cast<uint16_t>(DrawMode::kBoxFilterRendering)) = CreateGraphicsPipeline(DrawMode::kBoxFilterRendering, dxCommon);

	pipelineSets_.at(static_cast<uint16_t>(DrawMode::kGaussianFilterRendering)) = CreateGraphicsPipeline(DrawMode::kGaussianFilterRendering, dxCommon);

	pipelineSets_.at(static_cast<uint16_t>(DrawMode::kLuminanceOutlineRendering)) = CreateGraphicsPipeline(DrawMode::kLuminanceOutlineRendering, dxCommon);

	pipelineSets_.at(static_cast<uint16_t>(DrawMode::kOutlineRendering)) = CreateGraphicsPipeline(DrawMode::kOutlineRendering, dxCommon);

	pipelineSets_.at(static_cast<uint16_t>(DrawMode::kRadialBlurRendering)) = CreateGraphicsPipeline(DrawMode::kRadialBlurRendering, dxCommon);

	pipelineSets_.at(static_cast<uint16_t>(DrawMode::kDissolveRendering)) = CreateGraphicsPipeline(DrawMode::kDissolveRendering, dxCommon);

	pipelineSets_.at(static_cast<uint16_t>(DrawMode::kRandomRendering)) = CreateGraphicsPipeline(DrawMode::kRandomRendering, dxCommon);
}

std::unique_ptr<PipelineManager::PipelineSet> PipelineManager::CreateGraphicsPipeline(DrawMode blendMode, DirectXCommon* dxCommon) {

	std::unique_ptr<PipelineSet> pipelineSet = std::make_unique<PipelineSet>();

	

	

	//RootSignature作成
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	descriptionRootSignature.Flags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	//DescriptorRange
	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	descriptorRange[0] = CreateDescriptorRange(0);	//t0
	
	D3D12_DESCRIPTOR_RANGE descriptorRangeDirectionalLight[1] = {};
	descriptorRangeDirectionalLight[0] = CreateDescriptorRange(1);	//t1

	D3D12_DESCRIPTOR_RANGE descriptorRangePointLight[1] = {};
	descriptorRangePointLight[0] = CreateDescriptorRange(2);	//t2
	
	D3D12_DESCRIPTOR_RANGE descriptorRangeSpotLight[1] = {};
	descriptorRangeSpotLight[0] = CreateDescriptorRange(3);	//t3
	
	D3D12_DESCRIPTOR_RANGE descriptorRangeEnvironmentTexture[1] = {};
	descriptorRangeEnvironmentTexture[0] = CreateDescriptorRange(4);	//t4
	
	//RootParameter作成。複数設定できるので配列。
	std::vector<D3D12_ROOT_PARAMETER> rootParameters = {};

	switch (blendMode) {
	case DrawMode::kOffScreenRendering:
	case DrawMode::kGrayScaleRendering:
	case DrawMode::kVignetteRendering:
	case DrawMode::kBoxFilterRendering:
	case DrawMode::kGaussianFilterRendering:
	case DrawMode::kLuminanceOutlineRendering:
	case DrawMode::kRadialBlurRendering:
	{
		rootParameters.resize(static_cast<size_t>(PostEffectRootParam::kCount));

		//テクスチャ
		D3D12_ROOT_PARAMETER& textureParam = rootParameters[static_cast<size_t>(PostEffectRootParam::kTexture)];
		textureParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//DescriptorTableを使う
		textureParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	//PixelShaderで使う
		textureParam.DescriptorTable.pDescriptorRanges = descriptorRange;	//Tableの中身の配列を指定
		textureParam.DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);	//Tableで利用する数


		break;
	}
	case DrawMode::kRandomRendering:
	{
		rootParameters.resize(static_cast<size_t>(RandomRootParam::kCount));

		//テクスチャ
		D3D12_ROOT_PARAMETER& textureParam = rootParameters[static_cast<size_t>(RandomRootParam::kTexture)];
		textureParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//DescriptorTableを使う
		textureParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	//PixelShaderで使う
		textureParam.DescriptorTable.pDescriptorRanges = descriptorRange;	//Tableの中身の配列を指定
		textureParam.DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);	//Tableで利用する数

		//マテリアル
		D3D12_ROOT_PARAMETER& materialParam = rootParameters[static_cast<size_t>(RandomRootParam::kMaterial)];
		materialParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//CBVを使う
		materialParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	//PixelShaderで使う
		materialParam.Descriptor.ShaderRegister = 0;	//レジスタ番号0とバインド

		break;
	}
	case DrawMode::kOutlineRendering:
	{

		D3D12_DESCRIPTOR_RANGE descriptorRangeDepthTexture[1] = {};
		descriptorRangeDepthTexture[0] = CreateDescriptorRange(1);	//t1

		rootParameters.resize(static_cast<size_t>(DepthOutlineRootParam::kCount));

		//テクスチャ
		D3D12_ROOT_PARAMETER& textureParam = rootParameters[static_cast<size_t>(DepthOutlineRootParam::kTexture)];
		textureParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//DescriptorTableを使う
		textureParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	//PixelShaderで使う
		textureParam.DescriptorTable.pDescriptorRanges = descriptorRange;	//Tableの中身の配列を指定
		textureParam.DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);	//Tableで利用する数

		//DepthTexture
		D3D12_ROOT_PARAMETER& depthTextureParam = rootParameters[static_cast<size_t>(DepthOutlineRootParam::kDepthTexture)];
		depthTextureParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//DescriptorTableを使う
		depthTextureParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	//PixelShaderで使う
		depthTextureParam.DescriptorTable.pDescriptorRanges = descriptorRangeDepthTexture;	//Tableの中身の配列を指定
		depthTextureParam.DescriptorTable.NumDescriptorRanges = _countof(descriptorRangeDepthTexture);	//Tableで利用する数

		//マテリアル
		D3D12_ROOT_PARAMETER& materialParam = rootParameters[static_cast<size_t>(DepthOutlineRootParam::kMaterial)];
		materialParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//CBVを使う
		materialParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	//PixelShaderで使う
		materialParam.Descriptor.ShaderRegister = 0;	//レジスタ番号0とバインド

		break;
	}
	case DrawMode::kDissolveRendering:
	{
		D3D12_DESCRIPTOR_RANGE descriptorMaskTexture[1] = {};
		descriptorMaskTexture[0] = CreateDescriptorRange(1);	//t1

		rootParameters.resize(static_cast<size_t>(DissolveRootParam::kCount));

		//テクスチャ
		D3D12_ROOT_PARAMETER& textureParam = rootParameters[static_cast<size_t>(DissolveRootParam::kTexture)];
		textureParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//DescriptorTableを使う
		textureParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	//PixelShaderで使う
		textureParam.DescriptorTable.pDescriptorRanges = descriptorRange;	//Tableの中身の配列を指定
		textureParam.DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);	//Tableで利用する数

		//MaskTexture
		D3D12_ROOT_PARAMETER& maskTextureParam = rootParameters[static_cast<size_t>(DissolveRootParam::kMaskTexture)];
		maskTextureParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//DescriptorTableを使う
		maskTextureParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	//PixelShaderで使う
		maskTextureParam.DescriptorTable.pDescriptorRanges = descriptorMaskTexture;	//Tableの中身の配列を指定
		maskTextureParam.DescriptorTable.NumDescriptorRanges = _countof(descriptorMaskTexture);	//Tableで利用する数

		break;
	}

	case DrawMode::kLineMode:
	{
		rootParameters.resize(static_cast<size_t>(DebudLineRootParam::kCount));

		//WVP
		D3D12_ROOT_PARAMETER& wvpParam = rootParameters[static_cast<size_t>(DebudLineRootParam::kWVP)];
		wvpParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//CBVを使う
		wvpParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//VSとGSで使う
		wvpParam.Descriptor.ShaderRegister = 0;	//レジスタ番号0を使う

		break;
	}
	case DrawMode::kSphereMode:
	{
		rootParameters.resize(static_cast<size_t>(DebugSphereRootParam::kCount));

		D3D12_ROOT_PARAMETER& wvpParam = rootParameters[static_cast<size_t>(DebugSphereRootParam::kWVP)];
		wvpParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//CBVを使う
		wvpParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//VSとPSで使う
		wvpParam.Descriptor.ShaderRegister = 0;	//レジスタ番号0を使う
		break;
	}
	case DrawMode::kSkyboxMode:
	{
		rootParameters.resize(static_cast<size_t>(SkyBoxRootParam::kCount));

		//マテリアル
		D3D12_ROOT_PARAMETER& materialParam = rootParameters[static_cast<size_t>(SkyBoxRootParam::kMaterial)];
		materialParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//CBVを使う
		materialParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	//PixelShaderで使う
		materialParam.Descriptor.ShaderRegister = 0;	//レジスタ番号0とバインド

		//TransformationMatrix
		D3D12_ROOT_PARAMETER& transformParam = rootParameters[static_cast<size_t>(SkyBoxRootParam::kTransformationMatrix)];
		transformParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//CBVを使う
		transformParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;	//VertexShaderで使う
		transformParam.Descriptor.ShaderRegister = 0;	//レジスタ番号0を使う

		//テクスチャ
		D3D12_ROOT_PARAMETER& textureParam = rootParameters[static_cast<size_t>(SkyBoxRootParam::kTexture)];
		textureParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//DescriptorTableを使う
		textureParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	//PixelShaderで使う
		textureParam.DescriptorTable.pDescriptorRanges = descriptorRange;	//Tableの中身の配列を指定
		textureParam.DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);	//Tableで利用する数

		break;
	}
	case DrawMode::kSkinModelMode:
	{
		rootParameters.resize(static_cast<size_t>(SkinModelRootParam::kCount));

		//マテリアル
		D3D12_ROOT_PARAMETER& materialParam = rootParameters[static_cast<size_t>(SkinModelRootParam::kMaterial)];
		materialParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//CBVを使う
		materialParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	//PixelShaderで使う
		materialParam.Descriptor.ShaderRegister = 0;	//レジスタ番号0とバインド

		//TransformationMatrix
		D3D12_ROOT_PARAMETER& transformParam = rootParameters[static_cast<size_t>(SkinModelRootParam::kTransformationMatrix)];
		transformParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//CBVを使う
		transformParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;	//VertexShaderで使う
		transformParam.Descriptor.ShaderRegister = 0;	//レジスタ番号0を使う

		//テクスチャ
		D3D12_ROOT_PARAMETER& textureParam = rootParameters[static_cast<size_t>(SkinModelRootParam::kTexture)];
		textureParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//DescriptorTableを使う
		textureParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	//PixelShaderで使う
		textureParam.DescriptorTable.pDescriptorRanges = descriptorRange;	//Tableの中身の配列を指定
		textureParam.DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);	//Tableで利用する数

		//平行光源
		D3D12_ROOT_PARAMETER& directionalLightParam = rootParameters[static_cast<size_t>(SkinModelRootParam::kDirectionalLight)];
		directionalLightParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//DescriptorTableを使う
		directionalLightParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	//PixelShaderで使う
		directionalLightParam.DescriptorTable.pDescriptorRanges = descriptorRangeDirectionalLight;	//Tableの中身の配列を指定
		directionalLightParam.DescriptorTable.NumDescriptorRanges = _countof(descriptorRangeDirectionalLight);	//Tableで利用する数

		//カメラ
		D3D12_ROOT_PARAMETER& cameraParam = rootParameters[static_cast<size_t>(SkinModelRootParam::kCamera)];
		cameraParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//CBVを使う
		cameraParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	//PixelShaderで使う
		cameraParam.Descriptor.ShaderRegister = 1;

		//点光源
		D3D12_ROOT_PARAMETER& pointLightParam = rootParameters[static_cast<size_t>(SkinModelRootParam::kPointLight)];
		pointLightParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//DescriptorTableを使う
		pointLightParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	//PixelShaderで使う
		pointLightParam.DescriptorTable.pDescriptorRanges = descriptorRangePointLight;	//Tableの中身の配列を指定
		pointLightParam.DescriptorTable.NumDescriptorRanges = _countof(descriptorRangePointLight);	//Tableで利用する数

		//スポットライト
		D3D12_ROOT_PARAMETER& spotLightParam = rootParameters[static_cast<size_t>(SkinModelRootParam::kSpotLight)];
		spotLightParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//DescriptorTableを使う
		spotLightParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	//PixelShaderで使う
		spotLightParam.DescriptorTable.pDescriptorRanges = descriptorRangeSpotLight;	//Tableの中身の配列を指定
		spotLightParam.DescriptorTable.NumDescriptorRanges = _countof(descriptorRangeSpotLight);	//Tableで利用する数

		//ライトカウント
		D3D12_ROOT_PARAMETER& lightCountParam = rootParameters[static_cast<size_t>(SkinModelRootParam::kLightCount)];
		lightCountParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//CBVを使う
		lightCountParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	//PixelShaderで使う
		lightCountParam.Descriptor.ShaderRegister = 2;

		//環境マップ
		D3D12_ROOT_PARAMETER& environmentMapParam = rootParameters[static_cast<size_t>(SkinModelRootParam::kEnvironmentMap)];
		environmentMapParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//DescriptorTableを使う
		environmentMapParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	//PixelShaderで使う
		environmentMapParam.DescriptorTable.pDescriptorRanges = descriptorRangeEnvironmentTexture;	//Tableの中身の配列を指定
		environmentMapParam.DescriptorTable.NumDescriptorRanges = _countof(descriptorRangeEnvironmentTexture);	//Tableで利用する数

		//Well
		D3D12_ROOT_PARAMETER& wellParam = rootParameters[static_cast<size_t>(SkinModelRootParam::kWell)];
		wellParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//DescriptorTableを使う
		wellParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;	//VertexShaderで使う
		wellParam.DescriptorTable.pDescriptorRanges = descriptorRange;	//Tableの中身の配列を指定
		wellParam.DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);	//Tableで利用する数

		break;
	}
	case DrawMode::kBlendModeNoneSprite:
	case DrawMode::kBlendModeNormalSprite:
	case DrawMode::kBackGroundSprite:
	{
		rootParameters.resize(static_cast<size_t>(SpriteRootParam::kCount));
		//マテリアル
		D3D12_ROOT_PARAMETER& materialParam = rootParameters[static_cast<size_t>(SpriteRootParam::kMaterial)];
		materialParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//CBVを使う
		materialParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	//PixelShaderで使う
		materialParam.Descriptor.ShaderRegister = 0;	//レジスタ番号0とバインド
		//TransformationMatrix
		D3D12_ROOT_PARAMETER& transformParam = rootParameters[static_cast<size_t>(SpriteRootParam::kTransformationMatrix)];
		transformParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//CBVを使う
		transformParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;	//VertexShaderで使う
		transformParam.Descriptor.ShaderRegister = 0;	//レジスタ番号0を使う
		//テクスチャ
		D3D12_ROOT_PARAMETER& textureParam = rootParameters[static_cast<size_t>(SpriteRootParam::kTexture)];
		textureParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//DescriptorTableを使う
		textureParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	//PixelShaderで使う
		textureParam.DescriptorTable.pDescriptorRanges = descriptorRange;	//Tableの中身の配列を指定
		textureParam.DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);	//Tableで利用する数
		break;
	}
	case DrawMode::kBlendModeAddParticle:
	case DrawMode::kBlendModeNormalParticle:
	case DrawMode::kBlendModeAddBackDrawParticle:
	case DrawMode::kBlendModeNormalBackDrawParticle:
	{
		rootParameters.resize(static_cast<size_t>(ParticleRootParam::kCount));

		//マテリアル
		D3D12_ROOT_PARAMETER& materialParam = rootParameters[static_cast<size_t>(ParticleRootParam::kMaterial)];
		materialParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//CBVを使う
		materialParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	//PixelShaderで使う
		materialParam.Descriptor.ShaderRegister = 0;	//レジスタ番号0とバインド

		//ParticleForGPU
		D3D12_ROOT_PARAMETER& particleParam = rootParameters[static_cast<size_t>(ParticleRootParam::kParticleForGPU)];
		particleParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//DescriptorTableを使う
		particleParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;	//VertexShaderで使う
		particleParam.DescriptorTable.pDescriptorRanges = descriptorRange;	//Tableの中身の配列を指定
		particleParam.DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);	//Tableで利用する数

		//テクスチャ
		D3D12_ROOT_PARAMETER& textureParam = rootParameters[static_cast<size_t>(ParticleRootParam::kTexture)];
		textureParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//DescriptorTableを使う
		textureParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	//PixelShaderで使う
		textureParam.DescriptorTable.pDescriptorRanges = descriptorRange;	//Tableの中身の配列を指定
		textureParam.DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);	//Tableで利用する数

		break;
	}

	default:
	{
		rootParameters.resize(static_cast<size_t>(ModelRootParam::kCount));

		//マテリアル
		D3D12_ROOT_PARAMETER& materialParam = rootParameters[static_cast<size_t>(ModelRootParam::kMaterial)];
		materialParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//CBVを使う
		materialParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	//PixelShaderで使う
		materialParam.Descriptor.ShaderRegister = 0;	//レジスタ番号0とバインド

		switch (blendMode) {
		default:
		{
			//Object3d用
			//TransformationMatrix
			D3D12_ROOT_PARAMETER& transformParam = rootParameters[static_cast<size_t>(ModelRootParam::kTransformationMatrix)];
			transformParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//CBVを使う
			transformParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;	//VertexShaderで使う
			transformParam.Descriptor.ShaderRegister = 0;	//レジスタ番号0を使う
			break;
		}
		case DrawMode::kBlendModeNormalinstancing:
		{
			//Particle用、instancing用
			//ParticleForGPU、TransformationMatrix
			D3D12_ROOT_PARAMETER& transformParam = rootParameters[static_cast<size_t>(ModelRootParam::kTransformationMatrix)];
			transformParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//DescriptorTableを使う
			transformParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;	//VertexShaderで使う
			transformParam.DescriptorTable.pDescriptorRanges = descriptorRange;
			transformParam.DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);

			break;
		}
		}

		//テクスチャ
		D3D12_ROOT_PARAMETER& textureParam = rootParameters[static_cast<size_t>(ModelRootParam::kTexture)];
		textureParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//DescriptorTableを使う
		textureParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	//PixelShaderで使う
		textureParam.DescriptorTable.pDescriptorRanges = descriptorRange;	//Tableの中身の配列を指定
		textureParam.DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);	//Tableで利用する数

		//平行光源
		D3D12_ROOT_PARAMETER& directionalLightParam = rootParameters[static_cast<size_t>(ModelRootParam::kDirectionalLight)];
		directionalLightParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//DescriptorTableを使う
		directionalLightParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	//PixelShaderで使う
		directionalLightParam.DescriptorTable.pDescriptorRanges = descriptorRangeDirectionalLight;	//Tableの中身の配列を指定
		directionalLightParam.DescriptorTable.NumDescriptorRanges = _countof(descriptorRangeDirectionalLight);	//Tableで利用する数

		//カメラ
		D3D12_ROOT_PARAMETER& cameraParam = rootParameters[static_cast<size_t>(ModelRootParam::kCamera)];
		cameraParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//CBVを使う
		cameraParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	//PixelShaderで使う
		cameraParam.Descriptor.ShaderRegister = 1;

		//点光源
		D3D12_ROOT_PARAMETER& pointLightParam = rootParameters[static_cast<size_t>(ModelRootParam::kPointLight)];
		pointLightParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//DescriptorTableを使う
		pointLightParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	//PixelShaderで使う
		pointLightParam.DescriptorTable.pDescriptorRanges = descriptorRangePointLight;	//Tableの中身の配列を指定
		pointLightParam.DescriptorTable.NumDescriptorRanges = _countof(descriptorRangePointLight);	//Tableで利用する数

		//スポットライト
		D3D12_ROOT_PARAMETER& spotLightParam = rootParameters[static_cast<size_t>(ModelRootParam::kSpotLight)];
		spotLightParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//DescriptorTableを使う
		spotLightParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	//PixelShaderで使う
		spotLightParam.DescriptorTable.pDescriptorRanges = descriptorRangeSpotLight;	//Tableの中身の配列を指定
		spotLightParam.DescriptorTable.NumDescriptorRanges = _countof(descriptorRangeSpotLight);	//Tableで利用する数

		//ライトカウント
		D3D12_ROOT_PARAMETER& lightCountParam = rootParameters[static_cast<size_t>(ModelRootParam::kLightCount)];
		lightCountParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//CBVを使う
		lightCountParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	//PixelShaderで使う
		lightCountParam.Descriptor.ShaderRegister = 2;

		//環境マップ
		D3D12_ROOT_PARAMETER& environmentMapParam = rootParameters[static_cast<size_t>(ModelRootParam::kEnvironmentMap)];
		environmentMapParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//DescriptorTableを使う
		environmentMapParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	//PixelShaderで使う
		environmentMapParam.DescriptorTable.pDescriptorRanges = descriptorRangeEnvironmentTexture;	//Tableの中身の配列を指定
		environmentMapParam.DescriptorTable.NumDescriptorRanges = _countof(descriptorRangeEnvironmentTexture);	//Tableで利用する数

		break;
	}
	}

	descriptionRootSignature.pParameters = rootParameters.data();	//ルートパラメータ配列へのポインタ
	descriptionRootSignature.NumParameters = static_cast<UINT>(rootParameters.size());	//配列の長さ

	//Samplerの設定
	std::vector<D3D12_STATIC_SAMPLER_DESC> staticSamplers = {};

	switch (blendMode) {
	case DrawMode::kLineMode:
	case DrawMode::kSphereMode:

		break;

	case DrawMode::kOutlineRendering:

		staticSamplers.resize(2);

		staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;	//バイリニアフィルタ
		staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;	//0～1の範囲外をリピート
		staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;	//比較しない
		staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX;	//ありったけのMipmapｗｐ使う
		staticSamplers[0].ShaderRegister = 0;	//レジスタ番号0を使う s0
		staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	//PixelShaderで使う

		staticSamplers[1].Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;	//ポイントフィルタ
		staticSamplers[1].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;	//0～1の範囲外をリピート
		staticSamplers[1].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		staticSamplers[1].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		staticSamplers[1].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;	//比較しない
		staticSamplers[1].MaxLOD = D3D12_FLOAT32_MAX;	//ありったけのMipmapｗｐ使う
		staticSamplers[1].ShaderRegister = 1;	//レジスタ番号1を使う s1
		staticSamplers[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	//PixelShaderで使う

		break;

	default:

		staticSamplers.resize(1);

		staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;	//バイリニアフィルタ
		staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;	//0～1の範囲外をリピート
		staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;	//比較しない
		staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX;	//ありったけのMipmapｗｐ使う
		staticSamplers[0].ShaderRegister = 0;	//レジスタ番号0を使う s0
		staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	//PixelShaderで使う

		break;

	}

	
	descriptionRootSignature.pStaticSamplers = staticSamplers.data();
	descriptionRootSignature.NumStaticSamplers = static_cast<UINT>(staticSamplers.size());

	//シリアナイズしてバイナリにする
	Microsoft::WRL::ComPtr<ID3DBlob> signatureBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;
	HRESULT hr = D3D12SerializeRootSignature(&descriptionRootSignature,
		D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
	if (FAILED(hr)) {
		Logger::Log(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
		assert(false);
	}
	//バイナリを元に生成
	hr = dxCommon->GetDevice()->CreateRootSignature(0, signatureBlob->GetBufferPointer(),
		signatureBlob->GetBufferSize(), IID_PPV_ARGS(&pipelineSet->rootSignature));
	assert(SUCCEEDED(hr));

	//InputLayout
	std::vector<D3D12_INPUT_ELEMENT_DESC> inputElementDescs = {};

	switch (blendMode) {
	case DrawMode::kLineMode:
	case DrawMode::kSphereMode:

		inputElementDescs.resize(1);

		inputElementDescs[0].SemanticName = "POSITION";
		inputElementDescs[0].SemanticIndex = 0;
		inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

		break;

	case DrawMode::kSkinModelMode:

		inputElementDescs.resize(5);

		inputElementDescs[0].SemanticName = "POSITION";
		inputElementDescs[0].SemanticIndex = 0;
		inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
		inputElementDescs[1].SemanticName = "TEXCOORD";
		inputElementDescs[1].SemanticIndex = 0;
		inputElementDescs[1].Format = DXGI_FORMAT_R32G32_FLOAT;
		inputElementDescs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
		inputElementDescs[2].SemanticName = "NORMAL";
		inputElementDescs[2].SemanticIndex = 0;
		inputElementDescs[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		inputElementDescs[2].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
		inputElementDescs[3].SemanticName = "WEIGHT";
		inputElementDescs[3].SemanticIndex = 0;
		inputElementDescs[3].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;	//float32_t4
		inputElementDescs[3].InputSlot = 1;	//1番目のslotのVBVのことだと伝える
		inputElementDescs[3].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
		inputElementDescs[4].SemanticName = "INDEX";
		inputElementDescs[4].SemanticIndex = 0;
		inputElementDescs[4].Format = DXGI_FORMAT_R32G32B32A32_SINT;	//int32_t4
		inputElementDescs[4].InputSlot = 1;	//1番目のslotのVBVのことだと伝える
		inputElementDescs[4].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

		break;

	default:

		inputElementDescs.resize(3);

		inputElementDescs[0].SemanticName = "POSITION";
		inputElementDescs[0].SemanticIndex = 0;
		inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
		inputElementDescs[1].SemanticName = "TEXCOORD";
		inputElementDescs[1].SemanticIndex = 0;
		inputElementDescs[1].Format = DXGI_FORMAT_R32G32_FLOAT;
		inputElementDescs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
		inputElementDescs[2].SemanticName = "NORMAL";
		inputElementDescs[2].SemanticIndex = 0;
		inputElementDescs[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		inputElementDescs[2].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

		break;
	}

	

	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};

	//InputLayoutの設定
	switch (blendMode) {
	case DrawMode::kOffScreenRendering:
	case DrawMode::kGrayScaleRendering:
	case DrawMode::kVignetteRendering:
	case DrawMode::kBoxFilterRendering:
	case DrawMode::kGaussianFilterRendering:
	case DrawMode::kLuminanceOutlineRendering:
	case DrawMode::kOutlineRendering:
	case DrawMode::kRadialBlurRendering:
	case DrawMode::kDissolveRendering:
	case DrawMode::kRandomRendering:

		//フルスクリーン用のシェーダーは頂点情報を使わないので、
		//InputLayoutは使わない
		inputLayoutDesc.pInputElementDescs = nullptr;
		inputLayoutDesc.NumElements = 0;

		break;

	default:

		inputLayoutDesc.pInputElementDescs = inputElementDescs.data();
		inputLayoutDesc.NumElements = static_cast<UINT>(inputElementDescs.size());

		break;
	}
	

	//BlendStateの設定
	D3D12_BLEND_DESC blendDesc{};
	//すべての色要素を書き込む
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;

	switch (blendMode) {
	case DrawMode::kBlendModeNone:
	default:
		break;

	case DrawMode::kBlendModeNormal:
	case DrawMode::kBlendModeNormalSprite:
	case DrawMode::kBlendModeNormalinstancing:
	case DrawMode::kBlendModeNormalParticle:
	case DrawMode::kBlendModeNormalBackDrawParticle:
		blendDesc.RenderTarget[0].BlendEnable = TRUE;
		blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
		
		break;

	case DrawMode::kBlendModeAdd:
	case DrawMode::kBlendModeAddParticle:
	case DrawMode::kBlendModeAddBackDrawParticle:
		blendDesc.RenderTarget[0].BlendEnable = TRUE;
		blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
		break;

	case DrawMode::kBlendModeSubtract:
		blendDesc.RenderTarget[0].BlendEnable = TRUE;
		blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;
		blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
		break;

	case DrawMode::kBlendModeMultiply:
		blendDesc.RenderTarget[0].BlendEnable = TRUE;
		blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_SRC_COLOR;
		break;

	case DrawMode::kBlendModeScreen:
		blendDesc.RenderTarget[0].BlendEnable = TRUE;
		blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_INV_DEST_COLOR;
		blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
		break;
	}
	
	//ResiterizerStateの設定
	D3D12_RASTERIZER_DESC rasterizerDesc{};
	switch (blendMode) {
	case DrawMode::kBlendModeNoneSprite:
	case DrawMode::kBlendModeNormalSprite:
	case DrawMode::kBackGroundSprite:
	case DrawMode::kBlendModeAddBackDrawParticle:
	case DrawMode::kBlendModeNormalBackDrawParticle:
		//裏面（時計回り）を表示する
		rasterizerDesc.CullMode = D3D12_CULL_MODE_NONE;
		break;

	default:
		//裏面（時計回り）を表示しない
		rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
		break;
	}
	
	//三角形の中を塗りつぶす
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;

	//Shaderをコンパイルする
	Microsoft::WRL::ComPtr<IDxcBlob> vertexShaderBlob;
	Microsoft::WRL::ComPtr<IDxcBlob> GeometryShaderBlob;
	Microsoft::WRL::ComPtr<IDxcBlob> pixelShaderBlob;

	switch (blendMode) {
	default:
		//Object3d用
		
		vertexShaderBlob = dxCommon->CompilerShader(L"resources/shader/Object3d.VS.hlsl",
			L"vs_6_0");
		assert(vertexShaderBlob != nullptr);

		pixelShaderBlob = dxCommon->CompilerShader(L"resources/shader/Object3d.PS.hlsl",
			L"ps_6_0");
		assert(pixelShaderBlob != nullptr);
		break;

	case DrawMode::kOffScreenRendering:

		vertexShaderBlob = dxCommon->CompilerShader(L"resources/shader/FullScreen.VS.hlsl",
			L"vs_6_0");
		assert(vertexShaderBlob != nullptr);

		pixelShaderBlob = dxCommon->CompilerShader(L"resources/shader/CopyImage.PS.hlsl",
			L"ps_6_0");
		assert(pixelShaderBlob != nullptr);

		break;
	
	case DrawMode::kGrayScaleRendering:

		vertexShaderBlob = dxCommon->CompilerShader(L"resources/shader/FullScreen.VS.hlsl",
			L"vs_6_0");
		assert(vertexShaderBlob != nullptr);
		pixelShaderBlob = dxCommon->CompilerShader(L"resources/shader/GrayScale.PS.hlsl",
			L"ps_6_0");
		assert(pixelShaderBlob != nullptr);

		break;

	case DrawMode::kVignetteRendering:

		vertexShaderBlob = dxCommon->CompilerShader(L"resources/shader/FullScreen.VS.hlsl",
			L"vs_6_0");
		assert(vertexShaderBlob != nullptr);
		pixelShaderBlob = dxCommon->CompilerShader(L"resources/shader/Vignette.PS.hlsl",
			L"ps_6_0");
		assert(pixelShaderBlob != nullptr);

		break;

	case DrawMode::kBoxFilterRendering:

		vertexShaderBlob = dxCommon->CompilerShader(L"resources/shader/FullScreen.VS.hlsl",
			L"vs_6_0");
		assert(vertexShaderBlob != nullptr);
		pixelShaderBlob = dxCommon->CompilerShader(L"resources/shader/5x5BoxFilter.PS.hlsl",
			L"ps_6_0");
		assert(pixelShaderBlob != nullptr);

		break;

	case DrawMode::kGaussianFilterRendering:

		vertexShaderBlob = dxCommon->CompilerShader(L"resources/shader/FullScreen.VS.hlsl",
			L"vs_6_0");
		assert(vertexShaderBlob != nullptr);
		pixelShaderBlob = dxCommon->CompilerShader(L"resources/shader/GaussianFilter.PS.hlsl",
			L"ps_6_0");
		assert(pixelShaderBlob != nullptr);

		break;

	case DrawMode::kLuminanceOutlineRendering:

		vertexShaderBlob = dxCommon->CompilerShader(L"resources/shader/FullScreen.VS.hlsl",
			L"vs_6_0");
		assert(vertexShaderBlob != nullptr);
		pixelShaderBlob = dxCommon->CompilerShader(L"resources/shader/LuminanceBasedOutline.PS.hlsl",
			L"ps_6_0");
		assert(pixelShaderBlob != nullptr);

		break;

	case DrawMode::kOutlineRendering:

		vertexShaderBlob = dxCommon->CompilerShader(L"resources/shader/FullScreen.VS.hlsl",
			L"vs_6_0");
		assert(vertexShaderBlob != nullptr);
		pixelShaderBlob = dxCommon->CompilerShader(L"resources/shader/Outline.PS.hlsl",
			L"ps_6_0");
		assert(pixelShaderBlob != nullptr);

		break;
		
	case DrawMode::kRadialBlurRendering:

		vertexShaderBlob = dxCommon->CompilerShader(L"resources/shader/FullScreen.VS.hlsl",
			L"vs_6_0");
		assert(vertexShaderBlob != nullptr);
		pixelShaderBlob = dxCommon->CompilerShader(L"resources/shader/RadialBlur.PS.hlsl",
			L"ps_6_0");
		assert(pixelShaderBlob != nullptr);

		break;

	case DrawMode::kDissolveRendering:

		vertexShaderBlob = dxCommon->CompilerShader(L"resources/shader/FullScreen.VS.hlsl",
			L"vs_6_0");
		assert(vertexShaderBlob != nullptr);
		pixelShaderBlob = dxCommon->CompilerShader(L"resources/shader/Dissolve.PS.hlsl",
			L"ps_6_0");
		assert(pixelShaderBlob != nullptr);

		break;

	case DrawMode::kRandomRendering:

		vertexShaderBlob = dxCommon->CompilerShader(L"resources/shader/FullScreen.VS.hlsl",
			L"vs_6_0");
		assert(vertexShaderBlob != nullptr);
		pixelShaderBlob = dxCommon->CompilerShader(L"resources/shader/Random.PS.hlsl",
			L"ps_6_0");
		assert(pixelShaderBlob != nullptr);

		break;

	case DrawMode::kBlendModeNormalinstancing:
		vertexShaderBlob = dxCommon->CompilerShader(L"resources/shader/InstancingObject3d.VS.hlsl",
			L"vs_6_0");
		assert(vertexShaderBlob != nullptr);

		pixelShaderBlob = dxCommon->CompilerShader(L"resources/shader/Object3d.PS.hlsl",
			L"ps_6_0");
		assert(pixelShaderBlob != nullptr);
		break;

	case DrawMode::kBlendModeNoneSprite:
	case DrawMode::kBlendModeNormalSprite:
	case DrawMode::kBackGroundSprite:

		vertexShaderBlob = dxCommon->CompilerShader(L"resources/shader/Sprite.VS.hlsl",
			L"vs_6_0");
		assert(vertexShaderBlob != nullptr);

		pixelShaderBlob = dxCommon->CompilerShader(L"resources/shader/Sprite.PS.hlsl",
			L"ps_6_0");
		assert(pixelShaderBlob != nullptr);

		break;

	case DrawMode::kBlendModeAddParticle:
	case DrawMode::kBlendModeNormalParticle:
	case DrawMode::kBlendModeAddBackDrawParticle:
	case DrawMode::kBlendModeNormalBackDrawParticle:

		//Particle用
		vertexShaderBlob = dxCommon->CompilerShader(L"resources/shader/Particle.VS.hlsl",
			L"vs_6_0");
		assert(vertexShaderBlob != nullptr);

		pixelShaderBlob = dxCommon->CompilerShader(L"resources/shader/Particle.PS.hlsl",
			L"ps_6_0");
		assert(pixelShaderBlob != nullptr);

		break;

	case DrawMode::kLineMode:

		vertexShaderBlob = dxCommon->CompilerShader(L"resources/shader/Line3D.VS.hlsl",
			L"vs_6_0");
		assert(vertexShaderBlob != nullptr);

		GeometryShaderBlob = dxCommon->CompilerShader(L"resources/shader/Line3D.GS.hlsl", 
			L"gs_6_0");
		assert(GeometryShaderBlob != nullptr);

		pixelShaderBlob = dxCommon->CompilerShader(L"resources/shader/Line3D.PS.hlsl",
			L"ps_6_0");
		assert(pixelShaderBlob != nullptr);

		break;
	case DrawMode::kSphereMode:

		vertexShaderBlob = dxCommon->CompilerShader(L"resources/shader/Sphere3D.VS.hlsl",
			L"vs_6_0");
		assert(vertexShaderBlob != nullptr);

		GeometryShaderBlob = dxCommon->CompilerShader(L"resources/shader/Sphere3D.GS.hlsl",
			L"gs_6_0");
		assert(GeometryShaderBlob != nullptr);

		pixelShaderBlob = dxCommon->CompilerShader(L"resources/shader/Sphere3D.PS.hlsl",
			L"ps_6_0");
		assert(pixelShaderBlob != nullptr);

		break;

	case DrawMode::kSkinModelMode:


		vertexShaderBlob = dxCommon->CompilerShader(L"resources/shader/SkinningObject3d.VS.hlsl",
			L"vs_6_0");
		assert(vertexShaderBlob != nullptr);

		pixelShaderBlob = dxCommon->CompilerShader(L"resources/shader/Object3d.PS.hlsl",
			L"ps_6_0");
		assert(pixelShaderBlob != nullptr);

		break;

	case DrawMode::kSkyboxMode:

		vertexShaderBlob = dxCommon->CompilerShader(L"resources/shader/Skybox.VS.hlsl",
			L"vs_6_0");
		assert(vertexShaderBlob != nullptr);

		pixelShaderBlob = dxCommon->CompilerShader(L"resources/shader/Skybox.PS.hlsl",
			L"ps_6_0");
		assert(pixelShaderBlob != nullptr);

		break;
	}

	

	//DepthStencilStateの設定
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	
	switch (blendMode) {
	default:
		//Depthの機能を有効化する
		depthStencilDesc.DepthEnable = true;
		//書き込みします
		depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
		break;

	case DrawMode::kOffScreenRendering:
	case DrawMode::kGrayScaleRendering:
	case DrawMode::kVignetteRendering:
	case DrawMode::kBoxFilterRendering:
	case DrawMode::kGaussianFilterRendering:
	case DrawMode::kLuminanceOutlineRendering:
	case DrawMode::kOutlineRendering:
	case DrawMode::kRadialBlurRendering:
	case DrawMode::kDissolveRendering:
	case DrawMode::kRandomRendering:

		//Depthの機能を無効化する
		depthStencilDesc.DepthEnable = false;
		break;

	case DrawMode::kLineMode:
	case DrawMode::kSphereMode:
		//Depthの機能を有効化する
		depthStencilDesc.DepthEnable = false;
		//書き込みします
		depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;

		break;
	case DrawMode::kBlendModeAddParticle:
	case DrawMode::kBlendModeNormalParticle:
	case DrawMode::kBlendModeAddBackDrawParticle:
	case DrawMode::kBlendModeNormalBackDrawParticle:
		//Depthの機能を有効化する
		depthStencilDesc.DepthEnable = true;
		depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
		break;

	case DrawMode::kBackGroundSprite:
		depthStencilDesc.DepthEnable = FALSE;  // 深度バッファ無効化
		depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;  // 深度への書き込み無効
		depthStencilDesc.StencilEnable = FALSE;
		break;

	case DrawMode::kSkyboxMode:
		depthStencilDesc.DepthEnable = true;	//比較はするのでDepth自体は有効
		depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;	//全ピクセルがz=1に出力されるので、書き込む必要はない

		break;
	}
	//比較関数はLessEqual。つまり、近ければ描画される
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;

	//PSOを作成
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
	graphicsPipelineStateDesc.pRootSignature = pipelineSet->rootSignature.Get();	//RootSignature
	graphicsPipelineStateDesc.InputLayout = inputLayoutDesc;	//InputLayout
	graphicsPipelineStateDesc.VS = { vertexShaderBlob->GetBufferPointer(),
	vertexShaderBlob->GetBufferSize() };	//VertexShader
	graphicsPipelineStateDesc.PS = { pixelShaderBlob->GetBufferPointer(),
	pixelShaderBlob->GetBufferSize() };		//PixelShader
	switch (blendMode) {
	default:

		break;
	case DrawMode::kLineMode:
	case DrawMode::kSphereMode:

		graphicsPipelineStateDesc.GS = { GeometryShaderBlob->GetBufferPointer(),
		GeometryShaderBlob->GetBufferSize() };

		break;
	}
	graphicsPipelineStateDesc.BlendState = blendDesc;	//BlendState
	graphicsPipelineStateDesc.RasterizerState = rasterizerDesc;	//ResterizerState

	//DepthStencilの設定
	graphicsPipelineStateDesc.DepthStencilState = depthStencilDesc;
	graphicsPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	//書き込むRTVの情報
	graphicsPipelineStateDesc.NumRenderTargets = 1;
	graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;


	//利用するトポロジ（形状）のタイプ。
	switch (blendMode) {
	default:
		graphicsPipelineStateDesc.PrimitiveTopologyType =
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE; 
		break;

	case DrawMode::kLineMode:
	case DrawMode::kSphereMode:

		graphicsPipelineStateDesc.PrimitiveTopologyType =
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
		break;

	}
	//どのように画面に色を打ち込むかの設定
	graphicsPipelineStateDesc.SampleDesc.Count = 1;
	graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	//実際に生成
	hr = dxCommon->GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDesc,
		IID_PPV_ARGS(&pipelineSet->graphicsPipelineState));
	assert(SUCCEEDED(hr));

	return pipelineSet;
}

D3D12_DESCRIPTOR_RANGE YKEngine::PipelineManager::CreateDescriptorRange(uint32_t BaseShaderRegister)
{
	D3D12_DESCRIPTOR_RANGE descriptorRange{};
	descriptorRange.BaseShaderRegister = BaseShaderRegister;	//シェーダーレジスタ番号 t0など
	descriptorRange.NumDescriptors = 1;	//数は1つ
	descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;	//SRVを使う
	descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;	//Offsetを自動計算

	return descriptorRange;
}

void PipelineManager::SetPipelineSet(ID3D12GraphicsCommandList* commandList, DrawMode blendMode)
{

	commandList->SetGraphicsRootSignature(pipelineSets_.at(static_cast<uint16_t>(blendMode))->rootSignature.Get());
	commandList->SetPipelineState(pipelineSets_.at(static_cast<uint16_t>(blendMode))->graphicsPipelineState.Get());	//PSOを設定

}
