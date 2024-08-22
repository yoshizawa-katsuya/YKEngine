#pragma once

#include "AbstractSceneFactory.h"

class SceneFactory : public AbstractSceneFactory
{
public:
	///<summary>
	///シーン生成
	/// </summary>
	/// <param name="sceneName">シーン名</param>
	/// <returns>生成したシーン</returns>
	BaseScene* CreateScene(const std::string& sceneName) override;
};

