#pragma once
#include "My3dObject.h"
#include "WorldTransform.h"
#include "Input.h"

/// <summary>
/// プレイヤークラス
/// </summary>
class BasePlayer
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

	/// <summary>
	/// 移動。
	/// </summary>
	virtual void Move() = 0;

	//入力
	YKEngine::Input* input_ = nullptr;

	//Transform変数を作る
	YKEngine::WorldTransform worldTransform_;

	std::unique_ptr<YKEngine::My3dObject> object_;

};

