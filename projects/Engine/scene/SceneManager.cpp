#include "SceneManager.h"
#include <cassert>

SceneManager* SceneManager::GetInstance()
{
	static SceneManager instance;
	return &instance;
}

void SceneManager::Finalize()
{

}

void SceneManager::Update()
{

	//次シーンの予約があるなら
	if (nextScene_) {
		//シーン切り替え
		scene_.reset(nextScene_);
		nextScene_ = nullptr;

		//シーンマネージャをセット
		scene_->SetSceneManager(this);

		//次シーンを初期化する
		scene_->Initialize();
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
	nextScene_ = sceneFactory_->CreateScene(sceneName);

	if (!scene_) {

		//シーン切り替え
		scene_.reset(nextScene_);
		nextScene_ = nullptr;

		//シーンマネージャをセット
		scene_->SetSceneManager(this);

		//次シーンを初期化する
		scene_->Initialize();

	}
}
