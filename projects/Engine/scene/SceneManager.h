#pragma once
#include "BaseScene.h"
#include "AbstractSceneFactory.h"
#include <memory>

namespace YKEngine
{

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

	/// <summary>
	/// ImGuiの描画処理。
	/// </summary>
	void DrawImGui();

	//次シーン予約
	//void SetNextScene(BaseScene* nextScene) { nextScene_ = nextScene; }

	//シーンファクトリーのsetter
	void SetSceneFactory(AbstractSceneFactory* sceneFactory) { sceneFactory_ = sceneFactory; }

	/// <summary>
	/// シーンの変更。
	/// </summary>
	/// <param name="sceneName">変更先シーン名</param>
	void ChengeScene(const std::string& sceneName);

	/// <summary>
	/// セッター
	/// </summary>
	// 難易度の設定
	void SetDifficulty(uint32_t difficulty) { difficulty_ = difficulty; }


	/// <summary>
	/// ゲッター
	/// </summary>
	// 難易度の取得
	uint32_t GetDifficulty() const { return difficulty_; }

private:


	/// <summary>
	/// シーンの切り替え処理。
	/// </summary>
	void ChangeSceneProcess();

	//今のシーン(実行中シーン)
	std::unique_ptr<BaseScene> scene_;

	//次のシーン
	std::unique_ptr<BaseScene> nextScene_;

	//シーンファクトリー(借りてくる)
	AbstractSceneFactory* sceneFactory_ = nullptr;

	uint32_t difficulty_ = 0; // 難易度の変数
};

}