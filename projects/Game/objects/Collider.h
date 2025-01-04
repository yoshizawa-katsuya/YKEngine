#pragma once
#include "WorldTransform.h"
#include "Struct.h"
#include "Base3dObject.h"

class Collider
{
public:

	enum  class LRDirection {
		kRight,
		kLeft,
	};

	virtual ~Collider() = default;

	virtual void Initialize(BaseModel* model);

	virtual void Update();

	virtual void Draw(Camera* camera);

	virtual void OnCollision() {}

	//中心座標を取得
	virtual Vector3 GetCenterPosition() const;
	virtual Vector2 Get2DCenterPosition() const;

	//半径を取得
	float GetRadius() { return radius_; }

	void SetRadius(float radius) { radius_ = radius; }

protected:

	WorldTransform worldTransform_;

	std::unique_ptr<Base3dObject> object_;

	//衝突半径
	float radius_ = 0.5f;
};

