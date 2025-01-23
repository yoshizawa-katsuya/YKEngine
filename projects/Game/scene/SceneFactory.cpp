#include "SceneFactory.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "SelectScene.h"

BaseScene* SceneFactory::CreateScene(const std::string& sceneName)
{
	//次のシーンを生成
	BaseScene* newScene = nullptr;

	if (sceneName == "TitleScene") {
		newScene = new TitleScene();
	}
	else if (sceneName == "SelectScene") {
		newScene = new SelectScene();
	}
	else if (sceneName == "GameScene") {
		newScene = new GameScene();
	}
	return newScene;
}
