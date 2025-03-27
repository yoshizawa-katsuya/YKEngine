#include "dx12.h"
#include "PrimitiveDrawer.h"
#include <cassert>
#include <format>
#include "DirectXCommon.h"

void PrimitiveDrawer::Initialize(DirectXCommon* dxCommon) {

	pipelineSets_.at(static_cast<uint16_t>(BlendMode::kBlendModeNone)) = CreateGraphicsPipeline(BlendMode::kBlendModeNone, dxCommon);

	pipelineSets_.at(static_cast<uint16_t>(BlendMode::kBlendModeNormal)) = CreateGraphicsPipeline(BlendMode::kBlendModeNormal, dxCommon);

	pipelineSets_.at(static_cast<uint16_t>(BlendMode::kBlendModeAdd)) = CreateGraphicsPipeline(BlendMode::kBlendModeAdd, dxCommon);

	pipelineSets_.at(static_cast<uint16_t>(BlendMode::kBlendModeSubtract)) = CreateGraphicsPipeline(BlendMode::kBlendModeSubtract, dxCommon);
	
	pipelineSets_.at(static_cast<uint16_t>(BlendMode::kBlendModeMultiply)) = CreateGraphicsPipeline(BlendMode::kBlendModeMultiply, dxCommon);

	pipelineSets_.at(static_cast<uint16_t>(BlendMode::kBlendModeScreen)) = CreateGraphicsPipeline(BlendMode::kBlendModeScreen, dxCommon);

	pipelineSets_.at(static_cast<uint16_t>(BlendMode::kBlendModeNoneSprite)) = CreateGraphicsPipeline(BlendMode::kBlendModeNoneSprite, dxCommon);

	pipelineSets_.at(static_cast<uint16_t>(BlendMode::kBlendModeNormalSprite)) = CreateGraphicsPipeline(BlendMode::kBlendModeNormalSprite, dxCommon);

	pipelineSets_.at(static_cast<uint16_t>(BlendMode::kBackGroundSprite)) = CreateGraphicsPipeline(BlendMode::kBackGroundSprite, dxCommon);

	pipelineSets_.at(static_cast<uint16_t>(BlendMode::kBlendModeAddParticle)) = CreateGraphicsPipeline(BlendMode::kBlendModeAddParticle, dxCommon);

	pipelineSets_.at(static_cast<uint16_t>(BlendMode::kLineMode)) = CreateGraphicsPipeline(BlendMode::kLineMode, dxCommon);

	pipelineSets_.at(static_cast<uint16_t>(BlendMode::kSphereMode)) = CreateGraphicsPipeline(BlendMode::kSphereMode, dxCommon);

	pipelineSets_.at(static_cast<uint16_t>(BlendMode::kSkinModelMode)) = CreateGraphicsPipeline(BlendMode::kSkinModelMode, dxCommon);

	pipelineSets_.at(static_cast<uint16_t>(BlendMode::kBlendModeNormalinstancing)) = CreateGraphicsPipeline(BlendMode::kBlendModeNormalinstancing, dxCommon);

}

std::unique_ptr<PrimitiveDrawer::PipelineSet> PrimitiveDrawer::CreateGraphicsPipeline(BlendMode blendMode, DirectXCommon* dxCommon) {

	std::unique_ptr<PipelineSet> pipelineSet = std::make_unique<PipelineSet>();

	

	

	//RootSignature作成
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	descriptionRootSignature.Flags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	//DescriptorRange
	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	descriptorRange[0].BaseShaderRegister = 0;	//0から始まる
	descriptorRange[0].NumDescriptors = 1;	//数は1つ
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;	//SRVを使う
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;	//Offsetを自動計算
	/*
	D3D12_DESCRIPTOR_RANGE descriptorRangeForInstancing[1] = {};
	descriptorRangeForInstancing[0].BaseShaderRegister = 0;	//0から始まる
	descriptorRangeForInstancing[0].NumDescriptors = 1;	//数は1つ
	descriptorRangeForInstancing[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;	//SRVを使う
	descriptorRangeForInstancing[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;	//Offsetを自動計算

	D3D12_DESCRIPTOR_RANGE descriptorRangeSkinning[1] = {};
	if (blendMode == BlendMode::kSkinModelMode) {
		descriptorRangeSkinning[0].BaseShaderRegister = 0;
		descriptorRangeSkinning[0].NumDescriptors = 1;	//数は1つ
		descriptorRangeSkinning[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;	//SRVを使う
		descriptorRangeSkinning[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;	//Offsetを自動計算
	}
	*/
	//RootParameter作成。複数設定できるので配列。
	std::vector<D3D12_ROOT_PARAMETER> rootParameters = {};

	switch (blendMode) {
	case BlendMode::kLineMode:
	case BlendMode::kSphereMode:

		rootParameters.resize(1);

		rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//CBVを使う
		rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//VSとGSで使う
		rootParameters[0].Descriptor.ShaderRegister = 0;	//レジスタ番号0を使う

		break;

	case BlendMode::kSkinModelMode:

		rootParameters.resize(8);

		//マテリアル
		rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//CBVを使う
		rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	//PixelShaderで使う
		rootParameters[0].Descriptor.ShaderRegister = 0;	//レジスタ番号0とバインド

		//TransformationMatrix
		rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//CBVを使う
		rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;	//VertexShaderで使う
		rootParameters[1].Descriptor.ShaderRegister = 0;	//レジスタ番号0を使う
		
		//テクスチャ
		rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//DescriptorTableを使う
		rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	//PixelShaderで使う
		rootParameters[2].DescriptorTable.pDescriptorRanges = descriptorRange;	//Tableの中身の配列を指定
		rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);	//Tableで利用する数

		//平行光源
		rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//CBVを使う
		rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	//PixelShaderで使う
		rootParameters[3].Descriptor.ShaderRegister = 1;	//レジスタ番号1を使う

		//カメラ
		rootParameters[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//CBVを使う
		rootParameters[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	//PixelShaderで使う
		rootParameters[4].Descriptor.ShaderRegister = 2;

		//点光源
		rootParameters[5].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//CBVを使う
		rootParameters[5].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	//PixelShaderで使う
		rootParameters[5].Descriptor.ShaderRegister = 3;

		//スポットライト
		rootParameters[6].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//CBVを使う
		rootParameters[6].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	//PixelShaderで使う
		rootParameters[6].Descriptor.ShaderRegister = 4;

		//Well
		rootParameters[7].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//DescriptorTableを使う
		rootParameters[7].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;	//VertexShaderで使う
		rootParameters[7].DescriptorTable.pDescriptorRanges = descriptorRange;	//Tableの中身の配列を指定
		rootParameters[7].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);	//Tableで利用する数

		break;

	default:
		rootParameters.resize(7);

		//マテリアル
		rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//CBVを使う
		rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	//PixelShaderで使う
		rootParameters[0].Descriptor.ShaderRegister = 0;	//レジスタ番号0とバインド

		switch (blendMode) {
		default:
			//Object3d用
			//TransformationMatrix
			rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//CBVを使う
			rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;	//VertexShaderで使う
			rootParameters[1].Descriptor.ShaderRegister = 0;	//レジスタ番号0を使う
			break;
		case BlendMode::kBlendModeAddParticle:
		case BlendMode::kBlendModeNormalinstancing:
			//Particle用、instancing用
			//ParticleForGPU、TransformationMatrix
			rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//DescriptorTableを使う
			rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;	//VertexShaderで使う
			rootParameters[1].DescriptorTable.pDescriptorRanges = descriptorRange;
			rootParameters[1].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);

			break;
		}

		//テクスチャ
		rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//DescriptorTableを使う
		rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	//PixelShaderで使う
		rootParameters[2].DescriptorTable.pDescriptorRanges = descriptorRange;	//Tableの中身の配列を指定
		rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);	//Tableで利用する数

		//平行光源
		rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//CBVを使う
		rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	//PixelShaderで使う
		rootParameters[3].Descriptor.ShaderRegister = 1;	//レジスタ番号1を使う

		//カメラ
		rootParameters[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//CBVを使う
		rootParameters[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	//PixelShaderで使う
		rootParameters[4].Descriptor.ShaderRegister = 2;

		//点光源
		rootParameters[5].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//CBVを使う
		rootParameters[5].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	//PixelShaderで使う
		rootParameters[5].Descriptor.ShaderRegister = 3;

		//スポットライト
		rootParameters[6].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//CBVを使う
		rootParameters[6].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	//PixelShaderで使う
		rootParameters[6].Descriptor.ShaderRegister = 4;

		break;
	}

	descriptionRootSignature.pParameters = rootParameters.data();	//ルートパラメータ配列へのポインタ
	descriptionRootSignature.NumParameters = static_cast<UINT>(rootParameters.size());	//配列の長さ

	//Samplerの設定
	std::vector<D3D12_STATIC_SAMPLER_DESC> staticSamplers = {};

	switch (blendMode) {
	case BlendMode::kLineMode:
	case BlendMode::kSphereMode:

		break;

	default:

		staticSamplers.resize(1);

		staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;	//バイリニアフィルタ
		staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;	//0～1の範囲外をリピート
		staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;	//比較しない
		staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX;	//ありったけのMipmapｗｐ使う
		staticSamplers[0].ShaderRegister = 0;	//レジスタ番号0を使う
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
	case BlendMode::kLineMode:
	case BlendMode::kSphereMode:

		inputElementDescs.resize(1);

		inputElementDescs[0].SemanticName = "POSITION";
		inputElementDescs[0].SemanticIndex = 0;
		inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

		break;

	case BlendMode::kSkinModelMode:

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
	inputLayoutDesc.pInputElementDescs = inputElementDescs.data();
	inputLayoutDesc.NumElements = static_cast<UINT>(inputElementDescs.size());

	//BlendStateの設定
	D3D12_BLEND_DESC blendDesc{};
	//すべての色要素を書き込む
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;

	switch (blendMode) {
	case BlendMode::kBlendModeNone:
	default:
		break;

	case BlendMode::kBlendModeNormal:
	case BlendMode::kBlendModeNormalSprite:
	case BlendMode::kBlendModeNormalinstancing:
		blendDesc.RenderTarget[0].BlendEnable = TRUE;
		blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
		
		break;

	case BlendMode::kBlendModeAdd:
	case BlendMode::kBlendModeAddParticle:
		blendDesc.RenderTarget[0].BlendEnable = TRUE;
		blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
		break;

	case BlendMode::kBlendModeSubtract:
		blendDesc.RenderTarget[0].BlendEnable = TRUE;
		blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;
		blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
		break;

	case BlendMode::kBlendModeMultiply:
		blendDesc.RenderTarget[0].BlendEnable = TRUE;
		blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_SRC_COLOR;
		break;

	case BlendMode::kBlendModeScreen:
		blendDesc.RenderTarget[0].BlendEnable = TRUE;
		blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_INV_DEST_COLOR;
		blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
		break;
	}
	
	//ResiterizerStateの設定
	D3D12_RASTERIZER_DESC rasterizerDesc{};
	switch (blendMode) {
	case BlendMode::kBlendModeNoneSprite:
	case BlendMode::kBlendModeNormalSprite:
	case BlendMode::kBackGroundSprite:
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

	case BlendMode::kBlendModeNormalinstancing:
		vertexShaderBlob = dxCommon->CompilerShader(L"resources/shader/InstancingObject3d.VS.hlsl",
			L"vs_6_0");
		assert(vertexShaderBlob != nullptr);

		pixelShaderBlob = dxCommon->CompilerShader(L"resources/shader/Object3d.PS.hlsl",
			L"ps_6_0");
		assert(pixelShaderBlob != nullptr);
		break;

	case BlendMode::kBlendModeNoneSprite:
	case BlendMode::kBlendModeNormalSprite:
	case BlendMode::kBackGroundSprite:

		vertexShaderBlob = dxCommon->CompilerShader(L"resources/shader/Sprite.VS.hlsl",
			L"vs_6_0");
		assert(vertexShaderBlob != nullptr);

		pixelShaderBlob = dxCommon->CompilerShader(L"resources/shader/Sprite.PS.hlsl",
			L"ps_6_0");
		assert(pixelShaderBlob != nullptr);

		break;

	case BlendMode::kBlendModeAddParticle:

		//Particle用
		vertexShaderBlob = dxCommon->CompilerShader(L"resources/shader/Particle.VS.hlsl",
			L"vs_6_0");
		assert(vertexShaderBlob != nullptr);

		pixelShaderBlob = dxCommon->CompilerShader(L"resources/shader/Particle.PS.hlsl",
			L"ps_6_0");
		assert(pixelShaderBlob != nullptr);

		break;

	case BlendMode::kLineMode:

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
	case BlendMode::kSphereMode:

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

	case BlendMode::kSkinModelMode:


		vertexShaderBlob = dxCommon->CompilerShader(L"resources/shader/SkinningObject3d.VS.hlsl",
			L"vs_6_0");
		assert(vertexShaderBlob != nullptr);

		pixelShaderBlob = dxCommon->CompilerShader(L"resources/shader/Object3d.PS.hlsl",
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
	case BlendMode::kLineMode:
	case BlendMode::kSphereMode:
		//Depthの機能を有効化する
		depthStencilDesc.DepthEnable = false;
		//書き込みします
		depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;

		break;
	case BlendMode::kBlendModeAddParticle:
		//Depthの機能を有効化する
		depthStencilDesc.DepthEnable = true;
		depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
		break;

	case BlendMode::kBackGroundSprite:
		depthStencilDesc.DepthEnable = FALSE;  // 深度バッファ無効化
		depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;  // 深度への書き込み無効
		depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;
		depthStencilDesc.StencilEnable = FALSE;
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
	case BlendMode::kLineMode:
	case BlendMode::kSphereMode:

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

	case BlendMode::kLineMode:
	case BlendMode::kSphereMode:

		graphicsPipelineStateDesc.PrimitiveTopologyType =
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
		break;

	}
	//どのように画面に色を打ち込むかの設定（気にしなくて良い）
	graphicsPipelineStateDesc.SampleDesc.Count = 1;
	graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	//実際に生成
	hr = dxCommon->GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDesc,
		IID_PPV_ARGS(&pipelineSet->graphicsPipelineState));
	assert(SUCCEEDED(hr));

	return pipelineSet;
}

void PrimitiveDrawer::SetPipelineSet(ID3D12GraphicsCommandList* commandList, BlendMode blendMode) {

	commandList->SetGraphicsRootSignature(pipelineSets_.at(static_cast<uint16_t>(blendMode))->rootSignature.Get());
	commandList->SetPipelineState(pipelineSets_.at(static_cast<uint16_t>(blendMode))->graphicsPipelineState.Get());	//PSOを設定

}
