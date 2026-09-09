#include "SwiftEnemy.h"

#include "BasePlayer.h"

void SwiftEnemy::Initialize(const YKEngine::EnemySpawnData& spawnData, const BasePlayer* target1, const BasePlayer* target2)
{
	color_ = kColor_;

	BaseEnemy::Initialize(spawnData, target1, target2);
}

void SwiftEnemy::Move()
{
	maxSpeed_ = 10.0f / 60.0f;

	if (!hasReachedDestination_)
	{
		YKEngine::Vector3 targetPosition = CalculateCenterPosition();

		float distance = YKEngine::Length(targetPosition - worldTransform_.translation_);

		if (distance < 0.1f)
		{
			hasReachedDestination_ = true;
			velocity_ = YKEngine::Normalize(velocity_) * maxSpeed_;
			return;
		}

		YKEngine::Vector3 newVelocity = RotateTowards(targetPosition);

		velocity_ = YKEngine::Normalize(newVelocity) * maxSpeed_;
	}
}

YKEngine::Vector3 SwiftEnemy::CalculateCenterPosition()
{
	// ターゲットが両方とも存在する場合は、ターゲット同士の中心位置を計算
	if(target1_ && target2_)
	{
		YKEngine::Vector3 pos1 = target1_->GetPosition();
		YKEngine::Vector3 pos2 = target2_->GetPosition();

		return (pos1 + pos2) * 0.5f;
	}

	// ターゲットが片方しか存在しない場合は、進行方向を返す
	return worldTransform_.translation_ + velocity_;
}

YKEngine::Vector3 SwiftEnemy::RotateTowards(const YKEngine::Vector3& targetPosition, float deltaTime)
{
	YKEngine::Vector3 centerPosition = CalculateCenterPosition();

	// ターゲットの中心位置と現在の位置からターゲット方向を計算
	YKEngine::Vector3 targetDirection = YKEngine::Normalize(centerPosition - worldTransform_.translation_);

	// 速度を正規化して現在の方向を取得
	YKEngine::Vector3 currentDirection = YKEngine::Normalize(velocity_);

	// 2つの方向ベクトルの内積を計算
	float dot = YKEngine::Dot(currentDirection, targetDirection);

	// 内積から角度を計算（ラジアン）
	float angle = acosf(dot);

	// 角度が小さい場合は回転せずに現在の方向を返す
	if (angle > 0.0001f)
	{
		return targetDirection;
	}

	// 1フレームで到達できるなら、そのままターゲット方向へ
	if (angle <= kMaxHomingAngle_)
	{
		return targetDirection;
	}

	// 1フレームで到達できない場合は、最大ホーミング角度分だけ回転する
	YKEngine::Vector3 axis = YKEngine::Cross(currentDirection, targetDirection);

	float cosAngle = cosf(kMaxHomingAngle_);
	float sinAngle = sinf(kMaxHomingAngle_);

	return currentDirection * cosAngle + YKEngine::Cross(axis, currentDirection) * sinAngle;
}