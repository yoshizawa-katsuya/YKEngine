#include "TextureManager.h"
#include <dxcapi.h>
#include <cassert>
#include "dx12.h"
#include "DirectXTex/d3dx12.h"

TextureManager* TextureManager::GetInstance()
{
	static TextureManager instance;
	return &instance;
}

void TextureManager::Finalize()
{
	
}

void TextureManager::Initialize(DirectXCommon* dxCommon, SrvHeapManager* srvHeapManager) {

	assert(dxCommon);

	dxCommon_ = dxCommon;
	srvHeapManager_ = srvHeapManager;
	
	
	//全テクスチャリセット
	//ResetAll();

}

void TextureManager::PostDraw()
{
	if (!intermediateResources_.empty()) {
		intermediateResources_.clear(); // 必要なときだけ呼ぶ
	}
}

uint32_t TextureManager::Load(const std::string& fileName) {

	//読み込み済みテクスチャを検索
	if (textureHandles_.contains(fileName)) {
		return textureHandles_[fileName];
	}

	uint32_t index = srvHeapManager_->Allocate();
	textureHandles_[fileName] = index;

	//テクスチャ枚数上限チェック
	assert(srvHeapManager_->Check());

	//Textureを読んで転送する
	LoadTexture(fileName, index);

	return index;
}

void TextureManager::SetGraphicsRootDescriptorTable(uint32_t textureHandle) {

	// シェーダリソースビューをセット
	srvHeapManager_->SetGraphicsRootDescriptorTable(2, textureHandle);
	
}

const DirectX::TexMetadata& TextureManager::GetMetaData(uint32_t textureHandle)
{
	
	//範囲外指定違反チェック
	assert(textureHandle > 0 && textureHandle <= SrvHeapManager::kMaxSrvDescriptors_);

	Texture& texture = textures_[textureHandle];
	return texture.metadata;

}


//DiscriptorHeap作成の関数
Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> TextureManager::CreateDescriptorHeap(
	Microsoft::WRL::ComPtr<ID3D12Device> device, D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescriptors, bool shaderVisiblr) {

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap = nullptr;
	D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc{};
	descriptorHeapDesc.Type = heapType;
	descriptorHeapDesc.NumDescriptors = numDescriptors;
	descriptorHeapDesc.Flags = shaderVisiblr ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

	HRESULT hr = device->CreateDescriptorHeap(&descriptorHeapDesc, IID_PPV_ARGS(&descriptorHeap));
	assert(SUCCEEDED(hr));

	return descriptorHeap;

}

void TextureManager::LoadTexture(const std::string& filePath, uint32_t index) {

	//テクスチャデータを追加して書き込む
	Texture& texture = textures_[index];

	//テクスチャファイルを読み込んでプログラムで扱えるようにする
	DirectX::ScratchImage image{};
	std::wstring filePathW = StringUtility::ConvertString(filePath);

	size_t pos1;
	//ファイル拡張子
	std::wstring fileExt;

	//区切り文字'.'が出てくる一番最後の部分を検索
	pos1 = filePathW.rfind('.');
	//検索がヒットしたら
	if (pos1 != std::wstring::npos) {
		//区切り文字の後ろをファイル拡張子として保存
		fileExt = filePathW.substr(pos1 + 1, filePathW.size() - pos1 - 1);
	}
	else {
		fileExt = L"";
	}

	HRESULT hr;
	if (fileExt == L"dds") {
		//DDSテクスチャのロード
		hr = DirectX::LoadFromDDSFile(filePathW.c_str(), DirectX::DDS_FLAGS_FORCE_RGB, nullptr, image);
		assert(SUCCEEDED(hr));

	}
	else {
		//WICテクスチャのロード
		hr = DirectX::LoadFromWICFile(filePathW.c_str(), DirectX::WIC_FLAGS_FORCE_SRGB, nullptr, image);
		assert(SUCCEEDED(hr));

		//ミップマップの作成
		DirectX::ScratchImage mipImages{};
		hr = DirectX::GenerateMipMaps(image.GetImages(), image.GetImageCount(), image.GetMetadata(), DirectX::TEX_FILTER_SRGB, 0, mipImages);
		//assert(SUCCEEDED(hr));
		if (SUCCEEDED(hr)) {
			image = std::move(mipImages);
		}

	}

	//ミップマップ付きのデータを返す
	//textureResource.filePath = filePath;
	texture.metadata = image.GetMetadata();
	texture.resource = dxCommon_->CreateTextureResource(texture.metadata);

	//SRVを作成するDescriptorHeapの場所を決める。先頭はImGuiが使っているのでその次を使う
	texture.cpuDescHandleSRV = srvHeapManager_->GetCPUDescriptorHandle(index);
	texture.gpuDescHandleSRV = srvHeapManager_->GetGPUDescriptorHandle(index);

	srvHeapManager_->CreateSRVforTexture2D(index, texture.resource.Get(), texture.metadata.format, UINT(texture.metadata.mipLevels));

	intermediateResources_.push_back(UploadTextureData(texture.resource.Get(), image));
}

[[nodiscard]]
Microsoft::WRL::ComPtr<ID3D12Resource> TextureManager::UploadTextureData(ID3D12Resource* textureResource, const DirectX::ScratchImage& mipImages)
{
	std::vector<D3D12_SUBRESOURCE_DATA> subresources;
	ID3D12Device* device = dxCommon_->GetDevice();
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	DirectX::PrepareUpload(device, mipImages.GetImages(), mipImages.GetImageCount(), mipImages.GetMetadata(), subresources);
	uint64_t intermediateSize = GetRequiredIntermediateSize(textureResource, 0, UINT(subresources.size()));
	Microsoft::WRL::ComPtr<ID3D12Resource> intermediateResource = dxCommon_->CreateBufferResource(intermediateSize);
	UpdateSubresources(commandList, textureResource, intermediateResource.Get(), 0, 0, UINT(subresources.size()), subresources.data());
	
	//Textureへの転送後利用できるよう、D3D12_RESOURCE_STATE_COPY_DESTからD3D12_RESOURCE_STATE_READへResourceStateを変更する
	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = textureResource;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_GENERIC_READ;;
	
	dxCommon_->GetCommandList()->ResourceBarrier(1, &barrier);

	return intermediateResource;
	/*
	//Meta情報を取得
	const DirectX::TexMetadata& metadata = mipImages.GetMetadata();
	//全MipMapについて
	for (size_t mipLevel = 0; mipLevel < metadata.mipLevels; ++mipLevel) {
		//MipMapLevelを指定して各Imageを取得
		const DirectX::Image* img = mipImages.GetImage(mipLevel, 0, 0);
		//Textureに転送
		HRESULT hr = textureResource->WriteToSubresource(
			UINT(mipLevel),
			nullptr,				//全領域へコピー
			img->pixels,			//元データアドレス
			UINT(img->rowPitch),	//1ラインサイズ
			UINT(img->slicePitch)	//1枚サイズ
		);
		assert(SUCCEEDED(hr));
	}
	*/
}
