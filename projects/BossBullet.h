#pragma once
#include "BaseModel.h"
#include "SkinModel.h"
#include "WorldTransform.h"
#include "Animation.h"

class Boss;

class BossBullet
{
public:
	void Initialize(BaseModel* model, const Vector3& velocity);

	void Update();

	void Draw(Camera* camera);

	Vector3 Add(const Vector3& v1, const Vector3& v2);

	void SetPosition(const Vector3& position);
private:

	// 各部位のTransform変数を格納する
	WorldTransform worldTransform_;

	BaseModel* model_ = nullptr;


	Vector3 velocity_;


};

