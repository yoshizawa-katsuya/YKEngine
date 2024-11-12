#pragma once
#include <Windows.h>
#include <cstdint>

class WinApp
{
public:

	~WinApp();

	//ウィンドウプロシージャ
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	//初期化
	void Initialize();

	//メッセージの処理
	bool ProcessMessage();


	HWND GetHwnd() const { return hwnd_; }

	HINSTANCE GetHinstance() const { return wc_.hInstance; }

	//クライアント領域のサイズ
	static const int32_t kClientWidth = 1280;
	static const int32_t kClientHeight = 720;

private:

	//ゲームウィンドウの作成
	void CreateGameWindow();

	//ゲームウィンドウの破棄
	void TerminateGameWindow();

	//ウィンドウクラス
	WNDCLASS wc_{};

	//ウィンドウハンドル
	HWND hwnd_ = nullptr;
	
};

