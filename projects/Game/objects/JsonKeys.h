#pragma once
#include <string>

// JSONキー定義
namespace JsonKey
{
	// 弾関連
	namespace Bullet
	{
		const std::string kGroupName = "Bullet";
		const std::string kRotateSpeed = "RotateSpeed";
		const std::string kStopDistance = "StopDistance";
		const std::string kHomingLerpFactor = "HomingLerpFactor";
		const std::string kRadius = "Radius";	//半径
		const std::string kAttackPower = "AttackPower";	//攻撃力

		namespace Enemy
		{
			const std::string kGroupName = "EnemyBullet";
		}
		namespace Player
		{
			const std::string kGroupName = "PlayerBullet";
			namespace Charge01
			{
				const std::string kGroupName = "PlayerChargeBullet01";
			}
		}
	}

	// 敵関連
	namespace Enemy
	{
		const std::string kGroupName = "Enemy";
		const std::string kBrowAwaySpeed = "BrowAwaySpeed";	//吹き飛びの速さ
		const std::string kDirectionWeightBullet = "DirectionWeightBullet";	//吹き飛ぶ方向を決めるときの弾の移動方向の割合
		const std::string kDeadRotateSpeedMin = "DeadRotateSpeedMin";	//死亡時の回転の速さの最小値
		const std::string kDeadRotateSpeedMax = "DeadRotateSpeedMax";	//死亡時の回転の速さの最大値
		const std::string kLeaveTime = "LeaveTime";	//離脱までの時間
		const std::string kDeadTime = "DeadTime";	//完全に消滅するまでの時間
		const std::string kDamageReactionTime = "DamageReactionTime";	//ダメージリアクションの時間
		const std::string kDamageReactionMoveRange = "DamageReactionMoveRange";	//ダメージリアクションのランダム移動の範囲
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
		const std::string kRotateLerpFactor = "RotateLerpFactor";	//回転の補完係数
		const std::string kDodgeSpeed = "DodgeSpeed";
		//回避の所要時間（フレーム数）
		const std::string kDodgeTime = "DodgeTime";
		const std::string kSpeed = "Speed";
		//傾きの大きさ
		const std::string kTiltQuantity = "TiltQuantity";
		const std::string kTiltLerpFactor = "TiltLerpFactor";	//傾きの補完係数
		//開始アニメーションの時間（秒）
		const std::string kStartAnimeDuration = "StartAnimeDuration";
		//開始アニメーションの回転の大きさ
		const std::string kStartRotateAnimeQuantity = "StartRotateAnimeQuantity";
		const std::string kMoveLimitX = "MoveLimitX";	//移動限界座標X
		const std::string kMoveLimitY = "MoveLimitY";	//移動限界座標Y
		const std::string kGameOverRandomMoveRange = "GameOverRandomMoveRange";	//ゲームオーバー時のランダム移動の範囲
		const std::string kGameOverDuration = "GameOverDuration";	//ゲームオーバー演出の時間（秒）
		const std::string kDamageReactionDuration = "DamageReactionDuration";	//ダメージリアクションの時間（秒）
		const std::string kDamageReactionMoveRange = "DamageReactionMoveRange";	//ダメージリアクションのランダム移動の範囲
		const std::string kDodgeRotateLerpFactor = "DodgeRotateLerpFactor";	//回避の回転の補完係数
		const std::string kGameClearRotateLerpFactor = "GameClearRotateLerpFactor";	//ゲームクリアの回転の補完係数

		const std::string kNormalBulletShotInterval = "NormalBulletShotInterval";	//通常弾の発射間隔
		const std::string kChargeBulletShotInterval = "ChargeBulletShotInterval";	//チャージ弾の発射間隔
	}

	namespace DemoPlayer
	{
		const std::string kGroupName = "DemoPlayer";
		const std::string kInitialPosition = "InitialPosition";	//初期位置
		const std::string kStartAnimeDuration = "StartAnimeDuration";	//開始アニメーションの時間（秒）
		const std::string kEndAnimeDuration = "EndAnimeDuration";	//終了アニメーションの時間（秒）
		const std::string kEndAnimeTranslate = "EndAnimeTranslate";	//終了アニメーションの移動量	
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