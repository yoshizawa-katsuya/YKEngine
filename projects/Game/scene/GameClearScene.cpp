#include "GameClearScene.h"
#ifdef USE_IMGUI
#include "imgui.h"
#endif // USE_IMGUI

#include "input.h"
using namespace YKEngine;

GameClearScene::~GameClearScene() {
}

void GameClearScene::Initialize() {
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

void GameClearScene::Update()
{
	// 決定済みの場合はこれ以上の入力を受け付けない
	if (isDecided_) {
		return;
	}
	UpdateDecide();

	DrawImGui();
}

void GameClearScene::Draw()
{
}

void GameClearScene::DrawImGui()
{
	// ImGuiの描画
#ifdef USE_IMGUI
	ImGui::Begin("SELECT");
	ImGui::Text("-> Title");
	ImGui::End();
#endif
}

void GameClearScene::Finalize()
{
}



void GameClearScene::UpdateDecide()
{
	Input* input = Input::GetInstance();

	// 決定ボタン（スペース）
	bool decide = input->TriggerKey(DIK_SPACE);

	if (!decide) {
		return;
	}

	// 以降の入力を無視する（多重入力防止）
	isDecided_ = true;
	// タイトルシーンへ戻る
	sceneManager_->ChengeScene("TitleScene");


}
