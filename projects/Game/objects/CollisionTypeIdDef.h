#pragma once

#include <cstdint>

//コリジョン種別ID定義
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