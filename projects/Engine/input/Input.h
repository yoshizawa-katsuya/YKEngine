#pragma once
#include <Windows.h>
#include <wrl.h>
#define DIRECTINPUT_VERSION		0x0800	//DirectInputのバージョン指定
#include <dinput.h>
#include <Xinput.h>
#include <iostream>
#include "WinApp.h"
#include "Struct.h"

namespace YKEngine
{

/// <summary>
/// 入力クラス。
/// キーボード、マウス、ゲームパッドの入力を管理する。
/// 押した瞬間、押している、離した瞬間、押しっぱなしをチェックできる。
/// </summary>
class Input
{
public:
	
	/// <summary>
	/// シングルトンインスタンスの取得。
	/// </summary>
	/// <returns>入力クラスのインスタンス</returns>
	static Input* GetInstance();

	/// <summary>
	/// 終了処理。
	/// </summary>
	void Finalize();

	/// <summary>
	/// 初期化。
	/// </summary>
	/// <param name="winApp">ウィンドウアプリケーションクラス</param>
	void Initialize(WinApp* winApp);

	/// <summary>
	/// 更新。
	/// </summary>
	void Update();

	/// <summary>
	/// キーの押下をチェック。
	/// </summary>
	/// <param name="keyNumber">キー番号( DIK_0 等)</param>
	/// <returns>押されていればtrue。そうでなければfalse。</returns>
	bool PushKey(BYTE keyNumber);

	/// <summary>
	/// キーの押した瞬間をチェック。
	/// </summary>
	/// <param name="keyNumber">キー番号( DIK_0 等)</param>
	/// <returns>押した瞬間であればtrue。そうでなければfalse。</returns>
	bool TriggerKey(BYTE keyNumber);

	/// <summary>
	/// キーの離した瞬間をチェック。
	/// </summary>
	/// <param name="keyNumber">キー番号( DIK_0 等)</param>
	/// <returns>離した瞬間であればtrue。そうでなければfalse。</returns>
	bool ReleaseKey(BYTE keyNumber);

	/// <summary>
	/// キーを押しっぱなしにしているかチェック。
	/// </summary>
	/// <param name="keyNumber">キー番号( DIK_0 等)</param>
	/// <returns>押しっぱなしであればtrue。そうでなければfalse。</returns>
	bool HoldKey(BYTE keyNumber);

	/// <summary>
	/// マウスを左クリックしているかチェック。
	/// </summary>
	/// <returns>左クリックしていればtrue。そうでなければfalse。</returns>
	bool PushMouseLeft();

	/// <summary>
	/// マウスを左クリックした瞬間かチェック。
	/// </summary>
	/// <returns>左クリックした瞬間であればtrue。そうでなければfalse。</returns>
	bool TrigerMouseLeft();

	/// <summary>
	/// マウスの左クリックを離した瞬間かチェック。
	/// </summary>
	/// <returns>左クリックを離した瞬間であればtrue。そうでなければfalse。</returns>
	bool ReleaseMouseLeft();

	/// <summary>
	/// マウスの左クリックを押し続けているかチェック。
	/// </summary>
	/// <returns>左クリックを押し続けていればtrue。そうでなければfalse。</returns>
	bool HoldMouseLeft();

	/// <summary>
	/// マウスを中央(マウスホイール)クリックしているかチェック。
	/// </summary>
	/// <returns>中央クリックしていればtrue。そうでなければfalse。</returns>
	bool PushMouseCenter();

	/// <summary>
	/// マウスホイールを回転した量を取得。
	/// </summary>
	/// <returns>マウスホイールの回転量。正の値で前方、負の値で後方。</returns>
	float GetMouseWheel();

	/// <summary>
	/// マウスの移動量を取得。
	/// </summary>
	/// <returns>マウスの移動量ベクトル。</returns>
	Vector2 GetMouseVelocity();

	/// <summary>
	/// マウスの座標を取得。
	/// ウィンドウ内での座標系。
	/// </summary>
	/// <returns>マウスの座標。</returns>
	Vector2 GetMousePosition();

	/// <summary>
	/// 前フレームのキーの押下をチェック。
	/// </summary>
	/// <param name="keyNumber">キー番号( DIK_0 等)</param>
	/// <returns>押されていればtrue。そうでなければfalse。</returns>
	bool IsPushKeyPre(BYTE keyNumber);

	/// <summary>
	/// ゲームパッドのボタンを押しているかをチェック。
	/// </summary>
	/// <param name="xinput">どのボタンか(XINPUT_GAMEPAD_A等)</param>
	/// <returns>押されていればtrue。そうでなければfalse。</returns>
	bool PushButton(uint32_t xinput);

	/// <summary>
	/// ゲームパッドのボタンを押した瞬間をチェック。
	/// </summary>
	/// <param name="xinput">どのボタンか(XINPUT_GAMEPAD_A等)</param>
	/// <returns>押した瞬間であればtrue。そうでなければfalse。</returns>
	bool TriggerButton(uint32_t xinput);

	/// <summary>
	/// ゲームパッドのボタンを離した瞬間をチェック。
	/// </summary>
	/// <param name="xinput">どのボタンか(XINPUT_GAMEPAD_A等)</param>
	/// <returns>離した瞬間であればtrue。そうでなければfalse。</returns>
	bool ReleaseButton(uint32_t xinput);

	/// <summary>
	/// ゲームパッドの右トリガーを押した瞬間をチェック。
	/// </summary>
	/// <returns>押した瞬間であればtrue。そうでなければfalse。</returns>
	bool TrigerRT();

	/// <summary>
	/// ゲームパッドのボタンを押しっぱなしにしているかチェック。
	/// </summary>
	/// <param name="xinput">どのボタンか(XINPUT_GAMEPAD_A等)</param>
	/// <returns>押しっぱなしであればtrue。そうでなければfalse。</returns>
	bool HoldButton(uint32_t xinput);

	/// <summary>
	/// 左スティックのX方向の入力。-1.0f~1.0fの範囲。
	/// </summary>
	/// <returns>左スティックのX方向の入力値。</returns>
	float GetLeftStickX();

	/// <summary>
	/// 左スティックのY方向の入力。-1.0f~1.0fの範囲。
	/// </summary>
	/// <returns>左スティックのY方向の入力値。</returns>
	float GetLeftStickY();

	/// <summary>
	/// 左スティックを下に倒した瞬間をチェック。
	/// </summary>
	/// <returns>下に倒した瞬間であればtrue。そうでなければfalse。</returns>
	bool TrigerLeftStickDown();

	/// <summary>
	/// 左スティックを上に倒した瞬間をチェック。
	/// </summary>
	/// <returns>上に倒した瞬間であればtrue。そうでなければfalse。</returns>
	bool TrigerLeftStickUp();

	/// <summary>
	/// 右スティックのX方向の入力。-1.0f~1.0fの範囲。
	/// </summary>
	/// <returns>右スティックのX方向の入力値。</returns>
	float GetRightStickX();

	/// <summary>
	/// 右スティックのY方向の入力。-1.0f~1.0fの範囲。
	/// </summary>
	/// <returns>右スティックのY方向の入力値。</returns>
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

	Input() = default;
	~Input() = default;
	Input(Input&) = default;
	Input& operator=(Input&) = default;

	/// <summary>
	/// ゲームパッドの状態更新。
	/// </summary>
	/// <param name="padNo">パッド番号</param>
	/// <returns>接続されていればtrue。そうでなければfalse。</returns>
	bool GamePadUpdate(uint32_t padNo = 0);

	//WindowsAPI
	WinApp* winApp_ = nullptr;

	Microsoft::WRL::ComPtr<IDirectInputDevice8> keyboard_;
	Microsoft::WRL::ComPtr<IDirectInputDevice8> mouse_;
	Microsoft::WRL::ComPtr<IDirectInput8> directInput_ = nullptr;
	//全キーの入力情報
	BYTE key_[256] = {};
	//前回の全キーの入力情報
	BYTE keyPre_[256] = {};

	DIMOUSESTATE mouseState_;
	DIMOUSESTATE preMouseState_;

	const int32_t kDeadZone_ = 8000;
	XINPUT_STATE gamePadState_;
	XINPUT_STATE preGamePadState_;

};

} // namespace YKEngine