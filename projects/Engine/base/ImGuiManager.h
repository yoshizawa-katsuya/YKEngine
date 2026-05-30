#pragma once
#include "SrvHeapManager.h"
#include "DirectXCommon.h"

namespace YKEngine
{

/// <summary>
/// ImGui管理クラス。
/// ImGuiの初期化、描画を行う。
/// </summary>
class ImGuiManager
{
public:

	/// <summary>
	/// デストラクタ。
	/// </summary>
	~ImGuiManager();

	/// <summary>
	/// 初期化。
	/// </summary>
	/// <param name="dxCommon">DirectX共通クラス。</param>
	/// <param name="winApp">ウィンドウアプリケーションクラス。</param>
	/// <param name="srvHeapManager">SRVヒープマネージャークラス。</param>
	void Initialize(DirectXCommon* dxCommon, WinApp* winApp, SrvHeapManager* srvHeapManager);

	/// <summary>
	/// ImGui受付開始。
	/// </summary>
	void Begin();

	/// <summary>
	/// ImGui受付終了。
	/// </summary>
	void End();

	/// <summary>
	/// 描画。
	/// </summary>
	void Draw();

private:
	/// <summary>
	/// 終了処理。
	/// </summary>
	void Finalize();

	DirectXCommon* dxCommon_;

};

} // namespace YKEngine