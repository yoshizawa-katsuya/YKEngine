#include "Input.h"
#include <cassert>

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

Input* Input::GetInstance()
{
	static Input instance;
	return &instance;
}

void Input::Finalize()
{
}

void Input::Initialize(WinApp* winApp)
{

	//借りてきたWinAppのインスタンスを記録
	winApp_ = winApp;

	//DirectInputのインスタンス生成
	HRESULT result = DirectInput8Create(winApp_->GetHinstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput_, nullptr);
	assert(SUCCEEDED(result));
	//キーボードデバイス生成
	result = directInput_->CreateDevice(GUID_SysKeyboard, &keyboard_, NULL);
	assert(SUCCEEDED(result));
	//入力データ系列のセット
	result = keyboard_->SetDataFormat(&c_dfDIKeyboard);
	assert(SUCCEEDED(result));
	//排他制御レベルのセット
	result = keyboard_->SetCooperativeLevel(winApp_->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));

	//マウスデバイスの作成
	result = directInput_->CreateDevice(GUID_SysMouse, &mouse_, NULL);
	assert(SUCCEEDED(result));
	//入力データ系列のセット
	result = mouse_->SetDataFormat(&c_dfDIMouse);
	assert(SUCCEEDED(result));
	//排他制御レベルのセット
	result = mouse_->SetCooperativeLevel(winApp->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	assert(SUCCEEDED(result));

}

void Input::Update()
{

	//前回のキー入力を保存
	memcpy(keyPre_, key_, sizeof(key_));

	//キーボード情報の取得開始
	keyboard_->Acquire();
	//全キーの入力情報を取得する
	keyboard_->GetDeviceState(sizeof(key_), key_);

	preMouseState_ = mouseState_;

	mouse_->Acquire();
	mouse_->GetDeviceState(sizeof(DIMOUSESTATE), &mouseState_);

}

bool Input::PushKey(BYTE keyNumber)
{
	//指定キーを押していればtrueを返す
	if (key_[keyNumber]) {
		return true;
	}
	//そうでなければfalseを返す
	return false;
}

bool Input::TriggerKey(BYTE keyNumber)
{
	//指定キーを押した瞬間でtrueを返す
	if (!keyPre_[keyNumber] && key_[keyNumber]) {
		return true;
	}
	//そうでなければfalseを返す
	return false;
}

bool Input::ReleaseKey(BYTE keyNumber)
{
	//指定キーを離した瞬間でtrueを返す
	if (keyPre_[keyNumber] && !key_[keyNumber]) {
		return true;
	}
	//そうでなければfalseを返す
	return false;
}

bool Input::HoldKey(BYTE keyNumber)
{
	//指定キーを押し続けていればtrueを返す
	if (keyPre_[keyNumber] && key_[keyNumber]) {
		return true;
	}
	//そうでなければfalseを返す
	return false;
}

bool Input::PushMouseLeft()
{
	if (mouseState_.rgbButtons[0] & 0x80) {
		return true;
	}
	return false;
}

bool Input::TrigerMouseLeft()
{
	if (!(preMouseState_.rgbButtons[0] & 0x80) && mouseState_.rgbButtons[0] & 0x80) {
		return true;
	}
	return false;
}

bool Input::PushMouseCenter()
{
	if (mouseState_.rgbButtons[2] & 0x80) {
		return true;
	}
	return false;
}

float Input::GetMouseWheel()
{
	return (float)mouseState_.lZ;
}

Vector2 Input::GetMouseVelocity()
{

	return Vector2((float)mouseState_.lX, (float)mouseState_.lY);

}

Vector2 Input::GetMousePosition()
{
	POINT position;
	GetCursorPos(&position);
	ScreenToClient(winApp_->GetHwnd(), &position);

	return Vector2(static_cast<float>(position.x), static_cast<float>(position.y));
}

bool Input::IsPushKeyPre(BYTE keyNumber)
{
	if (keyPre_[keyNumber]) {
		return true;
	}

	// そうでなければfalseを返す
	return false;
}

bool Input::GamePadUpdate(uint32_t padNo)
{

	preGamePadState_ = gamePadState_;
	DWORD dwResult = XInputGetState(padNo, &gamePadState_);
	
	if (dwResult == ERROR_SUCCESS) {

		if (std::abs(gamePadState_.Gamepad.sThumbLX) < deadZone_) {
			gamePadState_.Gamepad.sThumbLX = 0;
		}
		if (std::abs(gamePadState_.Gamepad.sThumbLY) < deadZone_) {
			gamePadState_.Gamepad.sThumbLY = 0;
		}
		if (std::abs(gamePadState_.Gamepad.sThumbRX) < deadZone_) {
			gamePadState_.Gamepad.sThumbRX = 0;
		}
		if (std::abs(gamePadState_.Gamepad.sThumbRY) < deadZone_) {
			gamePadState_.Gamepad.sThumbRY = 0;
		}

		return true;
	}

	return false;
}

bool Input::PushButton(uint32_t xinput)
{
	if (gamePadState_.Gamepad.wButtons & xinput) {
		return true;
	}
	
	return false;
}

bool Input::TriggerButton(uint32_t xinput)
{
	if ((gamePadState_.Gamepad.wButtons & xinput) && !(preGamePadState_.Gamepad.wButtons & xinput)) {
		return true;
	}

	return false;
}

bool Input::ReleaseButton(uint32_t xinput)
{
	if (!(gamePadState_.Gamepad.wButtons & xinput) && (preGamePadState_.Gamepad.wButtons & xinput)) {
		return true;
	}

	return false;
}

bool Input::HoldButton(uint32_t xinput)
{
	if ((gamePadState_.Gamepad.wButtons & xinput) && (preGamePadState_.Gamepad.wButtons & xinput)) {
		return true;
	}

	return false;
}

float Input::GetLeftStickX()
{
	return static_cast<float>(gamePadState_.Gamepad.sThumbLX) / SHRT_MAX;
}

float Input::GetLeftStickY()
{
	return static_cast<float>(gamePadState_.Gamepad.sThumbLY) / SHRT_MAX;
}

float Input::GetRightStickX()
{
	return static_cast<float>(gamePadState_.Gamepad.sThumbRX) / SHRT_MAX;
}

float Input::GetRightStickY()
{
	return static_cast<float>(gamePadState_.Gamepad.sThumbRY) / SHRT_MAX;
}
