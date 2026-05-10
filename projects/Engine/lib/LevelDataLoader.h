#pragma once
#include "Struct.h"
#include "json.hpp"

namespace YKEngine
{

/// <summary>
/// オブジェクトの生成データ。
/// </summary>
/// <param name="fileName">モデルファイル名</param>
/// <param name="filePath">モデルファイルパス</param>
/// <param name="transform">位置、回転、スケール</param>
struct ObjectData
{
	std::string fileName;
	std::string filePath;
	EulerTransform transform;
	std::optional<uint32_t> waveNum = std::nullopt; // ウェーブ番号（オプション）
	std::optional<float> speed = std::nullopt; // スピード（オプション）
	bool hasCollider = false; // コライダーの有無
};

/// <summary>
/// スプライン曲線の制御点データ。
/// </summary>
/// <param name="controlPoints">制御点配列</param>
struct SplineData
{
	std::vector<Vector3> controlPoints;
};

/// <summary>
/// 自キャラの生成データ。
/// </summary>
/// <param name="transform">位置、回転、スケール</param>
struct PlayerSpawnData
{
	EulerTransform transform;
};

/// <summary>
/// 敵の生成データ。
/// </summary>
/// <param name="transform">位置、回転、スケール</param>
/// <param name="waitTime">待機時間（オプション）</param>
/// <param name="waveNum">ウェーブ番号（オプション）</param>
/// <param name="speed">スピード（オプション）</param>
/// <param name="spline">スプライン曲線（オプション）</param>
struct EnemySpawnData
{
	EulerTransform transform{};
	std::string type; // 敵の種類
	std::optional<float> waitTime = std::nullopt; // 待機時間（オプション）
	std::optional<uint32_t> waveNum = std::nullopt; // ウェーブ番号（オプション）
	std::optional<float> speed = std::nullopt; // スピード（オプション）
	std::optional<SplineData> spline = std::nullopt; // スプライン曲線（オプション）
};

/// <summary>
/// レベルデータ。
/// </summary>
/// <param name="objects">オブジェクト配列</param>
/// <param name="playerSpawns">自キャラ配列</param>
/// <param name="enemySpawns">敵配列</param>
/// <param name="splines">スプライン曲線配列</param>
struct LevelData
{
	std::vector<ObjectData> objects;

	//自キャラ配列
	std::vector<PlayerSpawnData> playerSpawns;

	//敵配列
	std::vector<EnemySpawnData> enemySpawns;

	//スプライン曲線
	std::vector<SplineData> splines;
};

/// <summary>
/// レベルデータのロードクラス。
/// </summary>
class LevelDataLoader
{
public:

	/// <summary>
	/// レベルデータのロード。
	/// </summary>
	/// <param name="kDefaultBaseDirectory">デフォルトのベースディレクトリ</param>
	/// <param name="fileName">レベルデータファイル名</param>
	/// <param name="kExtension">レベルデータファイル拡張子</param>
	/// <returns>レベルデータ</returns>
	void LevelDataLoad(const std::string& kDefaultBaseDirectory, const std::string& fileName, const std::string& kExtension);

	const LevelData& GetLevelData() const { return levelData_; }

private:
	
	/// <summary>
	/// オブジェクトデータのロード。
	/// </summary>
	/// <param name="object">オブジェクトのJSONデータ</param>
	void ObjectDateLoad(const nlohmann::json& object);

	/// <summary>
	/// 自機の生成データのロード。
	/// </summary>
	/// <param name="playerSpawn">自機の生成データのJSONデータ</param>
	void PlayerSpawnDataLoad(const nlohmann::json& playerSpawn);

	/// <summary>
	/// 敵の生成データのロード。
	/// </summary>
	/// <param name="enemySpawn">敵の生成データのJSONデータ</param>
	/// <param name="type">敵の種類</param>
	void EnemySpawnDataLoad(const nlohmann::json& enemySpawn, const std::string& type, uint32_t waveNum);

	/// <summary>
	/// スプライン曲線のロード。
	/// </summary>
	/// <param name="spline">スプライン曲線のJSONデータ</param>
	void SplineDataLoad(const nlohmann::json& spline);

	/// <summary>
	/// Transformのロード。
	/// </summary>
	/// <param name="transformData">TransformのJSONデータ</param>
	/// <returns>Transformデータ</returns>
	EulerTransform TranformLoad(const nlohmann::json& transformData);

	//レベルデータ
	LevelData levelData_;

	uint32_t enemySpawnEvenNum_ = 0; // 敵出現イベントの番号

};


}	// namespace YKEngine