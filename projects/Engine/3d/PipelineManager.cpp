#include "dx12.h"
#include "PipelineManager.h"
#include <cassert>
#include "DirectXCommon.h"
#include "RootParams.h"

using namespace YKEngine;
using namespace Microsoft::WRL;

const std::unordered_map<PipelineManager::StaticSamplerType, std::vector<PipelineManager::StaticSamplerConfig>> PipelineManager::staticSamplerTable_ =
{
	{
		PipelineManager::StaticSamplerType::Default,
		{
			{ D3D12_FILTER_MIN_MAG_MIP_LINEAR, 0 },
		}
	},
	{
		PipelineManager::StaticSamplerType::Outline,
		{
			{ D3D12_FILTER_MIN_MAG_MIP_LINEAR, 0 },
			{ D3D12_FILTER_MIN_MAG_MIP_POINT, 1 }
		}
	},
	{
		PipelineManager::StaticSamplerType::GeometryShader,
		{
			//使用しないため、空のベクター
		}
	},
};

const std::unordered_map<PipelineManager::BlendType, PipelineManager::BlendConfig> PipelineManager::blendTable_ =
{
	{
		PipelineManager::BlendType::None,
		{
			FALSE,
			D3D12_BLEND_ONE,
			D3D12_BLEND_ZERO,
			D3D12_BLEND_OP_ADD,
		}
	},
	{
		PipelineManager::BlendType::Normal,
		{
			TRUE,
			D3D12_BLEND_SRC_ALPHA,
			D3D12_BLEND_INV_SRC_ALPHA,
			D3D12_BLEND_OP_ADD,
		}
	},
	{
		PipelineManager::BlendType::Add,
		{
			TRUE,
			D3D12_BLEND_SRC_ALPHA,
			D3D12_BLEND_ONE,
			D3D12_BLEND_OP_ADD,
		}
	},
	{
		PipelineManager::BlendType::Subtract,
		{
			TRUE,
			D3D12_BLEND_SRC_ALPHA,
			D3D12_BLEND_ONE,
			D3D12_BLEND_OP_REV_SUBTRACT,
		}
	},
	{	
		PipelineManager::BlendType::Multiply,
		{
			TRUE,
			D3D12_BLEND_ZERO,
			D3D12_BLEND_SRC_COLOR,
			D3D12_BLEND_OP_ADD,
		}
	
	},
	{
		PipelineManager::BlendType::Screen,
		{
			TRUE,
			D3D12_BLEND_INV_DEST_COLOR,
			D3D12_BLEND_ONE,
			D3D12_BLEND_OP_ADD,
		}
	},
};

const std::unordered_map<PipelineManager::DepthType, PipelineManager::DepthConfig> PipelineManager::depthTable_ =
{
	{
		PipelineManager::DepthType::EnableWrite,
		{
			TRUE,
			D3D12_DEPTH_WRITE_MASK_ALL,
		}
	},
	{
		PipelineManager::DepthType::EnableNoWrite,
		{
			TRUE,
			D3D12_DEPTH_WRITE_MASK_ZERO,
		}
	},
	{
		PipelineManager::DepthType::NoEnableWrite,
		{
			FALSE,
			D3D12_DEPTH_WRITE_MASK_ALL,
		}
	},
	{
		PipelineManager::DepthType::Disable,
		{
			FALSE,
			D3D12_DEPTH_WRITE_MASK_ZERO,
		}
	},
};

const std::unordered_map<PipelineManager::RasterizerType, PipelineManager::RasterizerConfig> PipelineManager::rasterizerTable_ =
{
	{
		PipelineManager::RasterizerType::CullBack,
		{
			D3D12_CULL_MODE_BACK,
		}
	},
	{
		PipelineManager::RasterizerType::CullNone,
		{
			D3D12_CULL_MODE_NONE,
		}
	},
};

const std::unordered_map<PipelineManager::InputLayoutType, PipelineManager::InputLayoutConfig> PipelineManager::inputLayoutTable_ =
{
	{
		InputLayoutType::None,
		{
			{
				//入力レイアウトなし
			},
		}
	},

	{
		InputLayoutType::Default,
		{
			{
				{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
				 D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },

				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
				  D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },

				{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
				  D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
				
			},
		}
	},

	{
		InputLayoutType::GeometryShader,
		{
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
				  D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
			},
		}
	},

	{
		InputLayoutType::SkinModel,
		{
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
				  D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },

				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
				  D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },

				{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
				  D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },

				{ "WEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D12_APPEND_ALIGNED_ELEMENT,
				  D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },

				{ "INDEX", 0, DXGI_FORMAT_R32G32B32A32_SINT, 1, D3D12_APPEND_ALIGNED_ELEMENT,
				  D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
			},
		}
	}
};

const std::unordered_map<DrawMode, PipelineManager::PipelineConfig> PipelineManager::pipelineTable_ =
{
	{
		DrawMode::kBlendModeNone,
		{
			L"resources/shader/Object3d.VS.hlsl",
			L"resources/shader/Object3d.PS.hlsl",
			L"",
			RootParameterType::Model,
			StaticSamplerType::Default,
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
			RootParameterType::Model,
			StaticSamplerType::Default,
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
			RootParameterType::Model,
			StaticSamplerType::Default,
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
			RootParameterType::Model,
			StaticSamplerType::Default,
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
			RootParameterType::Model,
			StaticSamplerType::Default,
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
			RootParameterType::Model,
			StaticSamplerType::Default,
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
			RootParameterType::Sprite,
			StaticSamplerType::Default,
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
			RootParameterType::Sprite,
			StaticSamplerType::Default,
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
			RootParameterType::Sprite,
			StaticSamplerType::Default,
			InputLayoutType::Default,
			BlendType::None,
			RasterizerType::CullNone,
			DepthType::Disable,
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE
		}
	},

	{
		DrawMode::kBlendModeAddParticle,
		{
			L"resources/shader/Particle.VS.hlsl",
			L"resources/shader/Particle.PS.hlsl",
			L"",
			RootParameterType::Particle,
			StaticSamplerType::Default,
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
			RootParameterType::Particle,
			StaticSamplerType::Default,
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
			RootParameterType::Particle,
			StaticSamplerType::Default,
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
			RootParameterType::Particle,
			StaticSamplerType::Default,
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
			RootParameterType::Line,
			StaticSamplerType::GeometryShader,
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
			RootParameterType::Sphere,
			StaticSamplerType::GeometryShader,
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
			RootParameterType::SkinModel,
			StaticSamplerType::Default,
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
			RootParameterType::InstancingModel,
			StaticSamplerType::Default,
			InputLayoutType::Default,
			BlendType::Normal,
			RasterizerType::CullBack,
			DepthType::EnableWrite,
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE
		}
	},

	{
		DrawMode::kInstancingTriplanar,
		{
			L"resources/shader/InstancingObject3d.VS.hlsl",
			L"resources/shader/TriplanarObject3d.PS.hlsl",
			L"",
			RootParameterType::InstancingModel,
			StaticSamplerType::Default,
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
			RootParameterType::Skybox,
			StaticSamplerType::Default,
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
			RootParameterType::PostEffect,
			StaticSamplerType::Default,
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
			RootParameterType::PostEffect,
			StaticSamplerType::Default,
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
			RootParameterType::PostEffect,
			StaticSamplerType::Default,
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
			RootParameterType::PostEffect,
			StaticSamplerType::Default,
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
			RootParameterType::PostEffect,
			StaticSamplerType::Default,
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
			RootParameterType::PostEffect,
			StaticSamplerType::Default,
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
			RootParameterType::Outline,
			StaticSamplerType::Outline,
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
			RootParameterType::PostEffect,
			StaticSamplerType::Default,
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
			RootParameterType::Dissolve,
			StaticSamplerType::Default,
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
			RootParameterType::Random,
			StaticSamplerType::Default,
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
	//DescriptorRangeの生成
	CreateDescriptorRanges();

	//DrawMode分のパイプラインを作成
	for (const auto& [drawMode, pipelineConfig] : pipelineTable_)
	{
		pipelineSets_.at(static_cast<uint16_t>(drawMode)) = CreateGraphicsPipeline(pipelineConfig, dxCommon);
	}

}

std::unique_ptr<PipelineManager::PipelineSet> PipelineManager::CreateGraphicsPipeline(const PipelineConfig& pipelineConfig, DirectXCommon* dxCommon)
{
	
	ID3D12Device* device = dxCommon->GetDevice();

	ComPtr<ID3D12RootSignature> rootSig = CreateRootSignature(device, pipelineConfig.rootParameter, pipelineConfig.staticSampler);

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

ComPtr<ID3D12RootSignature> PipelineManager::CreateRootSignature(ID3D12Device* device, RootParameterType paramType, StaticSamplerType staticSamplerType)
{

	//RootSignature作成
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	descriptionRootSignature.Flags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	std::vector<D3D12_ROOT_PARAMETER> rootParameters = {};
	
	
	// テーブル取得
	const auto& table = GetRootParameterBuilders();

	//検索	
	auto it = table.find(paramType);
	if (it == table.end())
	{
		assert(false);
	}

	//ルートパラメータ生成
	(this->*(it->second))(rootParameters);
	
	descriptionRootSignature.pParameters = rootParameters.data();	//ルートパラメータ配列へのポインタ
	descriptionRootSignature.NumParameters = static_cast<UINT>(rootParameters.size());	//配列の長さ

	//Samplerの設定
	std::vector<D3D12_STATIC_SAMPLER_DESC> staticSamplers = {};

	auto samplerIt = staticSamplerTable_.find(staticSamplerType);
	if (samplerIt != staticSamplerTable_.end())
	{
		for (const StaticSamplerConfig& config : samplerIt->second)
		{
			staticSamplers.push_back(CreateStaticSamplerDesc(config));
		}
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

D3D12_STATIC_SAMPLER_DESC YKEngine::PipelineManager::CreateStaticSamplerDesc(StaticSamplerConfig config)
{

	D3D12_STATIC_SAMPLER_DESC samplerDesc{};

	samplerDesc.Filter = config.filter;
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;	//0～1の範囲外をリピート
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;	//比較しない
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;	//ありったけのMipmapを使う
	samplerDesc.ShaderRegister = config.reg;	//レジスタ番号の指定 s0など
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	//PixelShaderで使う

	
	return samplerDesc;
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

	BlendConfig config = blendTable_.at(type);

	blendDesc.RenderTarget[0].BlendEnable = config.enable;
	blendDesc.RenderTarget[0].SrcBlend = config.srcBlend;
	blendDesc.RenderTarget[0].DestBlend = config.destBlend;
	blendDesc.RenderTarget[0].BlendOp = config.blendOp;
	

	return blendDesc;
}

D3D12_DEPTH_STENCIL_DESC PipelineManager::CreateDepthDesc(DepthType type)
{
	//DepthStencilStateの設定
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};

	DepthConfig config = depthTable_.at(type);

	depthStencilDesc.DepthEnable = config.depthEnable;
	depthStencilDesc.DepthWriteMask = config.depthWriteMask;

	//比較関数はLessEqual。つまり、近ければ描画される
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;

	return depthStencilDesc;
}

D3D12_INPUT_LAYOUT_DESC PipelineManager::CreateInputLayout(InputLayoutType type, std::vector<D3D12_INPUT_ELEMENT_DESC>& inputElementDescs)
{
	//InputLayout
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};

	inputElementDescs = inputLayoutTable_.at(type).inputElementDescs;

	if (inputElementDescs.empty())
	{
		//InputLayoutは使わない
		inputLayoutDesc.pInputElementDescs = nullptr;
		inputLayoutDesc.NumElements = 0;
		return inputLayoutDesc;
	}

	inputLayoutDesc.pInputElementDescs = inputElementDescs.data();
	inputLayoutDesc.NumElements = static_cast<UINT>(inputElementDescs.size());

	return inputLayoutDesc;
}

D3D12_RASTERIZER_DESC PipelineManager::CreateRasterizerDesc(RasterizerType type)
{
	//RasterizerStateの設定
	D3D12_RASTERIZER_DESC rasterizerDesc{};

	RasterizerConfig config = rasterizerTable_.at(type);

	rasterizerDesc.CullMode = config.cullMode;

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

void PipelineManager::BuildModelRootParameters(std::vector<D3D12_ROOT_PARAMETER>& rootParameters)
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
	textureParam.DescriptorTable.pDescriptorRanges = &descriptorRange_;	//Tableの中身の配列を指定
	textureParam.DescriptorTable.NumDescriptorRanges = 1;	//Tableで利用する数

	//平行光源
	D3D12_ROOT_PARAMETER& directionalLightParam = rootParameters[static_cast<size_t>(ModelRootParam::kDirectionalLight)];
	directionalLightParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//DescriptorTableを使う
	directionalLightParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	//PixelShaderで使う
	directionalLightParam.DescriptorTable.pDescriptorRanges = &descriptorRangeDirectionalLight_;	//Tableの中身の配列を指定
	directionalLightParam.DescriptorTable.NumDescriptorRanges = 1;	//Tableで利用する数

	//カメラ
	D3D12_ROOT_PARAMETER& cameraParam = rootParameters[static_cast<size_t>(ModelRootParam::kCamera)];
	cameraParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//CBVを使う
	cameraParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	//PixelShaderで使う
	cameraParam.Descriptor.ShaderRegister = 1;

	//点光源
	D3D12_ROOT_PARAMETER& pointLightParam = rootParameters[static_cast<size_t>(ModelRootParam::kPointLight)];
	pointLightParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//DescriptorTableを使う
	pointLightParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	//PixelShaderで使う
	pointLightParam.DescriptorTable.pDescriptorRanges = &descriptorRangePointLight_;	//Tableの中身の配列を指定
	pointLightParam.DescriptorTable.NumDescriptorRanges = 1;	//Tableで利用する数

	//スポットライト
	D3D12_ROOT_PARAMETER& spotLightParam = rootParameters[static_cast<size_t>(ModelRootParam::kSpotLight)];
	spotLightParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//DescriptorTableを使う
	spotLightParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	//PixelShaderで使う
	spotLightParam.DescriptorTable.pDescriptorRanges = &descriptorRangeSpotLight_;	//Tableの中身の配列を指定
	spotLightParam.DescriptorTable.NumDescriptorRanges = 1;	//Tableで利用する数

	//ライトカウント
	D3D12_ROOT_PARAMETER& lightCountParam = rootParameters[static_cast<size_t>(ModelRootParam::kLightCount)];
	lightCountParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//CBVを使う
	lightCountParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	//PixelShaderで使う
	lightCountParam.Descriptor.ShaderRegister = 2;

	//環境マップ
	D3D12_ROOT_PARAMETER& environmentMapParam = rootParameters[static_cast<size_t>(ModelRootParam::kEnvironmentMap)];
	environmentMapParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//DescriptorTableを使う
	environmentMapParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	//PixelShaderで使う
	environmentMapParam.DescriptorTable.pDescriptorRanges = &descriptorRangeEnvironmentTexture_;	//Tableの中身の配列を指定
	environmentMapParam.DescriptorTable.NumDescriptorRanges = 1;	//Tableで利用する数

}

void PipelineManager::BuildSpriteRootParameters(std::vector<D3D12_ROOT_PARAMETER>& rootParameters)
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
	textureParam.DescriptorTable.pDescriptorRanges = &descriptorRange_;	//Tableの中身の配列を指定
	textureParam.DescriptorTable.NumDescriptorRanges = 1;	//Tableで利用する数
}

void PipelineManager::BuildParticleRootParameters(std::vector<D3D12_ROOT_PARAMETER>& rootParameters)
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
	particleParam.DescriptorTable.pDescriptorRanges = &descriptorRange_;	//Tableの中身の配列を指定
	particleParam.DescriptorTable.NumDescriptorRanges = 1;	//Tableで利用する数

	//テクスチャ
	D3D12_ROOT_PARAMETER& textureParam = rootParameters[static_cast<size_t>(ParticleRootParam::kTexture)];
	textureParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//DescriptorTableを使う
	textureParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	//PixelShaderで使う
	textureParam.DescriptorTable.pDescriptorRanges = &descriptorRange_;	//Tableの中身の配列を指定
	textureParam.DescriptorTable.NumDescriptorRanges = 1;	//Tableで利用する数
}

void PipelineManager::BuildInstancingModelRootParameters(std::vector<D3D12_ROOT_PARAMETER>& rootParameters)
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
	transformParam.DescriptorTable.pDescriptorRanges = &descriptorRange_;
	transformParam.DescriptorTable.NumDescriptorRanges = 1;

	//テクスチャ
	D3D12_ROOT_PARAMETER& textureParam = rootParameters[static_cast<size_t>(ModelRootParam::kTexture)];
	textureParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//DescriptorTableを使う
	textureParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	//PixelShaderで使う
	textureParam.DescriptorTable.pDescriptorRanges = &descriptorRange_;	//Tableの中身の配列を指定
	textureParam.DescriptorTable.NumDescriptorRanges = 1;	//Tableで利用する数

	//平行光源
	D3D12_ROOT_PARAMETER& directionalLightParam = rootParameters[static_cast<size_t>(ModelRootParam::kDirectionalLight)];
	directionalLightParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//DescriptorTableを使う
	directionalLightParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	//PixelShaderで使う
	directionalLightParam.DescriptorTable.pDescriptorRanges = &descriptorRangeDirectionalLight_;	//Tableの中身の配列を指定
	directionalLightParam.DescriptorTable.NumDescriptorRanges = 1;	//Tableで利用する数

	//カメラ
	D3D12_ROOT_PARAMETER& cameraParam = rootParameters[static_cast<size_t>(ModelRootParam::kCamera)];
	cameraParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//CBVを使う
	cameraParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	//PixelShaderで使う
	cameraParam.Descriptor.ShaderRegister = 1;

	//点光源
	D3D12_ROOT_PARAMETER& pointLightParam = rootParameters[static_cast<size_t>(ModelRootParam::kPointLight)];
	pointLightParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//DescriptorTableを使う
	pointLightParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	//PixelShaderで使う
	pointLightParam.DescriptorTable.pDescriptorRanges = &descriptorRangePointLight_;	//Tableの中身の配列を指定
	pointLightParam.DescriptorTable.NumDescriptorRanges = 1;	//Tableで利用する数

	//スポットライト
	D3D12_ROOT_PARAMETER& spotLightParam = rootParameters[static_cast<size_t>(ModelRootParam::kSpotLight)];
	spotLightParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//DescriptorTableを使う
	spotLightParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	//PixelShaderで使う
	spotLightParam.DescriptorTable.pDescriptorRanges = &descriptorRangeSpotLight_;	//Tableの中身の配列を指定
	spotLightParam.DescriptorTable.NumDescriptorRanges = 1;	//Tableで利用する数

	//ライトカウント
	D3D12_ROOT_PARAMETER& lightCountParam = rootParameters[static_cast<size_t>(ModelRootParam::kLightCount)];
	lightCountParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//CBVを使う
	lightCountParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	//PixelShaderで使う
	lightCountParam.Descriptor.ShaderRegister = 2;

	//環境マップ
	D3D12_ROOT_PARAMETER& environmentMapParam = rootParameters[static_cast<size_t>(ModelRootParam::kEnvironmentMap)];
	environmentMapParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//DescriptorTableを使う
	environmentMapParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	//PixelShaderで使う
	environmentMapParam.DescriptorTable.pDescriptorRanges = &descriptorRangeEnvironmentTexture_;	//Tableの中身の配列を指定
	environmentMapParam.DescriptorTable.NumDescriptorRanges = 1;	//Tableで利用する数
}

void PipelineManager::BuildPostEffectRootParameters(std::vector<D3D12_ROOT_PARAMETER>& rootParameters)
{
	

	rootParameters.resize(static_cast<size_t>(PostEffectRootParam::kCount));

	//テクスチャ
	D3D12_ROOT_PARAMETER& textureParam = rootParameters[static_cast<size_t>(PostEffectRootParam::kTexture)];
	textureParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//DescriptorTableを使う
	textureParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	//PixelShaderで使う
	textureParam.DescriptorTable.pDescriptorRanges = &descriptorRange_;	//Tableの中身の配列を指定
	textureParam.DescriptorTable.NumDescriptorRanges = 1;	//Tableで利用する数
}

void PipelineManager::BuildLineRootParameters(std::vector<D3D12_ROOT_PARAMETER>& rootParameters)
{
	rootParameters.resize(static_cast<size_t>(DebugLineRootParam::kCount));

	//WVP
	D3D12_ROOT_PARAMETER& wvpParam = rootParameters[static_cast<size_t>(DebugLineRootParam::kWVP)];
	wvpParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//CBVを使う
	wvpParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//VSとGSで使う
	wvpParam.Descriptor.ShaderRegister = 0;	//レジスタ番号0を使う

}

void PipelineManager::BuildSphereRootParameters(std::vector<D3D12_ROOT_PARAMETER>& rootParameters)
{
	rootParameters.resize(static_cast<size_t>(DebugSphereRootParam::kCount));

	//WVP
	D3D12_ROOT_PARAMETER& wvpParam = rootParameters[static_cast<size_t>(DebugSphereRootParam::kWVP)];
	wvpParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//CBVを使う
	wvpParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//VSとGSで使う
	wvpParam.Descriptor.ShaderRegister = 0;	//レジスタ番号0を使う
}

void PipelineManager::BuildSkinModelRootParameters(std::vector<D3D12_ROOT_PARAMETER>& rootParameters)
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
	textureParam.DescriptorTable.pDescriptorRanges = &descriptorRange_;	//Tableの中身の配列を指定
	textureParam.DescriptorTable.NumDescriptorRanges = 1;	//Tableで利用する数

	//平行光源
	D3D12_ROOT_PARAMETER& directionalLightParam = rootParameters[static_cast<size_t>(SkinModelRootParam::kDirectionalLight)];
	directionalLightParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//DescriptorTableを使う
	directionalLightParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	//PixelShaderで使う
	directionalLightParam.DescriptorTable.pDescriptorRanges = &descriptorRangeDirectionalLight_;	//Tableの中身の配列を指定
	directionalLightParam.DescriptorTable.NumDescriptorRanges = 1;	//Tableで利用する数

	//カメラ
	D3D12_ROOT_PARAMETER& cameraParam = rootParameters[static_cast<size_t>(SkinModelRootParam::kCamera)];
	cameraParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//CBVを使う
	cameraParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	//PixelShaderで使う
	cameraParam.Descriptor.ShaderRegister = 1;

	//点光源
	D3D12_ROOT_PARAMETER& pointLightParam = rootParameters[static_cast<size_t>(SkinModelRootParam::kPointLight)];
	pointLightParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//DescriptorTableを使う
	pointLightParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	//PixelShaderで使う
	pointLightParam.DescriptorTable.pDescriptorRanges = &descriptorRangePointLight_;	//Tableの中身の配列を指定
	pointLightParam.DescriptorTable.NumDescriptorRanges = 1;	//Tableで利用する数

	//スポットライト
	D3D12_ROOT_PARAMETER& spotLightParam = rootParameters[static_cast<size_t>(SkinModelRootParam::kSpotLight)];
	spotLightParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//DescriptorTableを使う
	spotLightParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	//PixelShaderで使う
	spotLightParam.DescriptorTable.pDescriptorRanges = &descriptorRangeSpotLight_;	//Tableの中身の配列を指定
	spotLightParam.DescriptorTable.NumDescriptorRanges = 1;	//Tableで利用する数

	//ライトカウント
	D3D12_ROOT_PARAMETER& lightCountParam = rootParameters[static_cast<size_t>(SkinModelRootParam::kLightCount)];
	lightCountParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//CBVを使う
	lightCountParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	//PixelShaderで使う
	lightCountParam.Descriptor.ShaderRegister = 2;

	//環境マップ
	D3D12_ROOT_PARAMETER& environmentMapParam = rootParameters[static_cast<size_t>(SkinModelRootParam::kEnvironmentMap)];
	environmentMapParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//DescriptorTableを使う
	environmentMapParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	//PixelShaderで使う
	environmentMapParam.DescriptorTable.pDescriptorRanges = &descriptorRangeEnvironmentTexture_;	//Tableの中身の配列を指定
	environmentMapParam.DescriptorTable.NumDescriptorRanges = 1;	//Tableで利用する数

	//Well
	D3D12_ROOT_PARAMETER& wellParam = rootParameters[static_cast<size_t>(SkinModelRootParam::kWell)];
	wellParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//DescriptorTableを使う
	wellParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;	//VertexShaderで使う
	wellParam.DescriptorTable.pDescriptorRanges = &descriptorRange_;	//Tableの中身の配列を指定
	wellParam.DescriptorTable.NumDescriptorRanges = 1;	//Tableで利用する数
}

void PipelineManager::BuildSkyboxRootParameters(std::vector<D3D12_ROOT_PARAMETER>& rootParameters)
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
	textureParam.DescriptorTable.pDescriptorRanges = &descriptorRange_;	//Tableの中身の配列を指定
	textureParam.DescriptorTable.NumDescriptorRanges = 1;	//Tableで利用する数
}

void PipelineManager::BuildRandomRootParameters(std::vector<D3D12_ROOT_PARAMETER>& rootParameters)
{
	

	rootParameters.resize(static_cast<size_t>(RandomRootParam::kCount));

	//テクスチャ
	D3D12_ROOT_PARAMETER& textureParam = rootParameters[static_cast<size_t>(RandomRootParam::kTexture)];
	textureParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//DescriptorTableを使う
	textureParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	//PixelShaderで使う
	textureParam.DescriptorTable.pDescriptorRanges = &descriptorRange_;	//Tableの中身の配列を指定
	textureParam.DescriptorTable.NumDescriptorRanges = 1;	//Tableで利用する数

	//マテリアル
	D3D12_ROOT_PARAMETER& materialParam = rootParameters[static_cast<size_t>(RandomRootParam::kMaterial)];
	materialParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//CBVを使う
	materialParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	//PixelShaderで使う
	materialParam.Descriptor.ShaderRegister = 0;	//レジスタ番号0とバインド
}

void PipelineManager::BuildOutlineRootParameters(std::vector<D3D12_ROOT_PARAMETER>& rootParameters)
{
	

	rootParameters.resize(static_cast<size_t>(DepthOutlineRootParam::kCount));

	//テクスチャ
	D3D12_ROOT_PARAMETER& textureParam = rootParameters[static_cast<size_t>(DepthOutlineRootParam::kTexture)];
	textureParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//DescriptorTableを使う
	textureParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	//PixelShaderで使う
	textureParam.DescriptorTable.pDescriptorRanges = &descriptorRange_;	//Tableの中身の配列を指定
	textureParam.DescriptorTable.NumDescriptorRanges = 1;	//Tableで利用する数

	//DepthTexture
	D3D12_ROOT_PARAMETER& depthTextureParam = rootParameters[static_cast<size_t>(DepthOutlineRootParam::kDepthTexture)];
	depthTextureParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//DescriptorTableを使う
	depthTextureParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	//PixelShaderで使う
	depthTextureParam.DescriptorTable.pDescriptorRanges = &descriptorRangeDepthTexture_;	//Tableの中身の配列を指定
	depthTextureParam.DescriptorTable.NumDescriptorRanges = 1;	//Tableで利用する数

	//マテリアル
	D3D12_ROOT_PARAMETER& materialParam = rootParameters[static_cast<size_t>(DepthOutlineRootParam::kMaterial)];
	materialParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//CBVを使う
	materialParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	//PixelShaderで使う
	materialParam.Descriptor.ShaderRegister = 0;	//レジスタ番号0とバインド
}

void PipelineManager::BuildDissolveRootParameters(std::vector<D3D12_ROOT_PARAMETER>& rootParameters)
{
	

	rootParameters.resize(static_cast<size_t>(DissolveRootParam::kCount));

	//テクスチャ
	D3D12_ROOT_PARAMETER& textureParam = rootParameters[static_cast<size_t>(DissolveRootParam::kTexture)];
	textureParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//DescriptorTableを使う
	textureParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	//PixelShaderで使う
	textureParam.DescriptorTable.pDescriptorRanges = &descriptorRange_;	//Tableの中身の配列を指定
	textureParam.DescriptorTable.NumDescriptorRanges = 1;	//Tableで利用する数

	//MaskTexture
	D3D12_ROOT_PARAMETER& maskTextureParam = rootParameters[static_cast<size_t>(DissolveRootParam::kMaskTexture)];
	maskTextureParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//DescriptorTableを使う
	maskTextureParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	//PixelShaderで使う
	maskTextureParam.DescriptorTable.pDescriptorRanges = &descriptorMaskTexture_;	//Tableの中身の配列を指定
	maskTextureParam.DescriptorTable.NumDescriptorRanges = 1;	//Tableで利用する数
}

const std::unordered_map<PipelineManager::RootParameterType, void(PipelineManager::*)(std::vector<D3D12_ROOT_PARAMETER>&)>& PipelineManager::GetRootParameterBuilders() const
{
	static const std::unordered_map<RootParameterType, void(PipelineManager::*)(std::vector<D3D12_ROOT_PARAMETER>&)> rootParameterBuilders =
	{
		{
			RootParameterType::Model,
			&PipelineManager::BuildModelRootParameters,
		},
		{
			RootParameterType::Sprite,
			&PipelineManager::BuildSpriteRootParameters,

		},
		{
			RootParameterType::Particle,
			&PipelineManager::BuildParticleRootParameters,
		},
		{
			RootParameterType::InstancingModel,
			&PipelineManager::BuildInstancingModelRootParameters,
		},
		{
			RootParameterType::PostEffect,
			&PipelineManager::BuildPostEffectRootParameters,
		},
		{
			RootParameterType::Line,
			&PipelineManager::BuildLineRootParameters,
		},
		{
			RootParameterType::Sphere,
			&PipelineManager::BuildSphereRootParameters,
		},
		{
			RootParameterType::SkinModel,
			&PipelineManager::BuildSkinModelRootParameters,
		},
		{
			RootParameterType::Skybox,
			&PipelineManager::BuildSkyboxRootParameters,
		},
		{
			RootParameterType::Random,
			&PipelineManager::BuildRandomRootParameters,
		},
		{
			RootParameterType::Outline,
			&PipelineManager::BuildOutlineRootParameters,
		},
		{
			RootParameterType::Dissolve,
			&PipelineManager::BuildDissolveRootParameters,
		}
	};

	return rootParameterBuilders;
}

void PipelineManager::CreateDescriptorRanges()
{
	descriptorRange_ = CreateDescriptorRange(0);	//t0
	descriptorRangeDirectionalLight_ = CreateDescriptorRange(1);	//t1
	descriptorRangePointLight_ = CreateDescriptorRange(2);	//t2
	descriptorRangeSpotLight_ = CreateDescriptorRange(3);	//t3
	descriptorRangeEnvironmentTexture_ = CreateDescriptorRange(4);	//t4
	descriptorRangeDepthTexture_ = CreateDescriptorRange(1);	//t1
	descriptorMaskTexture_ = CreateDescriptorRange(1);	//t1
}

void PipelineManager::SetPipelineSet(ID3D12GraphicsCommandList* commandList, DrawMode blendMode)
{

	commandList->SetGraphicsRootSignature(pipelineSets_.at(static_cast<uint16_t>(blendMode))->rootSignature.Get());
	commandList->SetPipelineState(pipelineSets_.at(static_cast<uint16_t>(blendMode))->graphicsPipelineState.Get());	//PSOを設定

}
