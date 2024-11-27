#include "BossCanon.h"
#include "Boss.h"
#include "Player.h"
#include "Rigid3dObject.h"
#include "Matrix.h"
#include "Vector.h"
#include "Slerp.h"

void BossCanon::Initialize(Base3dObject* object, Boss* boss, const Vector3& velocity)
{
	// 引数から変数に記録
	velocity_ = velocity;
	pBoss_ = boss;
	// オブジェクト生成
	object_ = object;
	// 初期化
	worldTransform_.Initialize();
	// ボスの中心座標を取得
	worldTransform_.translation_ = pBoss_->GetWorldPosition();

	// y軸周りの回転角度（0y）を計算
	worldTransform_.rotation_.y = std::atan2(velocity_.x, velocity_.z);

	Vector3 _velocity = velocity_;
	_velocity.y = 0;
	float velocityXZ = Length(_velocity);

	// x軸周りの回転角度（0x）を計算
	worldTransform_.rotation_.x = std::atan2(-velocity_.y, velocityXZ);

}

void BossCanon::Update(Camera* camera)
{
	// ホーミング処理

	// ボスの弾から自キャラへのベクトルを計算
	Vector3 toPlayer = pPlayer_->GetWorldPosition() - GetWorldPosition();

	// 正規化して方向ベクトルに変換
	toPlayer = Normalize(toPlayer);

	// ホーミング強度
	float homingStrength = 0.05f;
	// 弾の速さ
	float speed = 0.9f;

	// 現在の速度にプレイヤー方向を加味して加算
	velocity_ = Slerp(velocity_, toPlayer, homingStrength) * speed;

	// 弾が進行方向に向くように回転角度を更新

	// y軸周りの回転
	worldTransform_.rotation_.y = std::atan2(velocity_.x, velocity_.z);

	// x軸周りの回転
	Vector3 velocityXZ = velocity_;
	velocityXZ.y = 0;
	float lengthXZ = Length(velocityXZ);
	worldTransform_.rotation_.x = std::atan2(-velocity_.y, lengthXZ);

	// 高さの調整（無理やり）
	

	// 座標を移動させる
	worldTransform_.translation_ += velocity_;
	worldTransform_.UpdateMatrix();
	// オブジェクト更新
	object_->Update(worldTransform_, camera);
}

void BossCanon::Draw()
{
	// オブジェクト描画処理
	object_->Draw();

}

void BossCanon::OnCollision()
{
	isDead_ = true;
}

Vector3 BossCanon::GetWorldPosition()
{
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform_.worldMatrix_.m[3][0];
	worldPos.y = worldTransform_.worldMatrix_.m[3][1];
	worldPos.z = worldTransform_.worldMatrix_.m[3][2];

	return worldPos;
}
