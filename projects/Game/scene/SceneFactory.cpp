#include "SceneFactory.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "GameOverScene.h"
#include "ClearScene.h"

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
	else if (sceneName == "GameOverScene") {
		newScene = new GameOverScene();
	}
	else if (sceneName == "ClearScene") {
		newScene = new ClearScene();
	}

	return newScene;
}
