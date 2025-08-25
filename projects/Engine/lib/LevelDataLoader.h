#pragma once
#include "Struct.h"
#include "json.hpp"

struct ObjectData
{
	std::string fileName;
	std::string filePath;
	EulerTransform transform;
	std::optional<uint32_t> waveNum = std::nullopt; // ウェーブ番号（オプション）
	std::optional<float> speed = std::nullopt; // スピード（オプション）
};

//スプライン曲線の制御点データ
struct SplineData
{
	std::vector<Vector3> controlPoints;
};

//自キャラの生成データ
struct PlayerSpawnData
{
	EulerTransform transform;
};

//敵の生成データ
struct EnemySpawnData
{
	EulerTransform transform{};
	std::optional<float> waitTime = std::nullopt; // 待機時間（オプション）
	std::optional<uint32_t> waveNum = std::nullopt; // ウェーブ番号（オプション）
	std::optional<SplineData> spline = std::nullopt; // スプライン曲線（オプション）
};

struct LevelData
{
	std::vector<ObjectData> objects;

	//自キャラ配列
	std::vector<PlayerSpawnData> playerSpawns;

	//敵配列
	std::vector<EnemySpawnData> enemySpawns;

	//スプライン曲線の制御点データ
	std::vector<SplineData> splines;
};

LevelData* LevelDataLoad(const std::string& kDefaultBaseDirectory, const std::string& fileName, const std::string& kExtension);

EulerTransform TranformLoad(nlohmann::json& transformData);
