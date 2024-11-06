#pragma once
#include <Windows.h>
#include <wrl.h>
#define DIRECTINPUT_VERSION		0x0800	//DirectInputのバージョン指定
#include <dinput.h>
#include <Xinput.h>
#include <iostream>
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

	/// <summary>
	/// キーを押しっぱなしにしているかチェック
	/// </summary>
	/// <param name="keyNumber">キー番号( DIK_0 等)</param>
	/// <returns>トリガーか</returns>
	bool HoldKey(BYTE keyNumber);

	bool PushMouseLeft();

	bool TrigerMouseLeft();

	bool PushMouseCenter();

	float GetMouseWheel();

	Vector2 GetMouseVelocity();

	bool IsPushKeyPre(BYTE keyNumber);

	//ゲームパッドの状態を得る。毎フレームの最初に一度だけやれば良い
	bool GamePadUpdate(uint32_t padNo = 0);

	//ゲームパッドのボタンを押しているかチェック
	bool PushButton(uint32_t xinput);

	//ゲームパッドのボタンを押した瞬間をチェック
	bool TriggerButton(uint32_t xinput);

	//ゲームパッドのボタンを離した瞬間をチェック
	bool ReleaseButton(uint32_t xinput);

	//ゲームパッドのボタンを押し続けているかをチェック
	bool HoldButton(uint32_t xinput);

	//左スティックのX方向の入力。0.0f~1.0fの範囲
	float GetLeftStickX();

	//左スティックのY方向の入力。0.0f~1.0fの範囲
	float GetLeftStickY();

	//右スティックのX方向の入力。0.0f~1.0fの範囲
	float GetRightStickX();

	//右スティックのY方向の入力。0.0f~1.0fの範囲
	float GetRightStickY();

private:

	/*
	ゲームパッドのボタン
	XINPUT_GAMEPAD_DPAD_UP
	XINPUT_GAMEPAD_DPAD_DOWN
	XINPUT_GAMEPAD_DPAD_LEFT
	XINPUT_GAMEPAD_DPAD_RIGHT
	XINPUT_GAMEPAD_START
	XINPUT_GAMEPAD_BACK
	XINPUT_GAMEPAD_LEFT_THUMB
	XINPUT_GAMEPAD_RIGHT_THUMB
	XINPUT_GAMEPAD_LEFT_SHOULDER
	XINPUT_GAMEPAD_RIGHT_SHOULDER
	XINPUT_GAMEPAD_A
	XINPUT_GAMEPAD_B
	XINPUT_GAMEPAD_X
	XINPUT_GAMEPAD_Y
	*/

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

	const int32_t deadZone_ = 8000;
	XINPUT_STATE gamePadState_;
	XINPUT_STATE preGamePadState_;

};

