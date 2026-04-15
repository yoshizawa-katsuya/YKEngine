#include "ClearUI.h"
#include "Sprite.h"
#include "GlobalVariables.h"
#include "JsonKeys.h"

using namespace YKEngine;

void ClearUI::Initialize()
{
	spriteClear_ = std::make_unique<Sprite>();
	spriteClear_->Initialize(TextureManager::GetInstance()->Load("./Resources/clear.png"));

	pressA_ = std::make_unique<PressA>();
	pressA_->Initialize();

	//グローバル変数に登録
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const std::string& groupName = JsonKey::Clear::UI::kGroupName;
	globalVariables->CreateGroup(groupName);
	globalVariables->AddItem(groupName, JsonKey::Clear::UI::kClearPosition, Vector2(400.0f, 300.0f));
	globalVariables->AddItem(groupName, JsonKey::Clear::UI::kPressAPosition, Vector2(400.0f, 500.0f));

	SetUIPosition();
}

void ClearUI::Update()
{
#ifdef _DEBUG

	SetUIPosition();

#endif // _DEBUG

	pressA_->Update();

}

void ClearUI::Draw()
{
	spriteClear_->Draw();
	pressA_->Draw();
}

void ClearUI::SetUIPosition()
{
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const std::string& groupName = JsonKey::Clear::UI::kGroupName;

	spriteClear_->SetPosition(globalVariables->GetVector2Value(groupName, JsonKey::Clear::UI::kClearPosition));
	pressA_->SetPosition(globalVariables->GetVector2Value(groupName, JsonKey::Clear::UI::kPressAPosition));
}
