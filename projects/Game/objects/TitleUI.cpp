#include "TitleUI.h"
#include "Sprite.h"
#include "GlobalVariables.h"
#include "JsonKeys.h"
#include "Lerp.h"
#include "Easing.h"

using namespace YKEngine;

void TitleUI::Initialize()
{

	spriteLogo_ = std::make_unique<Sprite>();
	spriteLogo_->Initialize(TextureManager::GetInstance()->Load("./Resources/titleLogo.png"));

	pressA_ = std::make_unique<PressA>();
	pressA_->Initialize();

	//グローバル変数に登録
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const std::string& groupName = JsonKey::Title::UI::kGroupName;
	globalVariables->CreateGroup(groupName);
	globalVariables->AddItem(groupName, JsonKey::Title::UI::kLogoPosition, Vector2(400.0f, 300.0f));
	globalVariables->AddItem(groupName, JsonKey::Title::UI::kPressAPosition, Vector2(400.0f, 500.0f));

	SetUIPosition();

}

void TitleUI::Update()
{
#ifdef _DEBUG

	SetUIPosition();

#endif // _DEBUG

	pressA_->Update();

}

void TitleUI::Draw()
{
	spriteLogo_->Draw();
	pressA_->Draw();
}

void TitleUI::SetUIPosition()
{
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const std::string& groupName = JsonKey::Title::UI::kGroupName;

	spriteLogo_->SetPosition(globalVariables->GetVector2Value(groupName, JsonKey::Title::UI::kLogoPosition));
	pressA_->SetPosition(globalVariables->GetVector2Value(groupName, JsonKey::Title::UI::kPressAPosition));
}
