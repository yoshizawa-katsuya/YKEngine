#include "Enemy.h"
#include "cassert"
#include "Vector3.h"
#include "Matrix.h"
#include "Player.h"
#include "GameScene.h"
#include "WinApp.h"

Enemy::~Enemy() {
	/*
	for (EnemyBullet* bullet : enemyBullets_) {
		delete bullet;
	}
	*/
}

void Enemy::Initialize(BaseModel* model, const Vector3& position) {

	BaseCharacter::Initialize(model);
	
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float>;

	ApproachInitialize();
}

void Enemy::ApproachInitialize() {
	//発射タイマーを初期化
	fireTimer = kFireInterval;
}

void Enemy::Update() {

	// デスフラグの立った弾を削除
	/*
	enemyBullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});
	*/
	switch (phase_) {
	case Phase::Approach:
	default:
		ApproachUpdate();
		break;
	case Phase::Leave:
		LeaveUpdate();
		break;
	}

	// 弾更新
	/*
	for (EnemyBullet* bullet : enemyBullets_) {
		bullet->Update();
	}
	*/
	BaseCharacter::Update();
}

void Enemy::ApproachUpdate() {

	//発射タイマーカウントダウン
	fireTimer--;
	//指定時間に達した
	if (fireTimer == 0) {
		//弾を発射
		Fire();
		//発射タイマーを初期化
		fireTimer = kFireInterval;
	}

	// 移動
	worldTransform_.translation_ += ApproachVelocity_;
	// 規定の位置に到達したら離脱
	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}
}

void Enemy::LeaveUpdate() {
	// 移動
	worldTransform_.translation_ += LeaveVelocity_;
	
}

void Enemy::Fire() {

	//弾の速さ
	const float kBulletSpeed = 0.5f;

	Vector3 toPosition = player_->GetWorldPosition();
	Vector3 fromPosition = GetWorldPosition();
	Vector3 velocity = Subtract(toPosition, fromPosition);
	velocity = Normalize(velocity);
	velocity = Multiply(kBulletSpeed, velocity);

	// 弾を生成し、初期化
	gameScene_->AddEnemybullet(GetWorldPosition(), velocity);
	//enemyBullets_.push_back(newBullet);

}

void Enemy::OnCollision() {

	isDead_ = true;

}

Vector3 Enemy::GetWorldPosition() {

	return worldTransform_.GetWorldPosition();

}

Vector3 Enemy::GetScreenPosition(Camera* camera) {
	
	
	// ビューポート行列
	Matrix4x4 matViewport = MakeViewportMatrix(0, 0, WinApp::kClientWidth, WinApp::kClientHeight, 0, 1);

	// ビュー行列とプロジェクション行列、ビューポート行列を合成する
	Matrix4x4 matViewProjectionViewport = Multiply(camera->GetViewProjection(), matViewport);

	// ワールド→スクリーン座標変換(ここで3Dから2Dになる)
	Vector3 screenPosition = Transform(GetWorldPosition(), matViewProjectionViewport);

	return screenPosition;
}
