#include "ClearUI.h"
#include "Sprite.h"
#include "GlobalVariables.h"
#include "JsonKeys.h"
#include "Lerp.h"
#include "Easing.h"

using namespace YKEngine;

void ClearUI::Initialize()
{
	spriteClear_ = std::make_unique<Sprite>();
	spriteClear_->Initialize(TextureManager::GetInstance()->Load("./Resources/clear.png"));

	spritePressA_ = std::make_unique<Sprite>();
	spritePressA_->Initialize(TextureManager::GetInstance()->Load("./Resources/pressA.png"));

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

	t_ += 1.0f / 60.0f; // 60FPSで更新されることを想定

	//t_が1を超えないようにする
	if (t_ > 1.0f)
	{
		t_ = 1.0f;
	}

	//PressAを徐々に表示/非表示にする
	if (isPressAVisible_)
	{
		spritePressA_->SetColor({ 1.0f, 1.0f, 1.0f, Lerp(0.0f, 1.0f, EaseOutSine(t_)) });
	}
	else
	{
		spritePressA_->SetColor({ 1.0f, 1.0f, 1.0f, Lerp(1.0f, 0.0f, EaseInSine(t_)) });
	}

	//t_が1を超えたら、Aの表示/非表示を切り替える
	if (t_ >= 1.0f)
	{
		t_ = 0.0f;
		isPressAVisible_ = !isPressAVisible_;
	}
}

void ClearUI::Draw()
{
	spriteClear_->Draw();
	spritePressA_->Draw();
}

void ClearUI::SetUIPosition()
{
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const std::string& groupName = JsonKey::Clear::UI::kGroupName;

	spriteClear_->SetPosition(globalVariables->GetVector2Value(groupName, JsonKey::Clear::UI::kClearPosition));
	spritePressA_->SetPosition(globalVariables->GetVector2Value(groupName, JsonKey::Clear::UI::kPressAPosition));
}
