#include "SceneManager.h"
#include "ParticleManager.h"
#include <cassert>

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

}

void SceneManager::Draw()
{

	scene_->Draw();

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
