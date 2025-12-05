#include "SceneFactory.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "GameOverScene.h"
#include "ClearScene.h"

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
	else if (sceneName == "GameOverScene") {
		newScene = std::make_unique<GameOverScene>();
	}
	else if (sceneName == "ClearScene") {
		newScene = std::make_unique<ClearScene>();
	}
	

	return newScene;
}
