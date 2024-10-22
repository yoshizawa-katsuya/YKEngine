#include "SceneFactory.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "GameOverScene.h"
#include "GameClearScene.h"

BaseScene* SceneFactory::CreateScene(const std::string& sceneName)
{
	//次のシーンを生成
	BaseScene* newScene = nullptr;

	if (sceneName == "TITLE") {
		newScene = new TitleScene();
	}
	else if (sceneName == "GAMEPLAY") {
		newScene = new GameScene();
	}
	else if (sceneName == "GAMEOVER") {
		newScene = new GameOverScene();
	}
	else if (sceneName == "GAMECLEAR") {
		newScene = new GameClearScene();
	}

	return newScene;
}
