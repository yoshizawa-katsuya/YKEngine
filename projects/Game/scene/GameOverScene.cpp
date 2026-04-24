#include "GameOverScene.h"
#include "SceneManager.h"

#ifdef USE_IMGUI
#include "imgui/imgui.h"
#endif // USE_IMGUI

GameOverScene::~GameOverScene()
{
}

void GameOverScene::Initialize()
{
	dxCommon_ = YKEngine::DirectXCommon::GetInstance();
	audio_ = YKEngine::Audio::GetInstance();
	input_ = YKEngine::Input::GetInstance();
	spritePlatform_ = YKEngine::SpritePlatform::GetInstance();
	modelPlatform_ = YKEngine::ModelPlatform::GetInstance();
}

void GameOverScene::Update()
{
	#ifdef USE_IMGUI
	ImGui::Begin("Window");
	ImGui::Text("GameOver");
	ImGui::End();
#endif // USE_IMGUI
}

void GameOverScene::Draw()
{
}

void GameOverScene::Finalize()
{
}
