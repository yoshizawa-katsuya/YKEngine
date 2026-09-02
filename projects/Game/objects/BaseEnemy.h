#pragma once
#include "My3dObject.h"
#include "WorldTransform.h"
#include "Input.h"

/// <summary>
/// エネミークラス
/// </summary>
class BaseEnemy
{
public:

	/// <summary>
	/// 初期化。
	/// </summary>
	/// <param name="model">モデル</param>
	void Initialize(YKEngine::BaseModel* model);

	/// <summary>
	/// 更新。
	/// </summary>
	void Update();

	/// <summary>
	/// 描画。
	/// </summary>
	/// <param name="camera">カメラ</param>
	void Draw(YKEngine::Camera* camera);

protected:

	//Transform変数を作る
	YKEngine::WorldTransform worldTransform_;

	std::unique_ptr<YKEngine::My3dObject> object_;

};