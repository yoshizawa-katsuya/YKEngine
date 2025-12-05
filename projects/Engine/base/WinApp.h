#pragma once
#include <Windows.h>
#include <cstdint>

namespace YKEngine
{

/// <summary>
/// Windowsアプリケーションクラス。
/// シングルトンで実装。
/// ウィンドウの生成とメッセージ処理を行う。
/// ゲームウィンドウのクライアント領域は1280x720に固定。
/// </summary>
class WinApp
{
public:

	/// <summary>
	/// デストラクタ。
	/// </summary>
	~WinApp();

	/// <summary>
	/// ウィンドウプロシージャ。
	/// </summary>
	/// <param name="hwnd">ウィンドウハンドル</param>
	/// <param name="msg">メッセージ</param>
	/// <param name="wparam">メッセージの追加情報</param>
	/// <param name="lparam">メッセージの追加情報</param>
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	/// <summary>
	/// 初期化。
	/// </summary>
	void Initialize();

	/// <summary>
	/// メッセージ処理。
	/// </summary>
	/// <returns>終了メッセージが来たらtrueを返す。それ以外はfalseを返す。</returns>
	bool ProcessMessage();


	HWND GetHwnd() const { return hwnd_; }

	HINSTANCE GetHinstance() const { return wc_.hInstance; }

	//クライアント領域のサイズ
	static const int32_t kClientWidth = 1280;
	static const int32_t kClientHeight = 720;

private:

	/// <summary>
	/// ゲームウィンドウの生成。
	/// </summary>
	void CreateGameWindow();

	/// <summary>
	/// ゲームウィンドウの破棄。
	/// </summary>
	void TerminateGameWindow();

	//ウィンドウクラス
	WNDCLASS wc_{};

	//ウィンドウハンドル
	HWND hwnd_ = nullptr;
	
};

} // namespace YKEngine