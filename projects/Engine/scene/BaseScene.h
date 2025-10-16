#pragma once

//前方宣言
class SceneManager;

/// <summary>
/// シーンの基底クラス
/// </summary>
class BaseScene
{
public:

	virtual ~BaseScene() = default;

	virtual void Initialize() = 0;

	virtual void Update() = 0;

	virtual void Draw() = 0;

	virtual void Finalize() = 0;

	virtual void SetSceneManager(SceneManager* sceneManager) { sceneManager_ = sceneManager; }

protected:

	SceneManager* sceneManager_ = nullptr;

};

