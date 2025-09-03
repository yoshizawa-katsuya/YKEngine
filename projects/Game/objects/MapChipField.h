#pragma once
#include <stdint.h>
#include <string>
#include <vector>
#include <Vector3.h>

enum class MapChipType {
	kBlank, // 空白
	kBlock, // ブロック
};

struct MapChipData {
	std::vector<std::vector<MapChipType>> data;
};


/// <summary>
///マップチップフィールド
/// <summary>
class MapChipField {	

public:
	
	struct IndexSet {
		uint32_t xIndex;
		uint32_t yIndex;
	};

	//範囲矩形
	struct Rect {
		float left;
		float right;
		float bottom;
		float top;
	};

	void ResetMapChipDate();

	void LoadMapChipCsv(const std::string& filePath);

	MapChipType GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex);

	Vector3 GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex);

	Rect GetRectByIndex(uint32_t xIndex, uint32_t yIndex);

	IndexSet GetMapChipIndexSetByPosition(const Vector3& position);

	uint32_t GetNumCellVirtical();

	uint32_t GetNumCellHorizontal();

private:
	
	// 1ブロックのサイズ
	const float kCellWidth_ = 2.0f;
	const float kCellHeight_ = 2.0f;

	// ブロックの個数
	uint32_t kNumCellVirtical_ = 0;
	uint32_t kNumCellHorizontal_ = 0;

	MapChipData mapChipData_;

};

