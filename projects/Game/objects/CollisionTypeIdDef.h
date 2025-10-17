#pragma once

#include <cstdint>

/// <summary>
/// コライダーの種類ID。
/// </summary>
enum class CollisionTypeIdDef
{
	kDefault,
	kPlayer,
	kEnemy,
	kTackleEnemy,
	kPlayerBullet,
	kEnemyBullet,
	kRailMover,
	kEvent,
};