#pragma once

#include "BaseScene.h"
#include <string>

/// <summary>
/// シーン工場(概念)
/// シーンを生成するインターフェースを提供する。
/// 継承して使用する。
/// </summary>
class AbstractSceneFactory
{
public:

	//仮想デストラクタ
	virtual ~AbstractSceneFactory() = default;
	//シーン生成
	virtual BaseScene* CreateScene(const std::string& sceneName) = 0;

};