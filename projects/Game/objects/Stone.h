#pragma once
#include "Rigid3dObject.h"
#include "Input.h"
#include "MapChipField.h"

class Stone
{
public:

	Stone();

	void Initialize(BaseModel* model, const Vector3& position, MapChipField* mapChipField);

	void Update();

	void Draw(Camera* camera);

	Vector3& GetPosition() { return worldTransform_.translation_; }

	Vector2& GetDragStartPos() { return dragStartPos_; }

	Vector2& GetDragCurrentPos() { return dragCurrentPos_; }

private:

	// マップとの当たり判定情報
	struct CollisionMapInfo {
		//手前か奥の衝突判定
		bool isWallCollisionDepth = false;
		//左右の衝突判定
		bool isWallCollisionWidth = false;
		Vector3 move;
	};

	enum Corner {
		kRightFront,	//右手前
		kLeftFront,		//左手前
		kRightBack,		//右奥
		kLeftBack,		//左奥

		kNumCorner		//要素数

	};

	//移動入力
	void Move();

	//マップ衝突判定
	void MapCollision();
	void MapCollisionBack();
	void MapCollisionFront();
	void MapCollisionRight();
	void MapCollisionLeft();

	//判定結果を反映して移動させる
	void MoveAppli();

	//壁に接触している場合の処理
	void WallCollisionDepth();
	void WallCollisionWidth();

	Vector3 CornerPosition(const Vector3& center, Corner corner);

	Vector3 ConvertScreenToWorld(const Vector2& screenPos);

	Input* input_;

	//stone
	std::unique_ptr<Rigid3dObject> object_;
	WorldTransform worldTransform_;
	Vector3 velocity_;
	bool isDragging_ = false;
	Vector2 dragStartPos_;
	Vector2 dragCurrentPos_;
	float maxSpeed_ = 0.7f;
	float friction_ = 0.98f;

	//マップチップによるフィールド
	MapChipField* mapChipField_ = nullptr;

	CollisionMapInfo collisionMapInfo_;

	//キャラクターの当たり判定サイズ
	const float kWidth = 1.0f;
	const float kDepth = 1.0f;

	//余白
	const float kBlank = 0.01f;
};

