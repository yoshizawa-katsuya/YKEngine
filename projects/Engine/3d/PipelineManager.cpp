#include "dx12.h"
#include "PipelineManager.h"
#include <cassert>
#include <format>
#include "DirectXCommon.h"
#include "RootParams.h"

using namespace YKEngine;
using namespace Microsoft::WRL;

const std::unordered_map<DrawMode, PipelineManager::PipelineConfig> PipelineManager::pipelineTable_ =
{
	{
		DrawMode::kBlendModeNone,
		{
			L"resources/shader/Object3d.VS.hlsl",
			L"resources/shader/Object3d.PS.hlsl",
			L"",
			RootSignatureType::Model,
			InputLayoutType::Default,
			BlendType::None,
			RasterizerType::CullBack,
			DepthType::EnableWrite,
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE
		}
	},

	{
		DrawMode::kBlendModeNormal,
		{
			L"resources/shader/Object3d.VS.hlsl",
			L"resources/shader/Object3d.PS.hlsl",
			L"",
			RootSignatureType::Model,
			InputLayoutType::Default,
			BlendType::Normal,
			RasterizerType::CullBack,
			DepthType::EnableWrite,
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE
		}
	},

	{
		DrawMode::kBlendModeAdd,
		{
			L"resources/shader/Object3d.VS.hlsl",
			L"resources/shader/Object3d.PS.hlsl",
			L"",
			RootSignatureType::Model,
			InputLayoutType::Default,
			BlendType::Add,
			RasterizerType::CullBack,
			DepthType::EnableWrite,
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE
		}
	},

	{
		DrawMode::kBlendModeSubtract,
		{
			L"resources/shader/Object3d.VS.hlsl",
			L"resources/shader/Object3d.PS.hlsl",
			L"",
			RootSignatureType::Model,
			InputLayoutType::Default,
			BlendType::Subtract,
			RasterizerType::CullBack,
			DepthType::EnableWrite,
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE
		}
	},

	{
		DrawMode::kBlendModeMultiply,
		{
			L"resources/shader/Object3d.VS.hlsl",
			L"resources/shader/Object3d.PS.hlsl",
			L"",
			RootSignatureType::Model,
			InputLayoutType::Default,
			BlendType::Multiply,
			RasterizerType::CullBack,
			DepthType::EnableWrite,
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE
		}
	},

	{
		DrawMode::kBlendModeScreen,
		{
			L"resources/shader/Object3d.VS.hlsl",
			L"resources/shader/Object3d.PS.hlsl",
			L"",
			RootSignatureType::Model,
			InputLayoutType::Default,
			BlendType::Screen,
			RasterizerType::CullBack,
			DepthType::EnableWrite,
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE
		}
	},

	{
		DrawMode::kBlendModeNoneSprite,
		{
			L"resources/shader/Sprite.VS.hlsl",
			L"resources/shader/Sprite.PS.hlsl",
			L"",
			RootSignatureType::Sprite,
			InputLayoutType::Default,
			BlendType::None,
			RasterizerType::CullNone,
			DepthType::EnableWrite,
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE
		}
	},

	{
		DrawMode::kBlendModeNormalSprite,
		{
			L"resources/shader/Sprite.VS.hlsl",
			L"resources/shader/Sprite.PS.hlsl",
			L"",
			RootSignatureType::Sprite,
			InputLayoutType::Default,
			BlendType::Normal,
			RasterizerType::CullNone,
			DepthType::EnableWrite,
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE
		}
	},

	{
		DrawMode::kBackGroundSprite,
		{
			L"resources/shader/Sprite.VS.hlsl",
			L"resources/shader/Sprite.PS.hlsl",
			L"",
			RootSignatureType::Sprite,
			InputLayoutType::Default,
			BlendType::None,
			RasterizerType::CullNone,
			DepthType::NoStencil,
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE
		}
	},

	{
		DrawMode::kBlendModeAddParticle,
		{
			L"resources/shader/Particle.VS.hlsl",
			L"resources/shader/Particle.PS.hlsl",
			L"",
			RootSignatureType::Particle,
			InputLayoutType::Default,
			BlendType::Add,
			RasterizerType::CullBack,
			DepthType::EnableNoWrite,
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE
		}
	},

	{
		DrawMode::kBlendModeNormalParticle,
		{
			L"resources/shader/Particle.VS.hlsl",
			L"resources/shader/Particle.PS.hlsl",
			L"",
			RootSignatureType::Particle,
			InputLayoutType::Default,
			BlendType::Normal,
			RasterizerType::CullBack,
			DepthType::EnableNoWrite,
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE
		}
	},
	{
		DrawMode::kBlendModeAddBackDrawParticle,
		{
			L"resources/shader/Particle.VS.hlsl",
			L"resources/shader/Particle.PS.hlsl",
			L"",
			RootSignatureType::Particle,
			InputLayoutType::Default,
			BlendType::Add,
			RasterizerType::CullNone,
			DepthType::EnableNoWrite,
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE
		}
	},

	{
		DrawMode::kBlendModeNormalBackDrawParticle,
		{
			L"resources/shader/Particle.VS.hlsl",
			L"resources/shader/Particle.PS.hlsl",
			L"",
			RootSignatureType::Particle,
			InputLayoutType::Default,
			BlendType::Normal,
			RasterizerType::CullNone,
			DepthType::EnableNoWrite,
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE
		}
	},

	{
		DrawMode::kLineMode,
		{
			L"resources/shader/Line3D.VS.hlsl",
			L"resources/shader/Line3D.PS.hlsl",
			L"resources/shader/Line3D.GS.hlsl",
			RootSignatureType::Line,
			InputLayoutType::GeometryShader,
			BlendType::None,
			RasterizerType::CullBack,
			DepthType::NoEnableWrite,
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT
		}
	},

	{
		DrawMode::kSphereMode,
		{
			L"resources/shader/Sphere3D.VS.hlsl",
			L"resources/shader/Sphere3D.PS.hlsl",
			L"resources/shader/Sphere3D.GS.hlsl",
			RootSignatureType::Sphere,
			InputLayoutType::GeometryShader,
			BlendType::None,
			RasterizerType::CullBack,
			DepthType::NoEnableWrite,
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT
		}
	},

	{
		DrawMode::kSkinModelMode,
		{
			L"resources/shader/SkinningObject3d.VS.hlsl",
			L"resources/shader/Object3d.PS.hlsl",
			L"",
			RootSignatureType::SkinModel,
			InputLayoutType::SkinModel,
			BlendType::Normal,
			RasterizerType::CullBack,
			DepthType::EnableWrite,
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE
		}
	},

	{
		DrawMode::kBlendModeNormalinstancing,
		{
			L"resources/shader/InstancingObject3d.VS.hlsl",
			L"resources/shader/Object3d.PS.hlsl",
			L"",
			RootSignatureType::InstancingModel,
			InputLayoutType::Default,
			BlendType::Normal,
			RasterizerType::CullBack,
			DepthType::EnableWrite,
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE
		}
	},
	{
		DrawMode::kSkyboxMode,
		{
			L"resources/shader/Skybox.VS.hlsl",
			L"resources/shader/Skybox.PS.hlsl",
			L"",
			RootSignatureType::Skybox,
			InputLayoutType::Default,
			BlendType::None,
			RasterizerType::CullBack,
			DepthType::EnableNoWrite,
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE
		}
	},

	{
		DrawMode::kOffScreenRendering,
		{
			L"resources/shader/FullScreen.VS.hlsl",
			L"resources/shader/CopyImage.PS.hlsl",
			L"",
			RootSignatureType::PostEffect,
			InputLayoutType::None,
			BlendType::None,
			RasterizerType::CullBack,
			DepthType::Disable,
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE
		}
	},

	{
		DrawMode::kGrayScaleRendering,
		{
			L"resources/shader/FullScreen.VS.hlsl",
			L"resources/shader/GrayScale.PS.hlsl",
			L"",
			RootSignatureType::PostEffect,
			InputLayoutType::None,
			BlendType::None,
			RasterizerType::CullBack,
			DepthType::Disable,
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE
		}
	},

	{
		DrawMode::kVignetteRendering,
		{
			L"resources/shader/FullScreen.VS.hlsl",
			L"resources/shader/Vignette.PS.hlsl",
			L"",
			RootSignatureType::PostEffect,
			InputLayoutType::None,
			BlendType::None,
			RasterizerType::CullBack,
			DepthType::Disable,
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE
		}
	},
	{
		DrawMode::kBoxFilterRendering,
		{
			L"resources/shader/FullScreen.VS.hlsl",
			L"resources/shader/5x5BoxFilter.PS.hlsl",
			L"",
			RootSignatureType::PostEffect,
			InputLayoutType::None,
			BlendType::None,
			RasterizerType::CullBack,
			DepthType::Disable,
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE
		}
	},
	{
		DrawMode::kGaussianFilterRendering,
		{
			L"resources/shader/FullScreen.VS.hlsl",
			L"resources/shader/GaussianFilter.PS.hlsl",
			L"",
			RootSignatureType::PostEffect,
			InputLayoutType::None,
			BlendType::None,
			RasterizerType::CullBack,
			DepthType::Disable,
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE
		}
	},
	{
		DrawMode::kLuminanceOutlineRendering,
		{
			L"resources/shader/FullScreen.VS.hlsl",
			L"resources/shader/LuminanceBasedOutline.PS.hlsl",
			L"",
			RootSignatureType::PostEffect,
			InputLayoutType::None,
			BlendType::None,
			RasterizerType::CullBack,
			DepthType::Disable,
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE
		}
	},
	{
		DrawMode::kOutlineRendering,
		{
			L"resources/shader/FullScreen.VS.hlsl",
			L"resources/shader/Outline.PS.hlsl",
			L"",
			RootSignatureType::Outline,
			InputLayoutType::None,
			BlendType::None,
			RasterizerType::CullBack,
			DepthType::Disable,
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE
		}
	},
	{
		DrawMode::kRadialBlurRendering,
		{
			L"resources/shader/FullScreen.VS.hlsl",
			L"resources/shader/RadialBlur.PS.hlsl",
			L"",
			RootSignatureType::PostEffect,
			InputLayoutType::None,
			BlendType::None,
			RasterizerType::CullBack,
			DepthType::Disable,
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE
		}
	},
	{
		DrawMode::kDissolveRendering,
		{
			L"resources/shader/FullScreen.VS.hlsl",
			L"resources/shader/Dissolve.PS.hlsl",
			L"",
			RootSignatureType::Dissolve,
			InputLayoutType::None,
			BlendType::None,
			RasterizerType::CullBack,
			DepthType::Disable,
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE
		}
	},
	{
		DrawMode::kRandomRendering,
		{
			L"resources/shader/FullScreen.VS.hlsl",
			L"resources/shader/Random.PS.hlsl",
			L"",
			RootSignatureType::Random,
			InputLayoutType::None,
			BlendType::None,
			RasterizerType::CullBack,
			DepthType::Disable,
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE
		}
	}
};

void PipelineManager::Initialize(DirectXCommon* dxCommon)
{
	//DrawMode分のパイプラインを作成

	for (const auto& [drawMode, pipelineConfig] : pipelineTable_)
	{
		pipelineSets_.at(static_cast<uint16_t>(drawMode)) = CreateGraphicsPipeline(pipelineConfig, dxCommon);
	}

}

std::unique_ptr<PipelineManager::PipelineSet> PipelineManager::CreateGraphicsPipeline(const PipelineConfig& pipelineConfig, DirectXCommon* dxCommon)
{
	
	ID3D12Device* device = dxCommon->GetDevice();

	ComPtr<ID3D12RootSignature> rootSig = CreateRootSignature(device, pipelineConfig.rootSignature);

	D3D12_BLEND_DESC blend = CreateBlendDesc(pipelineConfig.blend);

	D3D12_DEPTH_STENCIL_DESC depth = CreateDepthDesc(pipelineConfig.depth);

	//InputElementDescを関数内で生成するとPSO作成前に無効ポインタになるため、ここで生成して渡す
	std::vector<D3D12_INPUT_ELEMENT_DESC> inputElementDescs = {};
	D3D12_INPUT_LAYOUT_DESC inputLayout = CreateInputLayout(pipelineConfig.inputLayout, inputElementDescs);

	D3D12_RASTERIZER_DESC rasterizer = CreateRasterizerDesc(pipelineConfig.rasterizer);

	//Shaderをコンパイルする
	ComPtr<IDxcBlob> vertexShaderBlob = dxCommon->CompilerShader(pipelineConfig.vsFilePath, L"vs_6_0");
	ComPtr<IDxcBlob> pixelShaderBlob = dxCommon->CompilerShader(pipelineConfig.psFilePath, L"ps_6_0");

	assert(vertexShaderBlob);
	assert(pixelShaderBlob);

	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc{};

	psoDesc.pRootSignature = rootSig.Get();	
	psoDesc.BlendState = blend;				
	psoDesc.DepthStencilState = depth;		
	psoDesc.InputLayout = inputLayout;
	psoDesc.RasterizerState = rasterizer;
	psoDesc.VS = { vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize() };	//VertexShader
	psoDesc.PS = { pixelShaderBlob->GetBufferPointer(), pixelShaderBlob->GetBufferSize() };		//PixelShader

	//ジオメトリシェーダーは必須ではないので、ファイルパスが空でないときだけコンパイルして設定する
	ComPtr<IDxcBlob> GeometryShaderBlob;
	if (pipelineConfig.gsFilePath != L"")
	{
		GeometryShaderBlob = dxCommon->CompilerShader(pipelineConfig.gsFilePath, L"gs_6_0");
		assert(GeometryShaderBlob);
		psoDesc.GS = { GeometryShaderBlob->GetBufferPointer(), GeometryShaderBlob->GetBufferSize() };
	}

	psoDesc.PrimitiveTopologyType = pipelineConfig.topology;
	//どのように画面に色を打ち込むかの設定
	psoDesc.SampleDesc.Count = 1;
	psoDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	psoDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	//書き込むRTVの情報
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

	//パイプラインセットの作成
	std::unique_ptr<PipelineSet> pipelineSet = std::make_unique<PipelineSet>();
	pipelineSet->rootSignature = rootSig;

	//実際に生成
	HRESULT hr = device->CreateGraphicsPipelineState(&psoDesc,IID_PPV_ARGS(&pipelineSet->graphicsPipelineState));
	assert(SUCCEEDED(hr));

	return pipelineSet;

}

ComPtr<ID3D12RootSignature> PipelineManager::CreateRootSignature(ID3D12Device* device, RootSignatureType type)
{

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

	switch (type)
	{
	case PipelineManager::RootSignatureType::Model:
	{

		rootParameters.resize(static_cast<size_t>(ModelRootParam::kCount));

		//マテリアル
		D3D12_ROOT_PARAMETER& materialParam = rootParameters[static_cast<size_t>(ModelRootParam::kMaterial)];
		materialParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//CBVを使う
		materialParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	//PixelShaderで使う
		materialParam.Descriptor.ShaderRegister = 0;	//レジスタ番号0とバインド

		//TransformationMatrix
		D3D12_ROOT_PARAMETER& transformParam = rootParameters[static_cast<size_t>(ModelRootParam::kTransformationMatrix)];
		transformParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//CBVを使う
		transformParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;	//VertexShaderで使う
		transformParam.Descriptor.ShaderRegister = 0;	//レジスタ番号0を使う

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

	case PipelineManager::RootSignatureType::Sprite:
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

	case PipelineManager::RootSignatureType::Particle:
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

	case PipelineManager::RootSignatureType::InstancingModel:
	{

		rootParameters.resize(static_cast<size_t>(ModelRootParam::kCount));

		//マテリアル
		D3D12_ROOT_PARAMETER& materialParam = rootParameters[static_cast<size_t>(ModelRootParam::kMaterial)];
		materialParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//CBVを使う
		materialParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	//PixelShaderで使う
		materialParam.Descriptor.ShaderRegister = 0;	//レジスタ番号0とバインド

		//TransformationMatrix
		D3D12_ROOT_PARAMETER& transformParam = rootParameters[static_cast<size_t>(ModelRootParam::kTransformationMatrix)];
		transformParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//DescriptorTableを使う
		transformParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;	//VertexShaderで使う
		transformParam.DescriptorTable.pDescriptorRanges = descriptorRange;
		transformParam.DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);

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

	case PipelineManager::RootSignatureType::PostEffect:
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

	case PipelineManager::RootSignatureType::Line:
	{

		rootParameters.resize(static_cast<size_t>(DebudLineRootParam::kCount));

		//WVP
		D3D12_ROOT_PARAMETER& wvpParam = rootParameters[static_cast<size_t>(DebudLineRootParam::kWVP)];
		wvpParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//CBVを使う
		wvpParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//VSとGSで使う
		wvpParam.Descriptor.ShaderRegister = 0;	//レジスタ番号0を使う

		break;
	}

	case PipelineManager::RootSignatureType::Sphere:
	{

		rootParameters.resize(static_cast<size_t>(DebugSphereRootParam::kCount));

		//WVP
		D3D12_ROOT_PARAMETER& wvpParam = rootParameters[static_cast<size_t>(DebugSphereRootParam::kWVP)];
		wvpParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//CBVを使う
		wvpParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//VSとGSで使う
		wvpParam.Descriptor.ShaderRegister = 0;	//レジスタ番号0を使う

		break;
	}

	case PipelineManager::RootSignatureType::SkinModel:
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
	case PipelineManager::RootSignatureType::Skybox:
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

	case PipelineManager::RootSignatureType::Random:
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

	case PipelineManager::RootSignatureType::Outline:
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

	case PipelineManager::RootSignatureType::Dissolve:
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
	default:
		break;
	}

	descriptionRootSignature.pParameters = rootParameters.data();	//ルートパラメータ配列へのポインタ
	descriptionRootSignature.NumParameters = static_cast<UINT>(rootParameters.size());	//配列の長さ

	//Samplerの設定
	std::vector<D3D12_STATIC_SAMPLER_DESC> staticSamplers = {};

	switch (type)
	{
	case PipelineManager::RootSignatureType::Model:
	case PipelineManager::RootSignatureType::Sprite:
	case PipelineManager::RootSignatureType::Particle:
	case PipelineManager::RootSignatureType::InstancingModel:
	case PipelineManager::RootSignatureType::PostEffect:
	case PipelineManager::RootSignatureType::SkinModel:
	case PipelineManager::RootSignatureType::Skybox:
	case PipelineManager::RootSignatureType::Random:
	case PipelineManager::RootSignatureType::Dissolve:

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

	case PipelineManager::RootSignatureType::Line:
	case PipelineManager::RootSignatureType::Sphere:

		break;
	
	
	case PipelineManager::RootSignatureType::Outline:

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
		break;
	}

	descriptionRootSignature.pStaticSamplers = staticSamplers.data();
	descriptionRootSignature.NumStaticSamplers = static_cast<UINT>(staticSamplers.size());

	//シリアナイズしてバイナリにする
	ComPtr<ID3DBlob> signatureBlob = nullptr;
	ComPtr<ID3DBlob> errorBlob = nullptr;
	HRESULT hr = D3D12SerializeRootSignature(&descriptionRootSignature,
		D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
	if (FAILED(hr)) {
		Logger::Log(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
		assert(false);
	}

	//rootSignatureを初期化
	ComPtr<ID3D12RootSignature> rootSignature = nullptr;

	//バイナリを元に生成
	hr = device->CreateRootSignature(0, signatureBlob->GetBufferPointer(),
		signatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(hr));

	return rootSignature;
}

D3D12_BLEND_DESC PipelineManager::CreateBlendDesc(BlendType type)
{
	//BlendStateの設定
	D3D12_BLEND_DESC blendDesc{};
	//すべての色要素を書き込む
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;

	switch (type)
	{
	case PipelineManager::BlendType::None:
		break;

	case PipelineManager::BlendType::Normal:
		blendDesc.RenderTarget[0].BlendEnable = TRUE;
		blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

		break;

	case PipelineManager::BlendType::Add:
		blendDesc.RenderTarget[0].BlendEnable = TRUE;
		blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;

		break;

	case PipelineManager::BlendType::Subtract:
		blendDesc.RenderTarget[0].BlendEnable = TRUE;
		blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;
		blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;

		break;

	case PipelineManager::BlendType::Multiply:
		blendDesc.RenderTarget[0].BlendEnable = TRUE;
		blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_SRC_COLOR;

		break;
	case PipelineManager::BlendType::Screen:
		blendDesc.RenderTarget[0].BlendEnable = TRUE;
		blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_INV_DEST_COLOR;
		blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;

		break;
	default:
		break;
	}

	return blendDesc;
}

D3D12_DEPTH_STENCIL_DESC PipelineManager::CreateDepthDesc(DepthType type)
{
	//DepthStencilStateの設定
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};


	switch (type)
	{
	case PipelineManager::DepthType::EnableWrite:
		//Depthの機能を有効化する
		depthStencilDesc.DepthEnable = true;
		//書き込みします
		depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;

		break;

	case PipelineManager::DepthType::EnableNoWrite:
		//Depthの機能を有効化する
		depthStencilDesc.DepthEnable = true;
		//書き込みしない
		depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;

		break;

	case PipelineManager::DepthType::NoEnableWrite:
		//Depthの機能を無効化する
		depthStencilDesc.DepthEnable = false;
		//書き込みします
		depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;

		break;

	case PipelineManager::DepthType::NoStencil:
		//Depthの機能を無効化する
		depthStencilDesc.DepthEnable = false;
		//書き込みしない
		depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
		depthStencilDesc.StencilEnable = false;	//ステンシルテスト無効化

		break;

	case PipelineManager::DepthType::Disable:
		//Depthの機能を無効化する
		depthStencilDesc.DepthEnable = false;
		//書き込みしない
		depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;

		break;
	default:
		break;
	}

	//比較関数はLessEqual。つまり、近ければ描画される
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;

	return depthStencilDesc;
}

D3D12_INPUT_LAYOUT_DESC PipelineManager::CreateInputLayout(InputLayoutType type, std::vector<D3D12_INPUT_ELEMENT_DESC>& inputElementDescs)
{
	//InputLayout
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};

	switch (type)
	{
	case PipelineManager::InputLayoutType::None:

		//InputLayoutは使わない
		inputLayoutDesc.pInputElementDescs = nullptr;
		inputLayoutDesc.NumElements = 0;

		//InputLayoutを使わないので、ここで返す
		return inputLayoutDesc;

		break;
	case PipelineManager::InputLayoutType::Default:
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

	case PipelineManager::InputLayoutType::GeometryShader:
		inputElementDescs.resize(1);

		inputElementDescs[0].SemanticName = "POSITION";
		inputElementDescs[0].SemanticIndex = 0;
		inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

		break;

	case PipelineManager::InputLayoutType::SkinModel:
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
		break;
	}

	inputLayoutDesc.pInputElementDescs = inputElementDescs.data();
	inputLayoutDesc.NumElements = static_cast<UINT>(inputElementDescs.size());

	return inputLayoutDesc;
}

D3D12_RASTERIZER_DESC PipelineManager::CreateRasterizerDesc(RasterizerType type)
{
	//ResiterizerStateの設定
	D3D12_RASTERIZER_DESC rasterizerDesc{};

	switch (type)
	{
	case PipelineManager::RasterizerType::CullBack:
		//裏面（時計回り）を表示しない
		rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;

		break;

	case PipelineManager::RasterizerType::CullNone:
		//裏面（時計回り）を表示する
		rasterizerDesc.CullMode = D3D12_CULL_MODE_NONE;

		break;

	default:
		break;
	}

	//三角形の中を塗りつぶす
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;

	return rasterizerDesc;
}

D3D12_DESCRIPTOR_RANGE PipelineManager::CreateDescriptorRange(uint32_t BaseShaderRegister)
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
