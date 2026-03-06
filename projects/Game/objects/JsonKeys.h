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

	namespace Player
	{
		const std::string kGroupName = "Player";
		const std::string kHeartPosition = "HeartPosition";
		const std::string kHeartSize = "HeartSize";
		const std::string kHeartSpacing = "HeartSpacing";
	}

	namespace RailCamera
	{
		const std::string kGroupName = "RailCamera";
		const std::string kClearLerpFactor = "ClearLerpFactor";
		const std::string kClearLerpTranslateFactor = "ClearLerpTranslateFactor";
		const std::string kClearLerpRotateFactor = "ClearLerpRotateFactor";
		const std::string kGameOverIncrease = "GameOverIncrease";
		const std::string kStartTIncrease = "StartTIncrease";
		const std::string kStartOfset = "StartOffset";
	}

	namespace RailMover
	{
		const std::string kGroupName = "RailMover";
		const std::string kRotateDuration = "RotateDuration";
		const std::string kLerpAngleFactor = "LerpAngleFactor";
	}

	namespace Pouse
	{
		const std::string kGroupName = "Pouse";

		const std::string kReturnTitlePosition = "ReturnTitlePosition";
		const std::string kContinuePosition = "ContinuePosition";
		const std::string kCursorOffset = "CousorOffset";
	}

	namespace Title
	{
		namespace UI
		{
			const std::string kGroupName = "TitleUI";

			const std::string kLogoPosition = "LogoPosition";
			const std::string kPressAPosition = "PressAPosition";
		}
	}

	namespace StageObjects
	{
		const std::string kGroupName = "StageObjects";
		
		const std::string kNightSkyBoxColor = "NightSkyBoxColor";
		const std::string kDayTimeSkyBoxColor = "DayTimeSkyBoxColor";
		const std::string kGroundEnvironmentCoefficient = "GroundEnvironmentCoefficient";

	}
}