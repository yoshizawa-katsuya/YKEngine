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

struct LevelData
{
	std::list<ObjectData> objects;

	//自キャラ配列
	std::list<PlayerSpawnData> playerSpawns;
};

LevelData* LevelDataLoad(const std::string& kDefaultBaseDirectory, const std::string& fileName, const std::string& kExtension);


