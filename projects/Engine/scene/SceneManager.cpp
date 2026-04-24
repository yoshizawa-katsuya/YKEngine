#include "SceneManager.h"
#include "ParticleManager.h"
#include <cassert>
#include <imgui.h>

using namespace YKEngine;

void SceneManager::Finalize()
{

}

void SceneManager::Update()
{

	//次シーンの予約があるなら
	if (nextScene_) 
	{
		ChangeSceneProcess();

		//パーティクルを全削除
		ParticleManager::GetInstance()->ClearAllParticles();
	}

	//実行中シーンを更新する
	scene_->Update();

	DrawImGui();
}

void SceneManager::Draw()
{

	scene_->Draw();

}

void YKEngine::SceneManager::DrawImGui()
{

#ifdef USE_IMGUI

	ImGui::Begin("SceneManager");
	// シーン切り替え
	
	// 各シーンへの遷移ボタン
	if (ImGui::Button("TITLE")) {
		ChengeScene("TitleScene");
	}
	ImGui::SameLine();
	if (ImGui::Button("GAMEPLAY")) {
		ChengeScene("GameScene");
	}
	ImGui::SameLine();
	if (ImGui::Button("GAMEOVER")) {
		ChengeScene("GameOverScene");
	}

	ImGui::End();
#endif
}

void SceneManager::ChengeScene(const std::string& sceneName)
{
	
	assert(sceneFactory_);
	assert(nextScene_ == nullptr);

	//次シーンを生成
	nextScene_ = std::move(sceneFactory_->CreateScene(sceneName));

	if (!scene_) 
	{
		ChangeSceneProcess();
	}
}

void SceneManager::ChangeSceneProcess()
{
	//シーン切り替え
	scene_ = std::move(nextScene_);
	nextScene_.reset();

	//シーンマネージャをセット
	scene_->SetSceneManager(this);

	//次シーンを初期化する
	scene_->Initialize();
}
