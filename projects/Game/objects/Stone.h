#pragma once
#include "Rigid3dObject.h"
#include "Input.h"
#include "MapChipField.h"

class Stone
{
public:

	struct AABB {
		Vector3 min;
		Vector3 max;
	};

	enum class State {
		Waiting,
		PowerSetting,
		Flying,
		Stopped
	};

	Stone();

	void Initialize(BaseModel* model, const Vector3& position);

	void Update();

	void Draw(Camera* camera);

	void HandleCollision(Stone& other);

	void SetSpeed(float speed) { speed_ = speed; }
	void SetState(State state) { state_ = state; }
	void SetPosition(const Vector3& position) { worldTransform_.translation_ = position; }

	Vector3& GetPosition() { return worldTransform_.translation_; }

	Vector2& GetDragStartPos() { return dragStartPos_; }

	Vector2& GetDragCurrentPos() { return dragCurrentPos_; }

	AABB GetAABB()const;
	State GetState()const { return state_; }
	float GetSpeed()const { return speed_; }
	Vector3 GetPosition()const { return worldTransform_.translation_; }
	bool CheckCollision(const Stone& stone1, const Stone& stone2);

private:

	//移動入力
	void Move();
	Input* input_;

	//stone
	std::unique_ptr<Rigid3dObject> object_;
	WorldTransform worldTransform_;
	Vector3 velocity_;
	Vector2 dragStartPos_;
	Vector2 dragCurrentPos_;
	State state_ = State::Waiting;
	float maxSpeed_ = 0.7f;
	float friction_ = 0.98f;
	float angle_ = 0.0f;
	float power_ = 0.0f;
	float powerChangeRate_ = 0.005f;
	bool powerIncreasing_ = true;
	float speed_ = 0.0f;
	float maxSpeed = 0.05f;

	//マップチップによるフィールド
	MapChipField* mapChipField_ = nullptr;
};

