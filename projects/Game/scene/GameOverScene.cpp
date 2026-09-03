#include "GameOverScene.h"
#ifdef USE_IMGUI
#include "imgui.h"
#endif // USE_IMGUI

#include "input.h"
using namespace YKEngine;

GameOverScene::~GameOverScene() {
}

void GameOverScene::Initialize() {
	// デバイスの初期化
	dxCommon_ = DirectXCommon::GetInstance();
	audio_ = Audio::GetInstance();
	input_ = Input::GetInstance();
	spritePlatform_ = SpritePlatform::GetInstance();
	modelPlatform_ = ModelPlatform::GetInstance();

	// 状態初期化
	select_ = SELECT::kRetry;
	isDecided_ = false;
}

void GameOverScene::Update()
{
	// 決定済みの場合はこれ以上の入力を受け付けない
	if (isDecided_) {
		return;
	}

	UpdateSelect();
	UpdateDecide();

	DrawImGui();
}

void GameOverScene::Draw()
{
}

void GameOverScene::DrawImGui()
{
	// ImGuiの描画
#ifdef USE_IMGUI
	ImGui::Begin("SELECT");
	// 選択肢の表示
	if (select_ == SELECT::kRetry) {
		ImGui::Text("-> Retry");
		ImGui::Text("   Title");
	}
	else if (select_ == SELECT::kTitle) {
		ImGui::Text("   Retry");
		ImGui::Text("-> Title");
	}
	ImGui::End();
#endif
}

void GameOverScene::Finalize()
{
}

void GameOverScene::UpdateSelect()
{
	Input* input = Input::GetInstance();

	// 左右キーどちらかが押されたら選択項目を切り替える
	// Trigger系（そのフレームで押した瞬間だけtrue）を使い、連続切り替わりを防ぐ
	bool pressedLeft = input->TriggerKey(DIK_LEFT);
	bool pressedRight = input->TriggerKey(DIK_RIGHT);

	if (pressedLeft || pressedRight) {
		// 選択肢は2つしかないので、左右どちらでもトグルでOK
		if (select_ == SELECT::kRetry) {
			select_ = SELECT::kTitle;
		}
		else {
			select_ = SELECT::kRetry;
		}
	}
}

void GameOverScene::UpdateDecide()
{
	Input* input = Input::GetInstance();

	// 決定ボタン（スペース）
	bool decide = input->TriggerKey(DIK_SPACE);

	if (!decide) {
		return;
	}

	// 以降の入力を無視する（多重入力防止）
	isDecided_ = true;

	// 選択結果に応じてシーン遷移
	switch (select_) {
	case SELECT::kRetry:
		// リトライ：ゲームプレイシーンへ
		sceneManager_->ChengeScene("GameScene");
		break;

	case SELECT::kTitle:
		// タイトルシーンへ戻る
		sceneManager_->ChengeScene("TitleScene");
		break;
	}
}
