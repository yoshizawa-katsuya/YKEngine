#include "LevelDataLoader.h"
#include "fstream"
#include "cassert"
#include "json.hpp"
#include <numbers>

LevelData* LevelDataLoad(const std::string& kDefaultBaseDirectory, const std::string& fileName, const std::string& kExtension)
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

	//レベルデータ格納用インスタンスを生成
	LevelData* levelData = new LevelData();

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
		if (type.compare("MESH") == 0) {
			//要素追加
			ObjectData& objectData = levelData->objects.emplace_back();

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

			//トランスフォームのパラメータ読み込み
			nlohmann::json& transform = object["transform"];
			//平行移動
			objectData.transform.translation.x = -static_cast<float>(transform["translation"][0]);
			objectData.transform.translation.y = static_cast<float>(transform["translation"][2]);
			objectData.transform.translation.z = -static_cast<float>(transform["translation"][1]);
			//回転角
			objectData.transform.rotation.x = -static_cast<float>(transform["rotation"][0]) / 180 * std::numbers::pi_v<float>;
			objectData.transform.rotation.y = -static_cast<float>(transform["rotation"][2]) / 180 * std::numbers::pi_v<float>;
			objectData.transform.rotation.z = -static_cast<float>(transform["rotation"][1]) / 180 * std::numbers::pi_v<float>;
			//スケーリング
			objectData.transform.scale.x = static_cast<float>(transform["scaling"][0]);
			objectData.transform.scale.y = static_cast<float>(transform["scaling"][2]);
			objectData.transform.scale.z = static_cast<float>(transform["scaling"][1]);


			//TODO: コライダーのパラメータ読み込み
		}
		//自キャラ発生ポイント
		else if (type.compare("PlayerSpawn") == 0) {
			//要素追加
			PlayerSpawnData& playerSpawnData = levelData->playerSpawns.emplace_back();

			//トランスフォームのパラメータ読み込み
			nlohmann::json& transform = object["transform"];
			//平行移動
			playerSpawnData.transform.translation.x = -static_cast<float>(transform["translation"][0]);
			playerSpawnData.transform.translation.y = static_cast<float>(transform["translation"][2]);
			playerSpawnData.transform.translation.z = -static_cast<float>(transform["translation"][1]);
			//回転角
			playerSpawnData.transform.rotation.x = -static_cast<float>(transform["rotation"][0]) / 180 * std::numbers::pi_v<float>;
			playerSpawnData.transform.rotation.y = -static_cast<float>(transform["rotation"][2]) / 180 * std::numbers::pi_v<float>;
			playerSpawnData.transform.rotation.z = -static_cast<float>(transform["rotation"][1]) / 180 * std::numbers::pi_v<float>;
			//スケーリング
			playerSpawnData.transform.scale.x = static_cast<float>(transform["scaling"][0]);
			playerSpawnData.transform.scale.y = static_cast<float>(transform["scaling"][2]);
			playerSpawnData.transform.scale.z = static_cast<float>(transform["scaling"][1]);


			//TODO: コライダーのパラメータ読み込み
		}
		//曲線
		else if (type.compare("CURVE") == 0)
		{
			//要素追加
			SplineData& splineData = levelData->splines.emplace_back();
			
			for (nlohmann::json& point : object["control_point"])
			{
				Vector3 pointData;
				pointData.x = -static_cast<float>(point[0]);
				pointData.y = static_cast<float>(point[2]);
				pointData.z = -static_cast<float>(point[1]);
				splineData.controlPoints.push_back(pointData);
			}
		}


		//TODO: オブジェクト走査を再帰関数にまとめ、再帰関数で枝を走査する
		if (object.contains("children")) {

		}

	}

	return levelData;

}
