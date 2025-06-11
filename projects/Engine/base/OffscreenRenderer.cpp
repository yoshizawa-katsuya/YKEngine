#include "OffscreenRenderer.h"
#include "SrvHeapManager.h"
#include "PrimitiveDrawer.h"

OffscreenRenderer* OffscreenRenderer::GetInstance()
{
	static OffscreenRenderer instance;
	return &instance;
}

void OffscreenRenderer::Initialize(SrvHeapManager* srvHeapManager)
{
	dxCommon_ = DirectXCommon::GetInstance();
	commandList_ = dxCommon_->GetCommandList();
	viewport_ = dxCommon_->GetViewport();
	scissorRect_ = dxCommon_->GetScissorRect();

	//RecderTexture作成
	CreateRenderTexture();

	CreateRenderTextureSRV(srvHeapManager);

}

void OffscreenRenderer::PreDrawRenderTexture()
{
	//描画先のRTVとDSVを設定する
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = dxCommon_->GetDSVCPUDescriptorHandle(0);
	commandList_->OMSetRenderTargets(1, &renderTextureRtvHandle_, false, &dsvHandle);

	//指定した色で画面全体をクリアする
	float clearColor[] = { 1.0f, 0.0f, 0.0f, 1.0f };	//赤
	commandList_->ClearRenderTargetView(renderTextureRtvHandle_, clearColor, 0, nullptr);

	//指定した深度で画面全体をクリアする
	commandList_->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	commandList_->RSSetViewports(1, viewport_);	//Viewportを設定

	commandList_->RSSetScissorRects(1, scissorRect_);	//Scirssorを設定
}

void OffscreenRenderer::PostDrawRenderTexture(PrimitiveDrawer* primitiveDrawer, SrvHeapManager* srvHeapManager)
{
	//TransitionBarrierの設定
	D3D12_RESOURCE_BARRIER barrier{};
	//今回のバリアはTransition
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	//Noneにしておく
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	//バリアを張る対象のリソース。現在のバックバッファに対して使う
	barrier.Transition.pResource = renderTextureResource_.Get();
	//遷移前（現在）のResourceState
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	//遷移後のResourceState
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
	//TransitionBarrierを張る
	commandList_->ResourceBarrier(1, &barrier);

	//コピー実行
	primitiveDrawer->SetPipelineSet(commandList_, BlendMode::kOffScreenRendering);
	srvHeapManager->SetGraphicsRootDescriptorTable(0, renderTextureSRVIndex_);

	commandList_->DrawInstanced(3, 1, 0, 0);

	//TransitionBarrierの設定
	D3D12_RESOURCE_BARRIER barrier2{};
	//今回のバリアはTransition
	barrier2.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	//Noneにしておく
	barrier2.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	//バリアを張る対象のリソース。現在のバックバッファに対して使う
	barrier2.Transition.pResource = renderTextureResource_.Get();
	//遷移前（現在）のResourceState
	barrier2.Transition.StateBefore = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
	//遷移後のResourceState
	barrier2.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	//TransitionBarrierを張る
	commandList_->ResourceBarrier(1, &barrier2);
}

void OffscreenRenderer::CreateRenderTexture()
{
	const Vector4 kRenderTargetClearValue{ 1.0f, 0.0f, 0.0f, 1.0f };	//一旦分かりやすいように赤
	CreateRenderTextureResource(WinApp::kClientWidth, WinApp::kClientHeight,
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, kRenderTargetClearValue);

	//RTVの設定
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;	//出力結果をSRGBに変換して書き込む
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;	//2dテクスチャとして書き込む

	renderTextureRtvHandle_ = dxCommon_->GetRTVCPUDescriptorHandle(2);

	dxCommon_->GetDevice()->CreateRenderTargetView(renderTextureResource_.Get(), &rtvDesc, renderTextureRtvHandle_);


}

void OffscreenRenderer::CreateRenderTextureSRV(SrvHeapManager* srvHeapManager)
{
	renderTextureSRVIndex_ = srvHeapManager->Allocate();
	srvHeapManager->CreateSRVforRenderTexture(renderTextureSRVIndex_, renderTextureResource_.Get());
}

void OffscreenRenderer::CreateRenderTextureResource(int32_t width, int32_t height, DXGI_FORMAT format, const Vector4& clearColor)
{
	//生成するResourceの設定
	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Width = width;	//Textureの幅
	resourceDesc.Height = height;	//Textureの高さ
	resourceDesc.MipLevels = 1;	//mipmapの数
	resourceDesc.DepthOrArraySize = 1;	//奥行き or 配列Textureの配列数
	resourceDesc.Format = format;	//DepthStencliとして利用可能なフォーマット
	resourceDesc.SampleDesc.Count = 1;	//サンプリングカウント。1固定。
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;	//2次元
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;	//RenderTargetとして使う通知

	//利用するHeapの設定
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;	//VRAM上に作る

	D3D12_CLEAR_VALUE clearValue;
	clearValue.Format = format;
	clearValue.Color[0] = clearColor.x;
	clearValue.Color[1] = clearColor.y;
	clearValue.Color[2] = clearColor.z;
	clearValue.Color[3] = clearColor.w;

	//Resourceの生成
	HRESULT hr = dxCommon_->GetDevice()->CreateCommittedResource(
		&heapProperties,	//Heapの設定
		D3D12_HEAP_FLAG_NONE,	//Heapの特殊な設定。特になし。
		&resourceDesc,	//Resourceの設定
		D3D12_RESOURCE_STATE_RENDER_TARGET,	//RenderTargetとして使う
		&clearValue,	//Clear最適値
		IID_PPV_ARGS(&renderTextureResource_)	//作成するResourceポインタへのポインタ
	);
	assert(SUCCEEDED(hr));

}
