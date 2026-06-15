#include "SceneFactory.h"
#include "TitleScene.h"
#include "GameScene.h"

using namespace YKEngine;

std::unique_ptr<BaseScene> SceneFactory::CreateScene(const std::string& sceneName)
{
	//次のシーンを生成
	std::unique_ptr<BaseScene> newScene = nullptr;

	if (sceneName == "TitleScene") {
		newScene = std::make_unique<TitleScene>();
	}
	else if (sceneName == "GameScene") {
		newScene = std::make_unique<GameScene>();
	}

	return newScene;
}
