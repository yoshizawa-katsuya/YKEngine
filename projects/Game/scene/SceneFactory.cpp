#include "SceneFactory.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "GameOverScene.h"
#include "ClearScene.h"
#include "TutorialScene.h"

BaseScene* SceneFactory::CreateScene(const std::string& sceneName)
{
	//次のシーンを生成
	BaseScene* newScene = nullptr;

	if (sceneName == "TitleScene") {
		newScene = new TitleScene();
	}
	else if (sceneName == "GameScene01") {
		newScene = new GameScene();
		GameScene::stageNum_ = 1;
	}
	else if (sceneName == "GameScene02") {
		newScene = new GameScene();
		GameScene::stageNum_ = 2;
	}
	else if (sceneName == "GameOverScene") {
		newScene = new GameOverScene();
	}
	else if (sceneName == "ClearScene") {
		newScene = new ClearScene();
	}
	else if (sceneName == "TutorialScene") {
		newScene = new TutorialScene();
	}

	return newScene;
}
