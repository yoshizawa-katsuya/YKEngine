#pragma once
#pragma once
#include "BaseModel.h"
#include "SkinModel.h"
#include "WorldTransform.h"
#include "Animation.h"
#include "BossBullet.h"
#include "Input.h"
#include "RigidModel.h"
class Camera;

class Boss
{

public:

	void Initialize(const std::vector<BaseModel*>& models);

	void Update();

	void Draw(Camera* camera);


	void GunAttack();

	Vector3 TransFormNormal(const Vector3& v, const Matrix4x4& m);

	Vector3 Add(const Vector3& v1, const Vector3& v2);


private:

	//Transform変数を作る
	std::vector<WorldTransform> worldTransforms_;

	std::vector<BaseModel*> models_;


	std::vector<std::unique_ptr<BossBullet>> bullets_;
	std::unique_ptr<BaseModel> bulletModel_ = nullptr;
	

	int bulletCount_ = 0;
	int cooldownTimer_ = 0;
	const int maxBulletCount_ = 10;
	const int cooldownDuration_ = 60;


	Input* input_;

};


