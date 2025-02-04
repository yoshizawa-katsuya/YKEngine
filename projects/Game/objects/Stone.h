#pragma once
#include "Rigid3dObject.h"
#include "Input.h"

class Stone
{
public:

	Stone();

	void Initialize(BaseModel* model, const Vector3& position);

	void Update();

	void Draw(Camera* camera);

	Vector3& GetPosition() { return worldTransform_.translation_; }

	Vector2& GetDragStartPos() { return dragStartPos_; }

	Vector2& GetDragCurrentPos() { return dragCurrentPos_; }

private:

	Vector3 ConvertScreenToWorld(const Vector2& screenPos);

	Input* input_;

	//stone
	std::unique_ptr<Rigid3dObject> stone_;
	WorldTransform worldTransform_;
	Vector3 velocity_;
	bool isDragging_ = false;
	Vector2 dragStartPos_;
	Vector2 dragCurrentPos_;
	float maxSpeed_ = 0.7f;
	float friction_ = 0.98f;

};

