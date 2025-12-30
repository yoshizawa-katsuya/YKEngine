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
	kShot01,	//自機狙い弾を撃つ敵
	kShot02,	//追尾弾を撃つ敵
	kTackle01,	//自機に向かって突進してくる敵。追尾はしない
	kTackle02,	//自機を追尾して突進してくる敵
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
	YKEngine::Vector3 position;       // 出現位置
	YKEngine::Vector3 rotation;		// 進行方向の回転角度（例：0, 0, 0）
	std::vector<YKEngine::Vector3> controlPoints;	// スプライン曲線制御点(通過点)
	std::optional<float> speed; // 移動速度（オプション）
};