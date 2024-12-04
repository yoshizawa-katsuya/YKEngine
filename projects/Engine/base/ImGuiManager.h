#pragma once
#include "SrvHeapManager.h"
#include "DirectXCommon.h"

class ImGuiManager
{
public:

	~ImGuiManager();

	//初期化
	void Initialize(DirectXCommon* dxCommon, WinApp* winApp, SrvHeapManager* srvHeapManager);

	//ImGui受付開始
	void Begin();

	//ImGui受付終了
	void End();

	//画面への描画
	void Draw();

private:
	//終了
	void Finalize();

	DirectXCommon* dxCommon_;

};

