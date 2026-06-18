#include "Pause.h"
#include "TextureManager.h"
#include <cstdint>
#include "Sprite.h"
#include "GlobalVariables.h"
#include "JsonKeys.h"
#include "Input.h"
#include "manager/AudioManager.h"

using namespace YKEngine;

void Pause::Initialize()
{
	//テクスチャの読み込み
	TextureManager* textureManager = TextureManager::GetInstance();
	uint32_t backGroundTetureHandle = textureManager->Load("./Resources/pause/backGround.png");
	uint32_t cursorTextureHandle = textureManager->Load("./Resources/pause/cursor.png");
	std::unordered_map<UIName, uint32_t> UITextureHandles;
	UITextureHandles[UIName::kReturnToTitle] = textureManager->Load("./Resources/pause/returnToTitle.png");
	UITextureHandles[UIName::kContinue] = textureManager->Load("./Resources/pause/continue.png");

	//スプライトの生成
	backgroundSprite_ = std::make_unique<Sprite>();
	backgroundSprite_->Initialize(backGroundTetureHandle);
	cursorSprite_ = std::make_unique<Sprite>();
	cursorSprite_->Initialize(cursorTextureHandle);

	for (uint32_t uiNameIndex = 0; uiNameIndex < static_cast<uint32_t>(UIName::kCount); ++uiNameIndex)
	{
		UIName uiName = static_cast<UIName>(uiNameIndex);
		UISprites_[uiName] = std::make_unique<Sprite>();
		UISprites_[uiName]->Initialize(UITextureHandles[uiName]);
	}

	//グローバル変数に登録
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const std::string& groupName = JsonKey::Pouse::kGroupName;
	globalVariables->CreateGroup(groupName);
	globalVariables->AddItem(groupName, JsonKey::Pouse::kReturnTitlePosition, Vector2(400.0f, 300.0f));
	globalVariables->AddItem(groupName, JsonKey::Pouse::kContinuePosition, Vector2(800.0f, 300.0f));
	globalVariables->AddItem(groupName, JsonKey::Pouse::kCursorOffset, 300.0f);

	SetUIPositions();
}

void Pause::Update()
{
#ifdef _DEBUG

	SetUIPositions();

#endif // _DEBUG

	Input* input = Input::GetInstance();
	//カーソルの移動
	if (input->TriggerKey(DIK_W) || input->TriggerButton(XINPUT_GAMEPAD_DPAD_UP) || input->TriggerLeftStickUp())
	{
		//カーソル移動SEを流す
		AudioManager::GetInstance()->PlaySE(SEType::kCursorMove01);

		cursorTarget_++;
		if (cursorTarget_ >= static_cast<int32_t>(UIName::kCount))
		{
			cursorTarget_ = 0;
		}
	}
	else if (input->TriggerKey(DIK_S) || input->TriggerButton(XINPUT_GAMEPAD_DPAD_DOWN) || input->TriggerLeftStickDown())
	{
		//カーソル移動SEを流す
		AudioManager::GetInstance()->PlaySE(SEType::kCursorMove01);

		cursorTarget_--;
		if (cursorTarget_ < 0)
		{
			cursorTarget_ = static_cast<int32_t>(UIName::kCount) - 1;
		}
	}
	cursorSprite_->SetPosition({ cursorSprite_->GetPosition().x, UIPositions_[static_cast<UIName>(cursorTarget_)].y});

	if (input->TriggerKey(DIK_SPACE) || input->TriggerButton(XINPUT_GAMEPAD_A))
	{
		//決定SEを流す
		AudioManager::GetInstance()->PlaySE(SEType::kDecision01);

		switch (static_cast<UIName>(cursorTarget_))
		{
		case UIName::kReturnToTitle:
			//タイトルへ戻る処理
			isReturnToTitle_ = true;
			break;
		case UIName::kContinue:
			//ポーズ解除
			isPause_ = false;
			break;
		default:
			break;
		}
	}

	if (input->TriggerKey(DIK_ESCAPE) || input->TriggerButton(XINPUT_GAMEPAD_B) || input->TriggerButton(XINPUT_GAMEPAD_START))
	{
		//ポーズ解除
		isPause_ = false;
		//決定SEを流す
		AudioManager::GetInstance()->PlaySE(SEType::kDecision01);
	}
}

void Pause::Draw()
{
	//ポーズ中でなければ描画しない
	if (!isPause_)
	{
		return;
	}
	backgroundSprite_->Draw();
	for (std::pair<const UIName, std::unique_ptr<Sprite>>& ui : UISprites_)
	{
		ui.second->Draw();
	}
	cursorSprite_->Draw();
}

void Pause::SetIsPause(bool isPause)
{
	isPause_ = isPause;
	if (isPause_)
	{
		//カーソルの初期位置を「続ける」に設定
		cursorTarget_ = static_cast<int32_t>(UIName::kContinue);

		//メニューSEを流す
		AudioManager::GetInstance()->PlaySE(SEType::kMenu01);
	}
}

void Pause::SetUIPositions()
{
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const std::string& groupName = JsonKey::Pouse::kGroupName;

	//UI位置の取得
	UIPositions_[UIName::kReturnToTitle] = globalVariables->GetVector2Value(groupName, JsonKey::Pouse::kReturnTitlePosition);
	UIPositions_[UIName::kContinue] = globalVariables->GetVector2Value(groupName, JsonKey::Pouse::kContinuePosition);

	//スプライトの位置設定
	for (std::pair<const UIName, std::unique_ptr<Sprite>>& ui : UISprites_)
	{
		ui.second->SetPosition(UIPositions_[ui.first]);
	}
	cursorSprite_->SetPosition({ globalVariables->GetFloatValue(groupName, JsonKey::Pouse::kCursorOffset), UIPositions_[static_cast<UIName>(cursorTarget_)].y});
}
