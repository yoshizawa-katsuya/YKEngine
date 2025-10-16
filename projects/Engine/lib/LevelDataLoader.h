#pragma once
#include "Struct.h"
#include "json.hpp"

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
struct EnemySpawnData
{
	EulerTransform transform{};
	std::optional<float> waitTime = std::nullopt; // 待機時間（オプション）
	std::optional<uint32_t> waveNum = std::nullopt; // ウェーブ番号（オプション）
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

LevelData* LevelDataLoad(const std::string& kDefaultBaseDirectory, const std::string& fileName, const std::string& kExtension);

EulerTransform TranformLoad(nlohmann::json& transformData);
