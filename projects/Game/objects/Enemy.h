#pragma once
#include "Base3dObject.h"
#include "Collider.h"
#include "WorldTransform.h"

class Enemy : public Collider
{
public:
	Enemy();
	~Enemy() override;

	void Initialize(BaseModel* model, const Vector3& translate = {0.0f, 0.5f, 0.0f});

	void Update();

	void Draw(Camera* camera);

private:
	//Transform変数を作る
	WorldTransform worldTransform_;

	std::unique_ptr<Base3dObject> object_;
};

