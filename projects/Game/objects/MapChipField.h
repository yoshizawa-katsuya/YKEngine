#pragma once
#include <stdint.h>
#include <string>
#include <vector>
#include <Vector3.h>
#include "MapChipType.h"

enum class MapChipType {
	kBlank, // 空白
	kBlock, // ブロック
	// 仮

	kPlayerSpawn, // プレイヤースポーン
	kDoorTrigger, // ドアトリガー
	kDoor,       // ドア

	kTransparentBlock, // 透明ブロック
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

	uint32_t GetNumBlocks() { return numBlocks_; };

	uint32_t GetNumSpines() { return numSpines_; };

	// ドア用
	void SetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex, MapChipType mapChipType);

	Vector2 GetCellSize() { return { kCellWidth_, kCellHeight_ }; }

private:
	
	//マップチップタイプごとの個数を数える
	void CountMapChipTypeNum(MapChipType mapChipType);

	// 1ブロックのサイズ
	const float kCellWidth_ = 2.0f;
	const float kCellHeight_ = 2.0f;

	// ブロックの個数
	uint32_t kNumCellVirtical_ = 0;
	uint32_t kNumCellHorizontal_ = 0;

	MapChipData mapChipData_;

	uint32_t numBlocks_ = 0;
	uint32_t numSpines_ = 0;
};

