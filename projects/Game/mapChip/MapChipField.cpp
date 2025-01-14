#include "MapChipField.h"
#include <map>
#include <fstream>
#include <cassert>
#include <sstream>

namespace {

    std::map<std::string, MapChipType> mapChipTable = {
		{"0", MapChipType::kFloor},
        {"1", MapChipType::kBox},
		{"2", MapChipType::kBlank},
    };

}

//マップチップデータをリセット
void MapChipField::ResetMapChipDate() {

	mapChipData_.data.clear();
	mapChipData_.data.resize(kNumBlockVertical);
	for (std::vector<MapChipType>& mapChipDataLine : mapChipData_.data) {
		mapChipDataLine.resize(kNumBlockHorizontal);
	}

}

void MapChipField::LoadMapChipCsv(const std::string& filePath) {

	ResetMapChipDate();

	//ファイルを開く
	std::ifstream file;
	file.open(filePath);
	assert(file.is_open());

	// マップチップCSV
	std::stringstream mapChipCsv;
	//ファイルの内容を文字列ストリームにコピー
	mapChipCsv << file.rdbuf();
	//ファイルを閉じる
	file.close();

	std::string line;

	for (uint32_t i = 0; i < kNumBlockVertical; ++i) {

		getline(mapChipCsv, line);

		//1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {

			std::string word;
			getline(line_stream, word, ',');

			if (mapChipTable.contains(word)) {
				mapChipData_.data[i][j] = mapChipTable[word];
			}

		}

	}

}

MapChipType MapChipField::GetMapChipTypeByIndex(uint32_t xIndex, uint32_t zIndex) {
	if (xIndex < 0 || kNumBlockHorizontal - 1 < xIndex) {
		return MapChipType::kBlank;
	}
	if (zIndex < 0 || kNumBlockVertical - 1 < zIndex) {
		return MapChipType::kBlank;
	}

	return mapChipData_.data[zIndex][xIndex];
}

Vector3 MapChipField::GetMapChipPositionByIndex(uint32_t xIndex, uint32_t zIndex) {
	//return Vector3(kBlockWidth * xIndex, 0, kBlockDepth * (kNumBlockVertical - 1 - zIndex));
	//マップの中心座標が原点になるように変更
	return Vector3(kBlockWidth * (xIndex - (kNumBlockHorizontal - 1) / 2.0f), 0, kBlockDepth * ((kNumBlockVertical - 1) / 2.0f - zIndex));

}

MapChipField::Rect MapChipField::GetRectByIndex(uint32_t xIndex, uint32_t zIndex) {

	//指定ブロックの中心座標を取得する
	Vector3 center = GetMapChipPositionByIndex(xIndex, zIndex);

	Rect rect;
	rect.left = center.x - kBlockWidth / 2.0f;
	rect.right = center.x + kBlockWidth / 2.0f;
	rect.front = center.z - kBlockDepth / 2.0f;
	rect.back = center.z + kBlockDepth / 2.0f;

	return rect;

}

MapChipField::IndexSet MapChipField::GetMapChipIndexSetByPosition(const Vector3& position) {

	IndexSet indexSet = {};
	indexSet.xIndex = uint32_t((position.x + kBlockWidth / 2) / kBlockWidth);
	//indexSet.yIndex = uint32_t(kNumBlockVirtical - 1 - (position.y + kBlockHeight / 2) / kBlockHeight);
	//indexSet.yIndex = uint32_t(kNumBlockVirtical - (position.y + kBlockHeight / 2) / kBlockHeight);
	indexSet.zIndex = uint32_t((position.z + kBlockDepth / 2) / kBlockDepth);
	indexSet.zIndex = kNumBlockVertical - 1 - indexSet.zIndex;
	return indexSet;
}