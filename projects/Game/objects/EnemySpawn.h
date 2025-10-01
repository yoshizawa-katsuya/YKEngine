#pragma once
#include <cstdint>
#include "Vector3.h"
#include <vector>
#include <optional>

// 敵の出現データを保持する構造体
struct EnemySpawn
{
	uint32_t waveNumber;        // ウェーブ番号
	Vector3 position;       // 出現位置
	Vector3 rotation;		// 進行方向の回転角度（例：0, 0, 0）
	std::vector<Vector3> controlPoints;	// スプライン曲線制御点(通過点)
	std::optional<float> speed; // 移動速度（オプション）
};