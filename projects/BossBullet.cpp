#include "BossBullet.h"
#include "Camera.h"

void BossBullet::Initialize(Base3dObject* object, const Vector3& translation, const Vector3& velocity)
{

	object_ = object;
	velocity_ = velocity;

	worldTransform_.Initialize();
	worldTransform_.translation_ = translation;
}

void BossBullet::Update(Camera* camera)
{
	// 時間経過でデス
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

	worldTransform_.translation_ += velocity_;

	// ワールドトランスフォーム更新
	worldTransform_.UpdateMatrix();

	object_->Update(worldTransform_,camera);
}

void BossBullet::Draw()
{
	object_->Draw();
}

void BossBullet::OnCollision()
{
	isDead_ = true;
}

Vector3 BossBullet::GetWorldPosition()
{
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform_.worldMatrix_.m[3][0];
	worldPos.y = worldTransform_.worldMatrix_.m[3][1];
	worldPos.z = worldTransform_.worldMatrix_.m[3][2];

	return worldPos;
}
