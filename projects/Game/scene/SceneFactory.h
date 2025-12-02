#pragma once

#include "AbstractSceneFactory.h"

/// <summary>
/// シーンの生成を担当するクラス。
/// </summary>
class SceneFactory : public AbstractSceneFactory
{
public:
	///<summary>
	///シーン生成
	/// </summary>
	/// <param name="sceneName">シーン名</param>
	/// <returns>生成したシーン</returns>
	std::unique_ptr<BaseScene> CreateScene(const std::string& sceneName) override;
};

