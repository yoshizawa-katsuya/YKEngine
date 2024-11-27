#include "SceneFactory.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "TutorialScene.h"
#include "GameClearScene.h"
#include "GameOverScene.h"

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
	else if (sceneName == "Tutorial") {
		newScene = new TutorialScene();
	}
	else if (sceneName == "GameClear") {
		newScene = new GameClearScene();
	}
	else if (sceneName == "GameOver") {
		newScene = new GameOverScene();
	}

	return newScene;
}
