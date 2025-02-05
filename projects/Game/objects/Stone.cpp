#include "Stone.h"

Stone::Stone()
	:input_(Input::GetInstance())
{
}

void Stone::Initialize(BaseModel* model, const Vector3& position, MapChipField* mapChipField)
{
	object_ = std::make_unique<Rigid3dObject>();
	object_->Initialize(model);

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	mapChipField_ = mapChipField;
}

void Stone::Update()
{
	//移動入力
	Move();

	//衝突判定を初期化
	collisionMapInfo_.isWallCollisionDepth = false;
	collisionMapInfo_.isWallCollisionWidth = false;
	//移動量に速度の値をコピー
	collisionMapInfo_.move = velocity_;

	//マップ衝突チェック
	MapCollision();

	//移動
	MoveAppli();

	WallCollisionDepth();
	WallCollisionWidth();
}

void Stone::Draw(Camera* camera)
{
	object_->CameraUpdate(camera);
	object_->Draw();
}

void Stone::Move()
{
	Vector2 mousePos = input_->GetMousePosition();

	if (input_->PushMouseLeft()) {
		Vector3 clickPos = ConvertScreenToWorld(mousePos);

		float distance = static_cast<float>(sqrt(pow(clickPos.x - worldTransform_.translation_.x, 2) + pow(clickPos.z - worldTransform_.translation_.z, 2)));

		if (distance < 1.0f) {
			dragStartPos_ = mousePos;
			isDragging_ = true;
		}
	}
	if (isDragging_ && input_->HoldMouseLeft()) {
		dragCurrentPos_ = input_->GetMousePosition();
	}
	if (isDragging_ && input_->ReleaseMouseLeft()) {
		isDragging_ = false;
		Vector2 dragVector = { dragStartPos_.x - dragCurrentPos_.x  , dragStartPos_.y - dragCurrentPos_.y };

		float length = sqrt(dragVector.x * dragVector.x + dragVector.y * dragVector.y);
		if (length > 0) {
			dragVector.x /= length;
			dragVector.y /= length;
		}

		float speed = std::min(length * 0.03f, maxSpeed_);
		velocity_ = { dragVector.x * speed, 0.0f, -dragVector.y * speed };

	}
}

void Stone::MapCollision()
{
	MapCollisionBack();
	MapCollisionFront();
	MapCollisionRight();
	MapCollisionLeft();
}

void Stone::MapCollisionBack()
{
	if (collisionMapInfo_.move.z <= 0) 
	{
		return;
	}

	//移動後の4つの角の座標
	std::array<Vector3, kNumCorner> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); ++i) 
	{
		positionsNew[i] = CornerPosition(
			{ worldTransform_.translation_.x + collisionMapInfo_.move.x, worldTransform_.translation_.y + collisionMapInfo_.move.y, worldTransform_.translation_.z + collisionMapInfo_.move.z }, static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	//奥の当たり判定を行う
	bool hit = false;
	//左奥点の判定
	MapChipField::IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBack]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.zIndex);
	if (mapChipType == MapChipType::kBox || mapChipType == MapChipType::ice) {
		hit = true;
	}
	//右奥点の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBack]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.zIndex);
	if (mapChipType == MapChipType::kBox || mapChipType == MapChipType::ice) {
		hit = true;
	}

	if (hit) {
		//めり込みを排除する方向に移動量を設定する
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBack]);
		//めり込み先ブロックの範囲矩形
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.zIndex);
		collisionMapInfo_.move.z = (std::max)(0.0f, (rect.front - worldTransform_.translation_.z) - (kDepth / 2 + kBlank));
		//壁に当たったことを記録する
		collisionMapInfo_.isWallCollisionDepth = true;
	}

}

void Stone::MapCollisionFront()
{
	if (collisionMapInfo_.move.z >= 0)
	{
		return;
	}

	//移動後の4つの角の座標
	std::array<Vector3, kNumCorner> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); ++i)
	{
		positionsNew[i] = CornerPosition(
			{ worldTransform_.translation_.x + collisionMapInfo_.move.x, worldTransform_.translation_.y + collisionMapInfo_.move.y, worldTransform_.translation_.z + collisionMapInfo_.move.z }, static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	//手前の当たり判定を行う
	bool hit = false;
	//左手前点の判定
	MapChipField::IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftFront]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.zIndex);
	if (mapChipType == MapChipType::kBox || mapChipType == MapChipType::ice) {
		hit = true;
	}
	//右手前点の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightFront]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.zIndex);
	if (mapChipType == MapChipType::kBox || mapChipType == MapChipType::ice) {
		hit = true;
	}

	if (hit) {
		//めり込みを排除する方向に移動量を設定する
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightFront]);
		//めり込み先ブロックの範囲矩形
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.zIndex);
		collisionMapInfo_.move.z = (std::min)(0.0f, (rect.back - worldTransform_.translation_.z) + (kDepth / 2 + kBlank));
		//壁に当たったことを記録する
		collisionMapInfo_.isWallCollisionDepth = true;
	}

}

void Stone::MapCollisionRight()
{
	if (collisionMapInfo_.move.x <= 0)
	{
		return;
	}

	//移動後の4つの角の座標
	std::array<Vector3, kNumCorner> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); ++i)
	{
		positionsNew[i] = CornerPosition(
			{ worldTransform_.translation_.x + collisionMapInfo_.move.x, worldTransform_.translation_.y + collisionMapInfo_.move.y, worldTransform_.translation_.z + collisionMapInfo_.move.z }, static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	//奥の当たり判定を行う
	bool hit = false;
	//左奥点の判定
	MapChipField::IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightFront]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.zIndex);
	if (mapChipType == MapChipType::kBox || mapChipType == MapChipType::ice) {
		hit = true;
	}
	//右奥点の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBack]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.zIndex);
	if (mapChipType == MapChipType::kBox || mapChipType == MapChipType::ice) {
		hit = true;
	}

	if (hit) {
		//めり込みを排除する方向に移動量を設定する
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBack]);
		//めり込み先ブロックの範囲矩形
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.zIndex);
		collisionMapInfo_.move.x = (std::max)(0.0f, (rect.left - worldTransform_.translation_.x) - (kWidth / 2 + kBlank));
		//壁に当たったことを記録する
		collisionMapInfo_.isWallCollisionWidth = true;
	}

}

void Stone::MapCollisionLeft()
{
	if (collisionMapInfo_.move.x >= 0)
	{
		return;
	}

	//移動後の4つの角の座標
	std::array<Vector3, kNumCorner> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); ++i)
	{
		positionsNew[i] = CornerPosition(
			{ worldTransform_.translation_.x + collisionMapInfo_.move.x, worldTransform_.translation_.y + collisionMapInfo_.move.y, worldTransform_.translation_.z + collisionMapInfo_.move.z }, static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	//手前の当たり判定を行う
	bool hit = false;
	//左手前点の判定
	MapChipField::IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftFront]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.zIndex);
	if (mapChipType == MapChipType::kBox || mapChipType == MapChipType::ice) {
		hit = true;
	}
	//右手前点の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBack]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.zIndex);
	if (mapChipType == MapChipType::kBox || mapChipType == MapChipType::ice) {
		hit = true;
	}

	if (hit) {
		//めり込みを排除する方向に移動量を設定する
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBack]);
		//めり込み先ブロックの範囲矩形
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.zIndex);
		collisionMapInfo_.move.x = (std::min)(0.0f, (rect.right - worldTransform_.translation_.x) + (kDepth / 2 + kBlank));
		//壁に当たったことを記録する
		collisionMapInfo_.isWallCollisionWidth = true;
	}

}

void Stone::MoveAppli()
{
	if (collisionMapInfo_.isWallCollisionWidth) {
		collisionMapInfo_.move.z = velocity_.z;
	}
	worldTransform_.translation_ += collisionMapInfo_.move;
	velocity_ *= friction_;

	if (fabs(velocity_.x) < 0.01f) velocity_.x = 0.0f;
	if (fabs(velocity_.z) < 0.01f) velocity_.z = 0.0f;


	worldTransform_.UpdateMatrix();
	object_->WorldTransformUpdate(worldTransform_);
}

void Stone::WallCollisionDepth()
{
	if (collisionMapInfo_.isWallCollisionDepth && !collisionMapInfo_.isWallCollisionWidth) {
		velocity_.z = -velocity_.z;
	}
}

void Stone::WallCollisionWidth()
{
	if (collisionMapInfo_.isWallCollisionWidth) {
		velocity_.x = -velocity_.x;
	}
}

Vector3 Stone::CornerPosition(const Vector3& center, Corner corner)
{
	Vector3 offsetTable[kNumCorner] = {
		{+kWidth / 2.0f, 0.0f, -kDepth / 2.0f},	//kRightFront
		{-kWidth / 2.0f, 0.0f, -kDepth / 2.0f},	//kLeftFront
		{+kWidth / 2.0f, 0.0f, +kDepth / 2.0f},	//kRightBack
		{-kWidth / 2.0f, 0.0f, +kDepth / 2.0f},	//kLeftBack
	};

	return { center.x + offsetTable[static_cast<uint32_t>(corner)].x,
			center.y + offsetTable[static_cast<uint32_t>(corner)].y,
			center.z + offsetTable[static_cast<uint32_t>(corner)].z };
}

Vector3 Stone::ConvertScreenToWorld(const Vector2& screenPos)
{
	float worldX = 0.0349f * (screenPos.x - 152.0f) - 12.0f;
	float worldZ = -0.035f * (screenPos.y - 136.0f) + 7.5f;
	return Vector3(worldX, 0.0f, worldZ);
}
