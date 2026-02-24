#include "TitleUI.h"
#include "Sprite.h"
#include "GlobalVariables.h"
#include "JsonKeys.h"

using namespace YKEngine;

void TitleUI::Initialize()
{

	spriteLogo_ = std::make_unique<Sprite>();
	spriteLogo_->Initialize(TextureManager::GetInstance()->Load("./Resources/titleLogo.png"));

	spritePressA_ = std::make_unique<Sprite>();
	spritePressA_->Initialize(TextureManager::GetInstance()->Load("./Resources/pressA.png"));

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
}

void TitleUI::Draw()
{
	spriteLogo_->Draw();
	spritePressA_->Draw();
}

void TitleUI::SetUIPosition()
{
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const std::string& groupName = JsonKey::Title::UI::kGroupName;

	spriteLogo_->SetPosition(globalVariables->GetVector2Value(groupName, JsonKey::Title::UI::kLogoPosition));
	spritePressA_->SetPosition(globalVariables->GetVector2Value(groupName, JsonKey::Title::UI::kPressAPosition));
}
