#pragma once
#include <string>

// JSONキー定義
namespace JsonKey
{
	// 弾関連
	namespace Bullet
	{
		const std::string kRotateSpeed = "RotateSpeed";
		const std::string kStopDistance = "StopDistance";
		const std::string kHomingLerpFactor = "HomingLerpFactor";

		namespace Enemy
		{
			const std::string kGroupName = "EnemyBullet";
		}
		namespace Player
		{
			const std::string kGroupName = "PlayerBullet";
		}
	}

	// 敵関連
	namespace Enemy
	{
		const std::string kFireInterval = "FireInterval";
		const std::string kBulletSpeed = "BulletSpeed";

		namespace Shot01
		{
			const std::string kGroupName = "EnemyShot01";
		}
		namespace Shot02
		{
			const std::string kGroupName = "EnemyShot02";
		}
	}
}