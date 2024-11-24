#include "BossBullet.h"


void BossBullet::Initialize(BaseModel* model, const Vector3& velocity)
{

	model_ = model;

	worldTransform_.Initialize();
	//worldTransform_.translation_ = { 100.0f,0.0f,0.0f };
	worldTransform_.rotation_ = { 0.0f,-1.5f,0.0f };
	worldTransform_.scale_ = { 0.1f,0.1f,0.1f };
	velocity_ = velocity;

}

void BossBullet::Update()
{

	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);

	worldTransform_.UpdateMatrix();


}

void BossBullet::Draw(Camera* camera)
{

	model_->Draw(worldTransform_, camera);




}

Vector3 BossBullet::Add(const Vector3& v1, const Vector3& v2)
{
	Vector3 result;

	result.x = v1.x + v2.x;
	result.y = v1.y + v2.y;
	result.z = v1.z + v2.z;

	return result;
}

void BossBullet::SetPosition(const Vector3& position)
{

	worldTransform_.translation_ = position;
}
