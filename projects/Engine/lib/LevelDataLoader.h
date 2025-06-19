#pragma once
#include "Struct.h"

struct ObjectData
{
	std::string fileName;
	std::string filePath;
	EulerTransform transform;
};

//自キャラの生成データ
struct PlayerSpawnData
{
	EulerTransform transform;
};

//スプライン曲線の制御点データ
struct SplineData
{
	std::list<Vector3> controlPoints;
};

struct LevelData
{
	std::list<ObjectData> objects;

	//自キャラ配列
	std::list<PlayerSpawnData> playerSpawns;

	//スプライン曲線の制御点データ
	std::list<SplineData> splines;
};

LevelData* LevelDataLoad(const std::string& kDefaultBaseDirectory, const std::string& fileName, const std::string& kExtension);


