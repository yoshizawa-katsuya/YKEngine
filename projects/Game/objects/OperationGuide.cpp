#include "OperationGuide.h"
#include "Sprite.h"
#include "TextureManager.h"
#include "GlobalVariables.h"
#include "JsonKeys.h"
#include "Input.h"

using namespace YKEngine;

void OperationGuide::Initialize()
{
	input_ = Input::GetInstance();

	//グローバル変数に登録
	globalVariables_ = GlobalVariables::GetInstance();
	const std::string& groupName = JsonKey::OperationGuide::kGroupName;
	globalVariables_->CreateGroup(groupName);
	globalVariables_->AddItem(groupName, JsonKey::OperationGuide::kRTriggerPosition, Vector2(1000.0f, 500.0f));
	globalVariables_->AddItem(groupName, JsonKey::OperationGuide::kLTriggerPosition, Vector2(200.0f, 500.0f));
	globalVariables_->AddItem(groupName, JsonKey::OperationGuide::kRStickPosition, Vector2(1000.0f, 300.0f));
	globalVariables_->AddItem(groupName, JsonKey::OperationGuide::kLStickPosition, Vector2(200.0f, 300.0f));
	globalVariables_->AddItem(groupName, JsonKey::OperationGuide::kPlayerIconPosition, Vector2(600.0f, 500.0f));
	globalVariables_->AddItem(groupName, JsonKey::OperationGuide::kDodgeIconPosition, Vector2(600.0f, 300.0f));
	globalVariables_->AddItem(groupName, JsonKey::OperationGuide::kShotIconPosition, Vector2(600.0f, 400.0f));
	globalVariables_->AddItem(groupName, JsonKey::OperationGuide::kReticleIconPosition, Vector2(600.0f, 200.0f));
	globalVariables_->AddItem(groupName, JsonKey::OperationGuide::kStickMoveValue, 10.0f);

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
	
	//操作に応じてスティックのアイコンを移動させる
	MoveStickIcon();
	
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
	globalVariables_ = GlobalVariables::GetInstance();
	const std::string& groupName = JsonKey::OperationGuide::kGroupName;
	RTriggerSprite_->SetPosition(globalVariables_->GetVector2Value(groupName, JsonKey::OperationGuide::kRTriggerPosition));
	LTriggerSprite_->SetPosition(globalVariables_->GetVector2Value(groupName, JsonKey::OperationGuide::kLTriggerPosition));
	RStickSprite_->SetPosition(globalVariables_->GetVector2Value(groupName, JsonKey::OperationGuide::kRStickPosition));
	LStickSprite_->SetPosition(globalVariables_->GetVector2Value(groupName, JsonKey::OperationGuide::kLStickPosition));

	playerIconSprite_->SetPosition(globalVariables_->GetVector2Value(groupName, JsonKey::OperationGuide::kPlayerIconPosition));
	dodgeIconSprite_->SetPosition(globalVariables_->GetVector2Value(groupName, JsonKey::OperationGuide::kDodgeIconPosition));
	shotIconSprite_->SetPosition(globalVariables_->GetVector2Value(groupName, JsonKey::OperationGuide::kShotIconPosition));
	reticleIconSprite_->SetPosition(globalVariables_->GetVector2Value(groupName, JsonKey::OperationGuide::kReticleIconPosition));
}

void OperationGuide::MoveStickIcon()
{
	//キャラクターの移動ベクトル
	Vector2 move = { 0.0f, 0.0f };

	//自機の移動ベクトルを取得
	move.x = input_->GetLeftStickX();
	move.y = -input_->GetLeftStickY();	//上方向はy軸の負の方向とする

	if (move.x == 0 && move.y == 0)
	{
		//左スティックがニュートラルなら、キーボード入力を確認
		//押した方向で移動ベクトルを変更(左右)
		if (input_->PushKey(DIK_A)) {
			move.x = -1.0f;
		}
		else if (input_->PushKey(DIK_D)) {
			move.x = 1.0f;
		}

		// 押した方向で移動ベクトルを変更(上下)
		if (input_->PushKey(DIK_S)) {
			move.y = 1.0f;
		}
		else if (input_->PushKey(DIK_W)) {
			move.y = -1.0f;		//上方向はy軸の負の方向とする
		}

		move = Normalize(move); //移動ベクトルの正規化

	}

	const std::string& groupName = JsonKey::OperationGuide::kGroupName;
	const float stickMoveValue = globalVariables_->GetFloatValue(groupName, JsonKey::OperationGuide::kStickMoveValue);

	const Vector2 LStickPosition = globalVariables_->GetVector2Value(groupName, JsonKey::OperationGuide::kLStickPosition);
	LStickSprite_->SetPosition(LStickPosition + move * stickMoveValue); //スティックの位置を移動ベクトルに応じて変化させる

	//照準の移動ベクトルを取得
	move.x = input_->GetRightStickX();
	move.y = -input_->GetRightStickY();	//上方向はy軸の負の方向とする

	if (move.x == 0 && move.y == 0)
	{
		//左スティックがニュートラルなら、キーボード入力を確認
		//押した方向で移動ベクトルを変更(左右)
		if (input_->PushKey(DIK_LEFT)) {
			move.x = -1.0f;
		}
		else if (input_->PushKey(DIK_RIGHT)) {
			move.x = 1.0f;
		}

		// 押した方向で移動ベクトルを変更(上下)
		if (input_->PushKey(DIK_DOWN)) {
			move.y = 1.0f;
		}
		else if (input_->PushKey(DIK_UP)) {
			move.y = -1.0f;		//上方向はy軸の負の方向とする
		}

		move = Normalize(move); //移動ベクトルの正規化

	}

	const Vector2 RStickPosition = globalVariables_->GetVector2Value(groupName, JsonKey::OperationGuide::kRStickPosition);
	RStickSprite_->SetPosition(RStickPosition + move * stickMoveValue); //スティックの位置を移動ベクトルに応じて変化させる
}
