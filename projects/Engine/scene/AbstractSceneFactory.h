#pragma once

#include "BaseScene.h"
#include <string>
#include <memory>

namespace YKEngine
{

/// <summary>
/// シーン工場(概念)
/// シーンを生成するインターフェースを提供する。
/// 継承して使用する。
/// </summary>
class AbstractSceneFactory
{
public:

	/// <summary>
	/// デストラクタ。純粋仮想関数。
	/// </summary>
	virtual ~AbstractSceneFactory() = default;
	
	/// <summary>
	/// シーンを生成する。純粋仮想関数。
	/// </summary>
	/// <param name="sceneName">生成するシーンの名前</param>
	/// <returns>生成されたシーンのポインタ</returns>
	virtual std::unique_ptr<BaseScene> CreateScene(const std::string& sceneName) = 0;

};

} // namespace YKEngine