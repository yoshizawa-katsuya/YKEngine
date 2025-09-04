#pragma once
#include "Rigid3dObject.h"
#include "WorldTransform.h"
#include "SRTAnimator.h"
#include "Animation.h"

class Camera;
class MapChipField;

class BaseCharacter
{
public:

	struct PhysicsParam {
		const float kGravityAcceleration;
		const float kLimitFallSpeed;
		const float kJumpAcceleration;
	};

private: 

	// マップとの当たり判定情報
	struct CollisionMapInfo {
		bool isCeilingCollision = false;
		bool landing = false;
		bool isWallCollision = false;
		Vector3 move;
	};

	enum Corner {
		kRightBottom,	//右下
		kLeftBottom,	//左下
		kRightTop,		//右上
		kLeftTop,		//左上

		kNumCorner		//要素数

	};

public: // 公開メンバ関数

	virtual void Initialize(BaseModel* model, const Vector3& position);

	virtual void Update();

	virtual void Draw(Camera* camera);

	//ワールド座標を取得
	Vector3 GetWorldPosition();

	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }

	WorldTransform& GetWorldTransform() { return worldTransform_; };

	Vector3& GetVelocity() { return velocity_; }

	void SetVelocity(const Vector3& velocity) { velocity_ = velocity; }

	//AABBを取得
	AABB GetAABB();

	//デスフラグのgetter
	bool GetIsDead() const { return isDead_; }

	PhysicsParam GetPhysicsParam() { return physicsParam_; }

private:

	//マップ衝突判定
	void MapCollision(CollisionMapInfo& info);
	void MapCollisionUp(CollisionMapInfo& info);
	void MapCollisionBottom(CollisionMapInfo& info);
	void MapCollisionRight(CollisionMapInfo& info);
	void MapCollisionLeft(CollisionMapInfo& info);

private:

	//判定結果を反映して移動させる
	void MoveAppli(const CollisionMapInfo& info);

	//天井に接触している場合の処理
	void CeilingCollision(const CollisionMapInfo& info);

	//地面に接触している場合の処理
	void GroundCollision(const CollisionMapInfo& info);

	//壁に接触している場合の処理
	void WallCollision(const CollisionMapInfo& info);

	Vector3 CornerPosition(const Vector3& center, Corner corner);

protected:

	//Transform変数を作る
	WorldTransform worldTransform_;

	std::unique_ptr<Rigid3dObject> object_;

	enum  class LRDirection {
		kRight,
		kLeft,
	};

	LRDirection lrDirection_ = LRDirection::kRight;

	//旋回開始時の角度
	float turnFirstRotationY_ = 0.0f;
	//旋回タイマー
	float turnTimer_ = 0.0f;
	//旋回時間<秒>
	const float kTimeTurn_ = 0.03f;

	Vector3 velocity_ = {};

	//設置状態フラグ
	bool onGround_ = true;

	const float kMoveSpeed_ = 0.1f;

	PhysicsParam physicsParam_ = { 0.03f, 1.0f, 0.5f};

	//キャラクターの当たり判定サイズ
	const float kWidth_ = 1.0f;
	const float kHeight_ = 1.0f;

	//余白
	const float kBlank_ = 0.01f;

	//マップチップによるフィールド
	MapChipField* mapChipField_ = nullptr;

	//デスフラグ
	bool isDead_ = false;
};

