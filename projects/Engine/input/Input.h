#pragma once
#include <Windows.h>
#include <wrl.h>
#define DIRECTINPUT_VERSION		0x0800	//DirectInputのバージョン指定
#include <dinput.h>
#include "WinApp.h"
#include "Struct.h"

using namespace Microsoft::WRL;

//入力
class Input
{
public:
	
	//シングルトンインスタンスの取得
	static Input* GetInstance();

	//終了
	void Finalize();

	//初期化
	void Initialize(WinApp* winApp);

	//更新
	void Update();

	/// <summary>
	/// キーの押下をチェック
	/// </summary>
	/// <param name="keyNumber">キー番号( DIK_0 等)</param>
	/// <returns>押されているか</returns>
	bool PushKey(BYTE keyNumber);

	/// <summary>
	/// キーの押した瞬間をチェック
	/// </summary>
	/// <param name="keyNumber">キー番号( DIK_0 等)</param>
	/// <returns>トリガーか</returns>
	bool TriggerKey(BYTE keyNumber);

	/// <summary>
	/// キーの離した瞬間をチェック
	/// </summary>
	/// <param name="keyNumber">キー番号( DIK_0 等)</param>
	/// <returns>トリガーか</returns>
	bool ReleaseKey(BYTE keyNumber);

	bool PushMouseLeft();

	bool TrigerMouseLeft();

	bool PushMouseCenter();

	float GetMouseWheel();

	Vector2 GetMouseVelocity();

	bool IsPushKeyPre(BYTE keyNumber);

private:

	static Input* instance_;

	Input() = default;
	~Input() = default;
	Input(Input&) = default;
	Input& operator=(Input&) = default;

	//WindowsAPI
	WinApp* winApp_ = nullptr;

	ComPtr<IDirectInputDevice8> keyboard_;
	ComPtr<IDirectInputDevice8> mouse_;
	ComPtr<IDirectInput8> directInput_ = nullptr;
	//全キーの入力情報
	BYTE key_[256] = {};
	//前回の全キーの入力情報
	BYTE keyPre_[256] = {};

	DIMOUSESTATE mouseState_;
	DIMOUSESTATE preMouseState_;

};

