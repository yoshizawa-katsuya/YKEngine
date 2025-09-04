#pragma once
#include "Input.h"
class Camera;
class MapChipField;
class Input;

#include "BaseCharacter.h"
#include "IPlayerState.h"
#include "PlayerIdleState.h"
#include "PlayerWalkState.h"
#include "PlayerJumpState.h"

class Player : public BaseCharacter
{
class Player {
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

public:
	/// <summary>
	/// 初期化
	///  </summary>
	void Initialize(BaseModel* model,  const Vector3& position);

	void Initialize(BaseModel* model) override;

	void Update() override;

	void Draw(Camera* camera) override;

	void ChangeState(std::unique_ptr<IPlayerState> state);

private: // 非公開メンバ関数

	void HandleInput();

	void StateUpdate();

	
	/// <summary>
	/// 更新
	///  </summary>
	void Update();

	/// <summary>
	/// 描画
	///  </summary>
	void Draw(Camera* camera);

	//移動入力
	void Move();

	//マップ衝突判定
	void MapCollision(CollisionMapInfo& info);
	void MapCollisionUp(CollisionMapInfo& info);
	void MapCollisionBottom(CollisionMapInfo& info);
	void MapCollisionRight(CollisionMapInfo& info);
	void MapCollisionLeft(CollisionMapInfo& info);

	//判定結果を反映して移動させる
	void MoveAppli(const CollisionMapInfo& info);

	//天井に接触している場合の処理
	void CeilingCollision(const CollisionMapInfo& info);

	//地面に接触している場合の処理
	void GroundCollision(const CollisionMapInfo& info);

	//壁に接触している場合の処理
	void WallCollision(const CollisionMapInfo& info);

	Vector3 CornerPosition(const Vector3& center, Corner corner);

	//ワールド座標を取得
	Vector3 GetWorldPosition();

	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }

	WorldTransform& GetWorldTransform() { return worldTransform_; };

	const Vector3& GetVelocity() const { return velocity_; }

	//AABBを取得
	AABB GetAABB();

	//デスフラグのgetter
	bool GetIsDead() const { return isDead_; }

private:

	Input* input_;

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

	//重力加速度
	const float kGravityAcceleration_ = 0.03f;

	const float kLimitFallSpeed_ = 1.0f;
	const float kJumpAcceleration_ = 0.5f;

	//キャラクターの当たり判定サイズ
	const float kWidth_ = 1.0f;
	const float kHeight_ = 1.0f;

	//余白
	const float kBlank_ = 0.01f;

	//ワールド変換データ
	WorldTransform worldTransform_;
	//オブジェクト
	std::unique_ptr<Rigid3dObject> object_;

	//マップチップによるフィールド
	MapChipField* mapChipField_ = nullptr;

	//デスフラグ
	bool isDead_ = false;
	std::unique_ptr<IPlayerState> state_ = nullptr;

};

