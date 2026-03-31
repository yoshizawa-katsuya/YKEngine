#include "OperationGuide.h"
#include "Sprite.h"
#include "TextureManager.h"
#include "GlobalVariables.h"
#include "JsonKeys.h"

using namespace YKEngine;

void OperationGuide::Initialize()
{
	//グローバル変数に登録
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const std::string& groupName = JsonKey::OperationGuide::kGroupName;
	globalVariables->CreateGroup(groupName);
	globalVariables->AddItem(groupName, JsonKey::OperationGuide::kRTriggerPosition, Vector2(1000.0f, 500.0f));
	globalVariables->AddItem(groupName, JsonKey::OperationGuide::kLTriggerPosition, Vector2(200.0f, 500.0f));
	globalVariables->AddItem(groupName, JsonKey::OperationGuide::kRStickPosition, Vector2(1000.0f, 300.0f));
	globalVariables->AddItem(groupName, JsonKey::OperationGuide::kLStickPosition, Vector2(200.0f, 300.0f));
	globalVariables->AddItem(groupName, JsonKey::OperationGuide::kPlayerIconPosition, Vector2(600.0f, 500.0f));
	globalVariables->AddItem(groupName, JsonKey::OperationGuide::kDodgeIconPosition, Vector2(600.0f, 300.0f));
	globalVariables->AddItem(groupName, JsonKey::OperationGuide::kShotIconPosition, Vector2(600.0f, 400.0f));
	globalVariables->AddItem(groupName, JsonKey::OperationGuide::kReticleIconPosition, Vector2(600.0f, 200.0f));

	isDraw_ = false;
	TextureManager* textureManager = TextureManager::GetInstance();

	//スプライトの生成
	RTriggerSprite_ = std::make_unique<Sprite>();
	RTriggerSprite_->Initialize(textureManager->Load("./Resources/RTrigger.png"));

	LTriggerSprite_ = std::make_unique<Sprite>();
	LTriggerSprite_->Initialize(textureManager->Load("./Resources/LTrigger.png"));

	RStickSprite_ = std::make_unique<Sprite>();
	RStickSprite_->Initialize(textureManager->Load("./Resources/RStick.png"));

	LStickSprite_ = std::make_unique<Sprite>();
	LStickSprite_->Initialize(textureManager->Load("./Resources/LStick.png"));

	playerIconSprite_ = std::make_unique<Sprite>();
	playerIconSprite_->Initialize(textureManager->Load("./Resources/playerIcon.png"));

	dodgeIconSprite_ = std::make_unique<Sprite>();
	dodgeIconSprite_->Initialize(textureManager->Load("./Resources/dodgeIcon.png"));

	shotIconSprite_ = std::make_unique<Sprite>();
	shotIconSprite_->Initialize(textureManager->Load("./Resources/shotIcon.png"));

	reticleIconSprite_ = std::make_unique<Sprite>();
	reticleIconSprite_->Initialize(textureManager->Load("./Resources/reticleIcon.png"));

	SetUIPositions();
}

void OperationGuide::Update()
{
#ifdef _DEBUG

	SetUIPositions();

#endif // DEBUG
}

void OperationGuide::Draw()
{

	if (!isDraw_)
	{
		return;
	}
	//操作説明HUDの描画
	RTriggerSprite_->Draw();
	LTriggerSprite_->Draw();
	RStickSprite_->Draw();
	LStickSprite_->Draw();

	//アイコンの描画
	playerIconSprite_->Draw();
	dodgeIconSprite_->Draw();
	shotIconSprite_->Draw();
	reticleIconSprite_->Draw();

}

void OperationGuide::ExitStart()
{
	isDraw_ = true;
}

void OperationGuide::SetUIPositions()
{
	//グローバル変数に登録
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const std::string& groupName = JsonKey::OperationGuide::kGroupName;
	RTriggerSprite_->SetPosition(globalVariables->GetVector2Value(groupName, JsonKey::OperationGuide::kRTriggerPosition));
	LTriggerSprite_->SetPosition(globalVariables->GetVector2Value(groupName, JsonKey::OperationGuide::kLTriggerPosition));
	RStickSprite_->SetPosition(globalVariables->GetVector2Value(groupName, JsonKey::OperationGuide::kRStickPosition));
	LStickSprite_->SetPosition(globalVariables->GetVector2Value(groupName, JsonKey::OperationGuide::kLStickPosition));

	playerIconSprite_->SetPosition(globalVariables->GetVector2Value(groupName, JsonKey::OperationGuide::kPlayerIconPosition));
	dodgeIconSprite_->SetPosition(globalVariables->GetVector2Value(groupName, JsonKey::OperationGuide::kDodgeIconPosition));
	shotIconSprite_->SetPosition(globalVariables->GetVector2Value(groupName, JsonKey::OperationGuide::kShotIconPosition));
	reticleIconSprite_->SetPosition(globalVariables->GetVector2Value(groupName, JsonKey::OperationGuide::kReticleIconPosition));
}
