#include "GameOverUI.h"
#include "Sprite.h"
#include "GlobalVariables.h"
#include "JsonKeys.h"

using namespace YKEngine;

void GameOverUI::Initialize()
{
	spriteGameOver_ = std::make_unique<Sprite>();
	spriteGameOver_->Initialize(TextureManager::GetInstance()->Load("./Resources/gameover.png"));

	pressA_ = std::make_unique<PressA>();
	pressA_->Initialize();

	//グローバル変数に登録
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const std::string& groupName = JsonKey::GameOver::UI::kGroupName;
	globalVariables->CreateGroup(groupName);
	globalVariables->AddItem(groupName, JsonKey::GameOver::UI::kGameOverPosition, Vector2(400.0f, 300.0f));
	globalVariables->AddItem(groupName, JsonKey::GameOver::UI::kPressAPosition, Vector2(400.0f, 500.0f));

	SetUIPosition();
}

void GameOverUI::Update()
{
#ifdef _DEBUG

	SetUIPosition();

#endif // _DEBUG

	pressA_->Update();

}

void GameOverUI::Draw()
{
	spriteGameOver_->Draw();
	pressA_->Draw();
}

void GameOverUI::SetUIPosition()
{
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const std::string& groupName = JsonKey::GameOver::UI::kGroupName;

	spriteGameOver_->SetPosition(globalVariables->GetVector2Value(groupName, JsonKey::GameOver::UI::kGameOverPosition));
	pressA_->SetPosition(globalVariables->GetVector2Value(groupName, JsonKey::GameOver::UI::kPressAPosition));
}
