#pragma once
#include <map>
#include "InstancingObjects.h"
#include "LevelDataLoader.h"
#include "CuboidObstacle.h"

namespace YKEngine
{
	class My3dObject;
}

/// <summary>
/// ステージ上のオブジェクトを管理するクラス。
/// </summary>
class StageObjects
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="isDayTime">昼ならtrue</param>
	void Initialize(bool isDayTime = false);

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
	/// トリプラナーマッピングを使用するインスタンシングオブジェクトの描画
	/// </summary>
	void InstancingTriplanarDraw(YKEngine::Camera* camera);

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

	//モデル
	std::shared_ptr<YKEngine::BaseModel> skyBoxModel_;
	std::shared_ptr<YKEngine::BaseModel> groundModel_;

	//スカイボックス
	std::unique_ptr<YKEngine::My3dObject> skyBox_;

	//地面
	std::unique_ptr<YKEngine::My3dObject> ground_;

	//オブジェクト
	std::map<std::string, std::unique_ptr<YKEngine::InstancingObjects>> instancingObjects_;
	std::map<std::string, std::unique_ptr<YKEngine::InstancingObjects>> instancingTriplanarObjects_;	//トリプラナーマッピングを使用するインスタンシングオブジェクト

	//直方体の障害物
	std::vector<std::unique_ptr<CuboidObstacle>> cuboidObstacles_;

	//昼ならtrue
	bool isDayTime_;

	using InstancingObjectsFactory = std::function<void(StageObjects*, const YKEngine::ObjectData&, const std::string&)>;

	const std::unordered_map<std::string, InstancingObjectsFactory>& GetInstancingObjectsFactoryMap() const;

};

