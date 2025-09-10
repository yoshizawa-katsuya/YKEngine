#include "BaseCharacter.h"

#include "Rigid3dObject.h"
#include "Input.h"
#include <numbers>
#include "MapChipField.h"
#include "Easing.h"
#include "Lerp.h"

void BaseCharacter::Initialize(BaseModel* model, const Vector3& position) {

	object_ = std::make_unique<Rigid3dObject>();
	object_->Initialize(model);

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
	worldTransform_.UpdateMatrix();
	object_->WorldTransformUpdate(worldTransform_);
}

void BaseCharacter::Update() {

	//衝突判定を初期化
	CollisionMapInfo collisionMapInfo;
	//移動量に速度の値をコピー
	collisionMapInfo.move = velocity_;

	//マップ衝突チェック
	MapCollision(collisionMapInfo);

	//移動
	MoveAppli(collisionMapInfo);

	//天井衝突
	CeilingCollision(collisionMapInfo);

	//地面衝突
	GroundCollision(collisionMapInfo);

	//壁衝突
	WallCollision(collisionMapInfo);

	//トゲ衝突
	SpineCollision(collisionMapInfo);

	// 旋回制御
	if (turnTimer_ < 1.0f) {
		turnTimer_ += 1.0f / 60.0f;

		// 左右の自キャラ角度テーブル
		float destinationRotationYTable[] = { std::numbers::pi_v<float> / 2.0f, std::numbers::pi_v<float> *3.0f / 2.0f };

		// 状態に応じた角度を取得する
		float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];
		float timer = EaseOutCubic(turnTimer_);
		// 自キャラの角度を設定する
		worldTransform_.rotation_.y = Lerp(turnFirstRotationY_, destinationRotationY, timer);
	}


	worldTransform_.UpdateMatrix();
	object_->WorldTransformUpdate(worldTransform_);
}

void BaseCharacter::Draw(Camera* camera) {

	object_->CameraUpdate(camera);
	object_->Draw();
}

Vector3 BaseCharacter::GetWorldPosition()
{
	//ワールド座標を入れる変数
	Vector3 worldPos;
	//ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform_.worldMatrix_.m[3][0];
	worldPos.y = worldTransform_.worldMatrix_.m[3][1];
	worldPos.z = worldTransform_.worldMatrix_.m[3][2];

	return worldPos;
}

AABB BaseCharacter::GetAABB()
{
	AABB aabb;
	Vector3 worldPos = GetWorldPosition();
	aabb.min.x = worldPos.x - (kWidth_ / 2.0f);
	aabb.min.y = worldPos.y - (kWidth_ / 2.0f);
	aabb.min.z = worldPos.z - (kWidth_ / 2.0f);

	aabb.max.x = worldPos.x + (kWidth_ / 2.0f);
	aabb.max.y = worldPos.y + (kWidth_ / 2.0f);
	aabb.max.z = worldPos.z + (kWidth_ / 2.0f);

	return aabb;
}

void BaseCharacter::MapCollision(CollisionMapInfo& info) {

	MapCollisionRight(info);
	MapCollisionLeft(info);
	MapCollisionUp(info);
	MapCollisionBottom(info);
}

void BaseCharacter::MapCollisionUp(CollisionMapInfo& info) {
	if (info.move.y <= 0) {
		return;
	}

	//移動後の4つの角の座標
	std::array<Vector3, kNumCorner> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(
			{ worldTransform_.translation_.x + info.move.x, worldTransform_.translation_.y + info.move.y, worldTransform_.translation_.z + info.move.z }, static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	//真上の当たり判定を行う
	bool hit = false;
	// 右上点の判定
	MapChipField::IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	CheckHitMapChipType(mapChipType);

	if (mapChipType == MapChipType::kBlock || mapChipType == MapChipType::kClosedDoor || mapChipType == MapChipType::kInactiveDisappear || mapChipType == MapChipType::kActiveAppear || mapChipType == MapChipType::kDoorTrigger || mapChipType == MapChipType::kSpineTrigger || mapChipType == MapChipType::kDisappearTrigger || mapChipType == MapChipType::kAppearTrigger) {
		hit = true;
	}
	//左上点の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	CheckHitMapChipType(mapChipType);
	
	if (mapChipType == MapChipType::kBlock || mapChipType == MapChipType::kClosedDoor) {
		hit = true;
	}

	if (IsHitObstacle())
	{
		//めり込みを排除する方向に移動量を設定する
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);
		//めり込み先ブロックの範囲矩形
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.move.y = (std::max)(0.0f, (rect.bottom - worldTransform_.translation_.y) - (kHeight_ / 2 + kBlank_));
		//天井に当たったことを記録する
		info.isCeilingCollision = true;
		if (hitSpine_ && !hitBlock_)
		{
			info.isSpineCollision = true;
		}
	}
	//ヒット情報リセット
	HitReset();
}

void BaseCharacter::MapCollisionBottom(CollisionMapInfo& info) {
	//下降あり?
	if (info.move.y >= 0) {
		return;
	}

	// 移動後の4つの角の座標
	std::array<Vector3, kNumCorner> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(
			{ worldTransform_.translation_.x + info.move.x, worldTransform_.translation_.y + info.move.y, worldTransform_.translation_.z + info.move.z }, static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	// 真下の当たり判定を行う
	bool hit = false;
	// 左下点の判定
	MapChipField::IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	CheckHitMapChipType(mapChipType);

	if (mapChipType == MapChipType::kBlock || mapChipType == MapChipType::kClosedDoor || mapChipType == MapChipType::kInactiveDisappear || mapChipType == MapChipType::kActiveAppear) {
		hit = true;
	}
	// 右下点の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock || mapChipType == MapChipType::kClosedDoor || mapChipType == MapChipType::kInactiveDisappear || mapChipType == MapChipType::kActiveAppear) {
		hit = true;
	}
	CheckHitMapChipType(mapChipType);

	//ブロックにヒット?
	if (IsHitObstacle())
	{
		// めり込みを排除する方向に移動量を設定する
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom]);
		// めり込み先ブロックの範囲矩形
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.move.y = (std::min)(0.0f, (rect.top - worldTransform_.translation_.y) + (kHeight_ / 2 + kBlank_));
		// 地面に当たったことを記録する
		info.landing = true;
		if (hitSpine_ && !hitBlock_)
		{
			info.isSpineCollision = true;
		}
	}
	//ヒット情報リセット
	HitReset();
}

void BaseCharacter::MapCollisionRight(CollisionMapInfo& info)
{
	//右移動あり?
	if (info.move.x <= 0) {
		return;
	}

	// 移動後の4つの角の座標
	std::array<Vector3, kNumCorner> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(
			{ worldTransform_.translation_.x + info.move.x, worldTransform_.translation_.y + info.move.y, worldTransform_.translation_.z + info.move.z }, static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	// 真右の当たり判定を行う
	bool hit = false;
	// 右上点の判定
	MapChipField::IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	CheckHitMapChipType(mapChipType);

	if (mapChipType == MapChipType::kBlock || mapChipType == MapChipType::kClosedDoor || mapChipType == MapChipType::kInactiveDisappear || mapChipType == MapChipType::kActiveAppear || mapChipType == MapChipType::kDoorTrigger || mapChipType == MapChipType::kSpineTrigger || mapChipType == MapChipType::kDisappearTrigger || mapChipType == MapChipType::kAppearTrigger) {
		hit = true;
	}
	// 右下点の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	CheckHitMapChipType(mapChipType);
	if (mapChipType == MapChipType::kBlock || mapChipType == MapChipType::kClosedDoor || mapChipType == MapChipType::kInactiveDisappear || mapChipType == MapChipType::kActiveAppear || mapChipType == MapChipType::kDoorTrigger || mapChipType == MapChipType::kSpineTrigger || mapChipType == MapChipType::kDisappearTrigger || mapChipType == MapChipType::kAppearTrigger) {
		hit = true;
	}

	// ブロックにヒット?
	if (IsHitObstacle())
	{
		// めり込みを排除する方向に移動量を設定する
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom]);
		// めり込み先ブロックの範囲矩形
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.move.x = (std::max)(0.0f, (rect.left - worldTransform_.translation_.x) - (kWidth_ / 2 + kBlank_));
		// 壁に当たったことを記録する
		info.isWallCollision = true;
		if (hitSpine_ && !hitBlock_)
		{
			info.isSpineCollision = true;
		}
	}
	//ヒット情報リセット
	HitReset();
}

void BaseCharacter::MapCollisionLeft(CollisionMapInfo& info) {
	// 左移動あり?
	if (info.move.x >= 0) {
		return;
	}

	// 移動後の4つの角の座標
	std::array<Vector3, kNumCorner> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] =
			CornerPosition({ worldTransform_.translation_.x + info.move.x, worldTransform_.translation_.y + info.move.y, worldTransform_.translation_.z + info.move.z }, static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	// 真左の当たり判定を行う
	bool hit = false;
	// 左上点の判定
	MapChipField::IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock || mapChipType == MapChipType::kClosedDoor || mapChipType == MapChipType::kInactiveDisappear || mapChipType == MapChipType::kActiveAppear || mapChipType == MapChipType::kDoorTrigger || mapChipType == MapChipType::kSpineTrigger || mapChipType == MapChipType::kDisappearTrigger || mapChipType == MapChipType::kAppearTrigger) {
		hit = true;
	}
	CheckHitMapChipType(mapChipType);

	// 左下点の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock || mapChipType == MapChipType::kClosedDoor || mapChipType == MapChipType::kInactiveDisappear || mapChipType == MapChipType::kActiveAppear || mapChipType == MapChipType::kDoorTrigger || mapChipType == MapChipType::kSpineTrigger || mapChipType == MapChipType::kDisappearTrigger || mapChipType == MapChipType::kAppearTrigger) {
		hit = true;
	}
	CheckHitMapChipType(mapChipType);

	// ブロックにヒット?
	if (IsHitObstacle())
	{
		// めり込みを排除する方向に移動量を設定する
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom]);
		// めり込み先ブロックの範囲矩形
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.move.x = (std::min)(0.0f, (rect.right - worldTransform_.translation_.x) + (kWidth_ / 2 + kBlank_));
		// 壁に当たったことを記録する
		info.isWallCollision = true;
		if (hitSpine_ && !hitBlock_)
		{
			info.isSpineCollision = true;
		}
	}
	//ヒット情報リセット
	HitReset();
}

void BaseCharacter::MoveAppli(const CollisionMapInfo& info) {
	worldTransform_.translation_ += info.move;
}

void BaseCharacter::CeilingCollision(const CollisionMapInfo& info) {
	//天井に当たった?
	if (info.isCeilingCollision) {
		velocity_.y = 0;
	}
}

void BaseCharacter::GroundCollision(CollisionMapInfo& info) {
	//自キャラが設置状態?
	if (onGround_) {

		//ジャンプ開始
		if (velocity_.y > 0.0f) {
			onGround_ = false;
		}
		else {

			// 移動後の4つの角の座標
			std::array<Vector3, kNumCorner> positionsNew;

			for (uint32_t i = 0; i < positionsNew.size(); ++i) {
				positionsNew[i] =
					CornerPosition(worldTransform_.translation_, static_cast<Corner>(i));
			}

			MapChipType mapChipType;
			// 真下の当たり判定を行う
			bool hit = false;
			// 左下点の判定
			MapChipField::IndexSet indexSet;
			indexSet = mapChipField_->GetMapChipIndexSetByPosition({ positionsNew[kLeftBottom].x, positionsNew[kLeftBottom].y - kBlank_ * 2, positionsNew[kLeftBottom].z });
			mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
			CheckHitMapChipType(mapChipType);

			if (mapChipType == MapChipType::kBlock || mapChipType == MapChipType::kClosedDoor || mapChipType == MapChipType::kInactiveDisappear || mapChipType == MapChipType::kActiveAppear || mapChipType == MapChipType::kDoorTrigger || mapChipType == MapChipType::kSpineTrigger || mapChipType == MapChipType::kDisappearTrigger || mapChipType == MapChipType::kAppearTrigger) {
				hit = true;
			}
			// 右下点の判定
			indexSet = mapChipField_->GetMapChipIndexSetByPosition({ positionsNew[kRightBottom].x, positionsNew[kRightBottom].y - kBlank_ * 2, positionsNew[kRightBottom].z });
			mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
			CheckHitMapChipType(mapChipType);

			if (mapChipType == MapChipType::kBlock || mapChipType == MapChipType::kClosedDoor || mapChipType == MapChipType::kInactiveDisappear || mapChipType == MapChipType::kActiveAppear || mapChipType == MapChipType::kDoorTrigger || mapChipType == MapChipType::kSpineTrigger || mapChipType == MapChipType::kDisappearTrigger || mapChipType == MapChipType::kAppearTrigger) {
				hit = true;
			}

			if (hitSpine_ && !hitBlock_)
			{
				info.isSpineCollision = true;
				/*if (mapChipType == MapChipType::kBlock || mapChipType == MapChipType::kDoor) {
					hit = true;
				}*/
				
			}
			
			// ブロックにヒット?
			if (!IsHitObstacle()) {
				//空中状態に切り替える
				onGround_ = false;
			}
		}

		//ヒット情報リセット
		HitReset();

	}
	else {

		if (info.landing) {
			//着地状態に切り替える（落下を止める）
			onGround_ = true;
			//Y速度をゼロにする
			velocity_.y = 0.0f;
		}
	}
}

void BaseCharacter::WallCollision(const CollisionMapInfo& info) {
}

void BaseCharacter::SpineCollision(const CollisionMapInfo& info)
{
}

Vector3 BaseCharacter::CornerPosition(const Vector3& center, Corner corner)
{

	Vector3 offsetTable[kNumCorner] = {
		{+kWidth_ / 2.0f, -kHeight_ / 2.0f, 0},	//kRightBottom
		{-kWidth_ / 2.0f, -kHeight_ / 2.0f, 0},	//kLeftBottom
		{+kWidth_ / 2.0f, +kHeight_ / 2.0f, 0},	//kRightTop
		{-kWidth_ / 2.0f, +kHeight_ / 2.0f, 0},	//kLeftTop
	};

	return { center.x + offsetTable[static_cast<uint32_t>(corner)].x,
			center.y + offsetTable[static_cast<uint32_t>(corner)].y,
			center.z + offsetTable[static_cast<uint32_t>(corner)].z };
}

void BaseCharacter::CheckHitMapChipType(MapChipType mapChipType)
{
	switch (mapChipType)
	{
	case MapChipType::kBlock:
		hitBlock_ = true;
		break;

	case MapChipType::kSpineTrap:
		hitSpine_ = true;
		break;

	case MapChipType::kActiveSpine:
		hitSpine_ = true;
		break;

	case MapChipType::kActiveAppear:
		hitBlock_ = true;
		break;

	case MapChipType::kInactiveDisappear:

		hitBlock_ = true;

		break;
	default:
		break;
	}
}

bool BaseCharacter::IsHitObstacle()
{
	return hitBlock_ || hitSpine_;
}

void BaseCharacter::HitReset()
{
	hitBlock_ = false;
	hitSpine_ = false;
}
