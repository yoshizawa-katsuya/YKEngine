#pragma once
#include "SrvHeapManager.h"
#include "DirectXCommon.h"

/// <summary>
/// ImGui管理クラス。
/// ImGuiの初期化、描画を行う。
/// </summary>
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

