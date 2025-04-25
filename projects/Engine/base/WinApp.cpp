#include "WinApp.h"
#include <cassert>
#include "imgui/imgui_impl_win32.h"
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

#pragma comment(lib, "winmm.lib")

WinApp::~WinApp()
{

	TerminateGameWindow();

}

//ウィンドウプロシージャ
LRESULT CALLBACK WinApp::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {

	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam)) {
		return true;
	}

	//メッセージに応じてゲーム固有の処理を行う
	switch (msg) {

		//ウィンドウが破棄された
	case WM_DESTROY:
		//OSに対して、アプリの終了を伝える
		PostQuitMessage(0);
		return 0;

	}

	//標準のメッセージ処理を行う
	return DefWindowProc(hwnd, msg, wparam, lparam);

}

void WinApp::Initialize()
{

	CreateGameWindow();

	//システムタイマーの分解能を上げる
	timeBeginPeriod(1);

}

void WinApp::CreateGameWindow() {

	//COMの初期化
	HRESULT hr = CoInitializeEx(0, COINIT_MULTITHREADED);
	assert(SUCCEEDED(hr));

	//ウィンドウプロシージャ
	wc_.lpfnWndProc = WindowProc;
	//ウィンドウクラス名（なんでもいい）
	wc_.lpszClassName = L"CG2WindowClass";
	//インスタンスハンドル
	wc_.hInstance = GetModuleHandle(nullptr);
	//カーソル
	wc_.hCursor = LoadCursor(nullptr, IDC_ARROW);

	//ウィンドウクラスを登録する
	RegisterClass(&wc_);

	//ウィンドウサイズを表す構造体にクライアント領域を入れる
	RECT wrc = { 0, 0, kClientWidth, kClientHeight };

	//クライアント領域を元に実際のサイズにwrcを変更してもらう
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

	//ウィンドウの生成
	hwnd_ = CreateWindow(
		wc_.lpszClassName,		//利用するクラス名	
		L"Engine",			//タイトルバーの文字(何でも良い)
		WS_OVERLAPPEDWINDOW,	//よく見るウィンドウスタイル
		CW_USEDEFAULT,			//表示x座標(Windowsに任せる)
		CW_USEDEFAULT,			//表示y座標(WindowsOSに任せる)
		wrc.right - wrc.left,	//ウィンドウ横幅
		wrc.bottom - wrc.top,	//ウィンドウ縦幅
		nullptr,				//親ウィンドウハンドル
		nullptr,				//メニューハンドル
		wc_.hInstance,			//インスタンスハンドル
		nullptr					//オプション
	);

	//ウィンドウを表示する
	ShowWindow(hwnd_, SW_SHOW);

}

bool WinApp::ProcessMessage() {

	MSG msg{};

	//メッセージを読み取る
	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	//終了メッセージ
	if (msg.message == WM_QUIT) {

		return true;
	}

	return false;

}

void WinApp::TerminateGameWindow() {

	CloseWindow(hwnd_);
	//COMの終了処理
	CoUninitialize();

}