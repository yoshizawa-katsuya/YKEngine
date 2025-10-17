#pragma once
#include <cstdint>
#include "Vector3.h"
#include <vector>
#include <optional>

/// <summary>
/// 敵の種類を表す列挙型。
/// </summary>
enum class EnemyType
{
	Shot01,
	Tackle01,
};

/// <summary>
/// 敵の出現情報を表す構造体。
/// </summary>
/// <param name="type">敵の種類。</param>
/// <param name="waveNumber">ウェーブ番号。</param>
/// <param name="position">出現位置。</param>
/// <param name="rotation">進行方向の回転角度。</param>
/// <param name="controlPoints">スプライン曲線制御点(通過点)。</param>
/// <param name="speed">移動速度（オプション）。</param>
struct EnemySpawn
{
	EnemyType type;     // 敵の種類
	uint32_t waveNumber;        // ウェーブ番号
	Vector3 position;       // 出現位置
	Vector3 rotation;		// 進行方向の回転角度（例：0, 0, 0）
	std::vector<Vector3> controlPoints;	// スプライン曲線制御点(通過点)
	std::optional<float> speed; // 移動速度（オプション）
};