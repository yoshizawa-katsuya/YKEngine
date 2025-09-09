#include "MapChipField.h"
#include <map>
#include <fstream>
#include <sstream>
#include <assert.h>

namespace {

std::map<std::string, MapChipType> mapChipTable = {
    {"0", MapChipType::kBlank},
    {"1", MapChipType::kBlock},
	{"3", MapChipType::kDoorTrigger},
	{"4", MapChipType::kDoor},
	{"12", MapChipType::kSpine},
	{"18", MapChipType::kGoal},
};

}

//マップチップデータをリセット
void MapChipField::ResetMapChipDate() {

    mapChipData_.data.clear();
	mapChipData_.data.resize(kNumCellVirtical_);
	for (std::vector<MapChipType>& mapChipDataLine : mapChipData_.data) {
		mapChipDataLine.resize(kNumCellHorizontal_);
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

	while (getline(mapChipCsv, line)) {
		
		//1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);
		std::string word;

		std::vector<MapChipType> mapChipDataLine;

		while (getline(line_stream, word, ',')) {

			if (mapChipTable.contains(word)) 
			{
				mapChipDataLine.push_back(mapChipTable[word]);
				CountMapChipTypeNum(mapChipTable[word]);
			}

		}

		if (kNumCellVirtical_ == 0)
		{
			kNumCellHorizontal_ = static_cast<uint32_t>(mapChipDataLine.size());
		}
		mapChipData_.data.push_back(mapChipDataLine);
		kNumCellVirtical_++;
	}

}

MapChipType MapChipField::GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex) {
	if (xIndex < 0 || kNumCellHorizontal_ - 1 < xIndex) {
		return MapChipType::kBlank;
	}
	if (yIndex < 0 || kNumCellVirtical_ - 1 < yIndex) {
		return MapChipType::kBlank;
	}

	return mapChipData_.data[yIndex][xIndex];
}

Vector3 MapChipField::GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex) {
	return Vector3(kCellWidth_ * xIndex, kCellHeight_ * (kNumCellVirtical_ - 1 - yIndex), 0);
}

MapChipField::Rect MapChipField::GetRectByIndex(uint32_t xIndex, uint32_t yIndex) {

	//指定ブロックの中心座標を取得する
	Vector3 center = GetMapChipPositionByIndex(xIndex, yIndex);

	Rect rect;
	rect.left = center.x - kCellWidth_ / 2.0f;
	rect.right = center.x + kCellWidth_ / 2.0f;
	rect.bottom = center.y - kCellHeight_ / 2.0f;
	rect.top = center.y + kCellHeight_ / 2.0f;

	return rect;

}

MapChipField::IndexSet MapChipField::GetMapChipIndexSetByPosition(const Vector3& position) {

	IndexSet indexSet = {};
	indexSet.xIndex = uint32_t((position.x + kCellWidth_ / 2) / kCellWidth_);
	//indexSet.yIndex = uint32_t(kNumCellVirtical_ - 1 - (position.y + kCellHeight_ / 2) / kCellHeight_);
	//indexSet.yIndex = uint32_t(kNumCellVirtical_ - (position.y + kCellHeight_ / 2) / kCellHeight_);
	indexSet.yIndex = uint32_t((position.y + kCellHeight_ / 2) / kCellHeight_);
	indexSet.yIndex = kNumCellVirtical_ - 1 - indexSet.yIndex;
	return indexSet;
}

uint32_t MapChipField::GetNumCellVirtical() { return kNumCellVirtical_; }

uint32_t MapChipField::GetNumCellHorizontal() { return kNumCellHorizontal_; }

void MapChipField::CountMapChipTypeNum(MapChipType mapChipType)
{
	switch (mapChipType)
	{
	case MapChipType::kBlock:
		++numBlocks_;
		break;
	case MapChipType::kSpine:
		++numSpines_;
		break;
	default:
		break;
	}
}
uint32_t MapChipField::GetNumCellHorizontal() { return kNumCellHorizontal_; }

void MapChipField::SetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex, MapChipType mapChipType)
{
	mapChipData_.data[yIndex][xIndex] = mapChipType;
}
