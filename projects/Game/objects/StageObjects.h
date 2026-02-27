#pragma once
#include <map>
#include "InstancingObjects.h"
#include "LevelDataLoader.h"

namespace YKEngine
{
	class My3dObject;
}

class StageObjects
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(YKEngine::Camera* camera);

	/// <summary>
	/// スカイボックスの描画
	/// </summary>
	void DrawSkyBox(YKEngine::Camera* camera);

	/// <summary>
	/// インスタンシングオブジェクト取得
	/// </summary>
	/// <param name="objectDatas">オブジェクトデータ配列</param>
	void GetInstancingObject(const std::vector<YKEngine::ObjectData>& objectDatas);

	/// <summary>
	/// インスタンシングオブジェクトの描画
	/// </summary>
	void InstancingDraw(YKEngine::Camera* camera);

	/// <summary>
	/// スカイボックスのテクスチャハンドル取得
	/// </summary>
	/// <returns>テクスチャハンドル</returns>
	uint32_t GetTextureHandleSkyBox() const { return textureHandleSkyBox_; }

private:

	///<summary>
	///jsonから値を読み込む
	///</summary>
	void LoadFromJson();

	//テクスチャハンドル
	uint32_t textureHandleSkyBox_;

	//スカイボックス
	std::unique_ptr<YKEngine::My3dObject> skyBox_;

	//地面
	std::unique_ptr<YKEngine::My3dObject> ground_;

	//オブジェクト
	std::map<std::string, std::unique_ptr<YKEngine::InstancingObjects>> instancingObjects_;

};

