#include "LevelDataLoader.h"
#include "fstream"
#include "cassert"
#include <numbers>

using namespace YKEngine;

void LevelDataLoader::LevelDataLoad(const std::string& kDefaultBaseDirectory, const std::string& fileName, const std::string& kExtension)
{

	//連結してフルパスを得る
	const std::string fullpath = kDefaultBaseDirectory + fileName + kExtension;

	//ファイルストリーム
	std::ifstream file;

	//ファイルを開く
	file.open(fullpath);
	//ファイルオープン失敗をチェック
	if (file.fail()) {
		assert(0);
	}

	//JSON文字列から解凍したデータ
	nlohmann::json deserialized;

	//解凍
	file >> deserialized;

	//正しいレベルデータファイルかチェック
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	//"name"を文字列として取得
	std::string name = deserialized["name"].get<std::string>();
	//正しいレベルデータファイルかチェック
	assert(name.compare("scene") == 0);

	//"objects"の全オブジェクトを走査
	for (nlohmann::json& object : deserialized["objects"]) {
		assert(object.contains("type"));

		if (object.contains("disabled"))
		{
			//有効無効フラグ
			bool disabled = object["disabled"].get<bool>();
			if (disabled) {
				//無効ならスキップ
				continue;
			}
		}

		//種別を取得
		std::string type = object["type"].get<std::string>();

		//MESH
		if (type.compare("MESH") == 0) 
		{
			ObjectDateLoad(object);
		}
		//自キャラ発生ポイント
		else if (type.compare("PlayerSpawn") == 0) 
		{
			PlayerSpawnDataLoad(object);
		}
		//敵発生ポイント
		else if (type.find("Enemy") != std::string::npos && type.find("Spawn") != std::string::npos)
		{
			EnemySpawnDataLoad(object, type);
		}

		//曲線
		else if (type.compare("CURVE") == 0)
		{
			SplineDataLoad(object);
		}

		//TODO: オブジェクト走査を再帰関数にまとめ、再帰関数で枝を走査する

	}

}

void LevelDataLoader::ObjectDateLoad(const nlohmann::json& object)
{
	//要素追加
	ObjectData& objectData = levelData_.objects.emplace_back();

	if (object.contains("file_name"))
	{
		//ファイル名
		objectData.fileName = object["file_name"];
	}
	if (object.contains("file_path"))
	{
		//ファイルパス
		objectData.filePath = object["file_path"];
	}
	if (object.contains("wave_num"))
	{
		//ウェーブ数
		objectData.waveNum = object["wave_num"].get<uint32_t>();
	}
	if (object.contains("speed"))
	{
		//移動速度
		objectData.speed = object["speed"].get<float>();
	}

	objectData.transform = TranformLoad(object["transform"]);

	//TODO: コライダーのパラメータ読み込み
}

void YKEngine::LevelDataLoader::PlayerSpawnDataLoad(const nlohmann::json& playerSpawn)
{
	//要素追加
	PlayerSpawnData& playerSpawnData = levelData_.playerSpawns.emplace_back();

	playerSpawnData.transform = TranformLoad(playerSpawn["transform"]);

	//TODO: コライダーのパラメータ読み込み
}

void YKEngine::LevelDataLoader::EnemySpawnDataLoad(const nlohmann::json& enemySpawn, const std::string& type)
{
	//要素追加
	EnemySpawnData& enemySpawnData = levelData_.enemySpawns.emplace_back();

	enemySpawnData.transform = TranformLoad(enemySpawn["transform"]);
	enemySpawnData.type = type; //EnemySpawnXXのまま格納

	if (enemySpawn.contains("wait_time"))
	{
		//待機時間
		enemySpawnData.waitTime = enemySpawn["wait_time"].get<float>();
	}

	if (enemySpawn.contains("wave_num"))
	{
		//ウェーブ数
		enemySpawnData.waveNum = enemySpawn["wave_num"].get<uint32_t>();
	}

	if (enemySpawn.contains("speed"))
	{
		//移動速度
		enemySpawnData.speed = enemySpawn["speed"].get<float>();
	}

	if (enemySpawn.contains("children"))
	{
		for (const nlohmann::json& child : enemySpawn["children"])
		{
			std::string type = child["type"].get<std::string>();
			if (type.compare("CURVE") == 0)
			{
				enemySpawnData.spline.emplace();
				for (const nlohmann::json& point : child["control_point"])
				{
					Vector3 pointData;
					pointData.x = -static_cast<float>(point[0]);
					pointData.y = static_cast<float>(point[2]);
					pointData.z = -static_cast<float>(point[1]);
					enemySpawnData.spline->controlPoints.push_back(pointData);
				}
				//曲線要素が見つかったら抜ける
				break;
			}
		}
	}
	//TODO: コライダーのパラメータ読み込み
}

void YKEngine::LevelDataLoader::SplineDataLoad(const nlohmann::json& spline)
{
	//要素追加
	SplineData& splineData = levelData_.splines.emplace_back();

	for (const nlohmann::json& point : spline["control_point"])
	{
		Vector3 pointData;
		pointData.x = -static_cast<float>(point[0]);
		pointData.y = static_cast<float>(point[2]);
		pointData.z = -static_cast<float>(point[1]);
		splineData.controlPoints.push_back(pointData);
	}
}

EulerTransform LevelDataLoader::TranformLoad(const nlohmann::json& transformData)
{
	EulerTransform transform;
	//平行移動
	transform.translation.x = -static_cast<float>(transformData["translation"][0]);
	transform.translation.y = static_cast<float>(transformData["translation"][2]);
	transform.translation.z = -static_cast<float>(transformData["translation"][1]);
	//回転角
	transform.rotation.x = static_cast<float>(transformData["rotation"][0]) / 180 * std::numbers::pi_v<float>;
	transform.rotation.y = -static_cast<float>(transformData["rotation"][2]) / 180 * std::numbers::pi_v<float>;
	transform.rotation.z = static_cast<float>(transformData["rotation"][1]) / 180 * std::numbers::pi_v<float>;
	//スケーリング
	transform.scale.x = static_cast<float>(transformData["scaling"][0]);
	transform.scale.y = static_cast<float>(transformData["scaling"][2]);
	transform.scale.z = static_cast<float>(transformData["scaling"][1]);

	return transform;
}
