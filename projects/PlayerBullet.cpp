#include "PlayerBullet.h"

PlayerBullet::~PlayerBullet()
{
}

void PlayerBullet::Initialize(BaseModel* model, const Vector3& position, const Vector3& velocity)
{
	// NULLポインタチェック
	assert(model);
	model_ = model;
	// テクスチャ読み込み
	textureHandle_ = TextureManager::GetInstance()->Load("white.png");
	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	// 引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;
	// 引数で受け取った速度をメンバ変数に代入
	velocity_ = velocity;
}

void PlayerBullet::Update()
{
}

void PlayerBullet::Draw(Camera* camera)
{
}
