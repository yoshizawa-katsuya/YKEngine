#pragma once
#include <string>
#include <vector>
#include "Struct.h"

enum class MapChipType {
	kFloor,//床
	disappearBox1,
	disappearBox2,
	appearBox1,
	appearBox2,
	kBox, // ボックス
	kBlank,
	stone,
	star,
	hole,
	ice,
};

class MapChipField
{
public:

	struct IndexSet {
		uint32_t xIndex;
		uint32_t zIndex;
	};

	//範囲矩形
	struct Rect {
		float left;
		float right;
		float front;
		float back;
	};

	void ResetMapChipDate();


	void LoadMapChipCsv(const std::string& filePath);

	MapChipType GetMapChipTypeByIndex(uint32_t xIndex, uint32_t zIndex);

	Vector3 GetMapChipPositionByIndex(uint32_t xIndex, uint32_t zIndex);

	Rect GetRectByIndex(uint32_t xIndex, uint32_t zIndex);

	IndexSet GetMapChipIndexSetByPosition(const Vector3& position);

	uint32_t GetNumBlockVertical() { return kNumBlockVertical; };

	uint32_t GetNumBlockHorizontal() { return kNumBlockHorizontal; }

private:

	struct MapChipData {
		std::vector<std::vector<MapChipType>> data;
	};

	// 1ブロックのサイズ
	const float kBlockWidth = 1.0f;
	const float kBlockDepth = 1.0f;

	// ブロックの個数
	const uint32_t kNumBlockVertical = 15;
	const uint32_t kNumBlockHorizontal = 24;

	MapChipData mapChipData_;

};
