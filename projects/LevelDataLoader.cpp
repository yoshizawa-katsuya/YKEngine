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

		//種別を取得
		std::string type = object["type"].get<std::string>();

		//MESH
		if (type.compare("MESH") == 0) {
			//要素追加
			levelData->objects.emplace_back(ObjectData{});
			//今追加した要素の参照を得る
			ObjectData& objectData = levelData->objects.back();

			if (object.contains("file_name")) {
				//ファイル名
				objectData.fileName = object["file_name"];
			}

			//トランスフォームのパラメータ読み込み
			nlohmann::json& transform = object["transform"];
			//平行移動
			objectData.transform.translate.x = -static_cast<float>(transform["translation"][0]);
			objectData.transform.translate.y = static_cast<float>(transform["translation"][2]);
			objectData.transform.translate.z = -static_cast<float>(transform["translation"][1]);
			//回転角
			objectData.transform.rotate.x = -static_cast<float>(transform["rotation"][0]) / 180 * std::numbers::pi_v<float>;
			objectData.transform.rotate.y = -static_cast<float>(transform["rotation"][2]) / 180 * std::numbers::pi_v<float>;
			objectData.transform.rotate.z = -static_cast<float>(transform["rotation"][1]) / 180 * std::numbers::pi_v<float>;
			//スケーリング
			objectData.transform.scale.x = static_cast<float>(transform["scaling"][0]);
			objectData.transform.scale.y = static_cast<float>(transform["scaling"][2]);
			objectData.transform.scale.z = static_cast<float>(transform["scaling"][1]);


			//TODO: コライダーのパラメータ読み込み
		}


		//TODO: オブジェクト走査を再帰関数にまとめ、再帰関数で枝を走査する
		if (object.contains("children")) {

		}

	}

	return levelData;

}
