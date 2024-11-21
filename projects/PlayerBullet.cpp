#include "PlayerBullet.h"

PlayerBullet::~PlayerBullet()
{
}

void PlayerBullet::Initialize(std::unique_ptr<BaseModel> model, const Vector3& position, const Vector3& velocity)
{
	// NULLポインタチェック
	assert(model);
	// モデルをセット
	model_ = std::move(model);
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
	if (isDead_) {
		return;
	}

	// 弾の移動処理
	worldTransform_.translation_ +=  velocity_;

	// 寿命タイマーを減らす
	--deathTimer_;
	if (deathTimer_ <= 0) {
		isDead_ = true;
	}

	// ワールドトランスフォームの行列を更新
	worldTransform_.UpdateMatrix();
}

void PlayerBullet::Draw(Camera* camera)
{
	if (isDead_) {
		return;
	}

	// モデルを描画
	assert(model_);
	model_->Draw(worldTransform_, camera);
}
