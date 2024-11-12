#pragma once
#include "BaseScene.h"
#include "AbstractSceneFactory.h"
#include <memory>

//シーン管理
class SceneManager
{
public:

	//シングルトンインスタンスの取得
	static SceneManager* GetInstance();

	//終了
	void Finalize();

	//更新
	void Update();

	//描画
	void Draw();

	//次シーン予約
	//void SetNextScene(BaseScene* nextScene) { nextScene_ = nextScene; }

	//シーンファクトリーのsetter
	void SetSceneFactory(AbstractSceneFactory* sceneFactory) { sceneFactory_ = sceneFactory; }

	//シーン切り替え
	void ChengeScene(const std::string& sceneName);

private:

	SceneManager() = default;
	~SceneManager() = default;
	SceneManager(SceneManager&) = delete;
	SceneManager& operator=(SceneManager&) = delete;

	//今のシーン(実行中シーン)
	std::unique_ptr<BaseScene> scene_;

	//次のシーン
	BaseScene* nextScene_ = nullptr;

	//シーンファクトリー(借りてくる)
	AbstractSceneFactory* sceneFactory_ = nullptr;

};