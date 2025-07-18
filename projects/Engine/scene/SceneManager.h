#pragma once
#include "BaseScene.h"
#include "AbstractSceneFactory.h"
#include <memory>

struct SceneData {
	uint32_t selectedTutorial = 0;
	uint32_t selectedBundle = 0;
	uint32_t selectedStage = 0;
	bool selectClick = true;
};

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

	BaseScene* GetCurrentScene();
	
	void SetSelectedTutorial(uint32_t value) { sceneData_.selectedTutorial = value; }
	void SetSelectedBundle(uint32_t value) { sceneData_.selectedBundle = value; }
	void SetSelectedStage(uint32_t value) { sceneData_.selectedStage = value; }
	void SetSelectClick(bool value) { sceneData_.selectClick = value; }
	const SceneData& GetSceneData() const { return sceneData_; }

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

	SceneData sceneData_;
};