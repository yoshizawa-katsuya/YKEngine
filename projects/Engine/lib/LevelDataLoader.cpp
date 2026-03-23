#include "LevelDataLoader.h"
#include "fstream"
#include "cassert"
#include <numbers>

namespace YKEngine
{

LevelData LevelDataLoad(const std::string& kDefaultBaseDirectory, const std::string& fileName, const std::string& kExtension)
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
	LevelData levelData;

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
			ObjectData& objectData = levelData.objects.emplace_back();

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

			objectData.transform = TranformLoad(transform);

			//TODO: コライダーのパラメータ読み込み
		}
		//自キャラ発生ポイント
		else if (type.compare("PlayerSpawn") == 0) {
			//要素追加
			PlayerSpawnData& playerSpawnData = levelData.playerSpawns.emplace_back();

			//トランスフォームのパラメータ読み込み
			nlohmann::json& transform = object["transform"];

			playerSpawnData.transform = TranformLoad(transform);

			//TODO: コライダーのパラメータ読み込み
		}
		//敵発生ポイント
		else if (type.compare("EnemySpawn") == 0) {
			//要素追加
			EnemySpawnData& enemySpawnData = levelData.enemySpawns.emplace_back();
			//トランスフォームのパラメータ読み込み
			nlohmann::json& transform = object["transform"];

			enemySpawnData.transform = TranformLoad(transform);

			if (object.contains("wait_time"))
			{
				//待機時間
				enemySpawnData.waitTime = object["wait_time"].get<float>();
			}

			if (object.contains("wave_num"))
			{
				//ウェーブ数
				enemySpawnData.waveNum = object["wave_num"].get<uint32_t>();
			}
			//TODO: コライダーのパラメータ読み込み
		}

		//曲線
		else if (type.compare("CURVE") == 0)
		{
			//要素追加
			SplineData& splineData = levelData.splines.emplace_back();
			
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

EulerTransform TranformLoad(const nlohmann::json& transformData)
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

} // namespace YKEngine