#pragma once

#include <cstdint>

//コリジョン種別ID定義
enum class CollisionTypeIdDef
{
	kDefault,
	kPlayer,
	kEnemy,
	kPlayerBullet,
	kEnemyBullet,
	kRailMover,
	kWaveEvent,
	kSpeedEvent,
	kRotateEvent,
};