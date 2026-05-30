#pragma once

namespace YKEngine
{

//前方宣言
class SceneManager;

/// <summary>
/// シーンの基底クラス
/// </summary>
class BaseScene
{
public:

	/// <summary>
	/// デストラクタ。純粋仮想関数として宣言。
	/// </summary>
	virtual ~BaseScene() = default;

	/// <summary>
	/// 初期化処理。純粋仮想関数。
	/// </summary>
	virtual void Initialize() = 0;

	/// <summary>
	/// 更新処理。純粋仮想関数。
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// 描画処理。純粋仮想関数。
	/// </summary>
	virtual void Draw() = 0;

	/// <summary>
	/// 終了処理。純粋仮想関数。
	/// </summary>
	virtual void Finalize() = 0;

	virtual void SetSceneManager(SceneManager* sceneManager) { sceneManager_ = sceneManager; }

protected:

	SceneManager* sceneManager_ = nullptr;

};

} // namespace YKEngine