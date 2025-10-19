#pragma once
#include "BaseScene.h"
#include "AbstractSceneFactory.h"
#include <memory>

/// <summary>
/// シーンの管理クラス。
/// シーンの更新、描画、切り替えを行う。
/// シーンの生成はシーンファクトリーに任せる。
/// </summary>
class SceneManager
{
public:

	/// <summary>
	/// 終了処理。
	/// </summary>
	void Finalize();

	/// <summary>
	/// 更新処理。
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理。
	/// </summary>
	void Draw();

	//次シーン予約
	//void SetNextScene(BaseScene* nextScene) { nextScene_ = nextScene; }

	//シーンファクトリーのsetter
	void SetSceneFactory(AbstractSceneFactory* sceneFactory) { sceneFactory_ = sceneFactory; }

	/// <summary>
	/// シーンの変更。
	/// </summary>
	/// <param name="sceneName">変更先シーン名</param>
	void ChengeScene(const std::string& sceneName);

private:

	//今のシーン(実行中シーン)
	std::unique_ptr<BaseScene> scene_;

	//次のシーン
	BaseScene* nextScene_ = nullptr;

	//シーンファクトリー(借りてくる)
	AbstractSceneFactory* sceneFactory_ = nullptr;

};