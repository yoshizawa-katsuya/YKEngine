#include "SceneFactory.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "GameClearScene.h"
#include "GameOverScene.h"
#include "StageSelectScene.h"

BaseScene* SceneFactory::CreateScene(const std::string& sceneName)
{
	//次のシーンを生成
	BaseScene* newScene = nullptr;

	if (sceneName == "TitleScene") {
		newScene = new TitleScene();
	}
	else if (sceneName == "GameScene") {
		newScene = new GameScene();
	}
	else if (sceneName == "GameClearScene") {
		newScene = new GameClearScene();
	}
	else if (sceneName == "GameOverScene") {
		newScene = new GameOverScene();
	}
	else if (sceneName == "StageSelectScene") {
		newScene = new StageSelectScene();
	}

	return newScene;
}
