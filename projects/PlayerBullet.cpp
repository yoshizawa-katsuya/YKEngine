#include "PlayerBullet.h"
#include "imgui/imgui.h"
#include "Rigid3dObject.h"

PlayerBullet::~PlayerBullet()
{

}

void PlayerBullet::Initialize(BaseModel* model, const Vector3& position, const Vector3& velocity)
{
	// NULLポインタチェック
	assert(model);
	// モデルをセット
	//model_ = model;
	object_ = std::make_unique<Rigid3dObject>();
	object_->Initialize(model);
	// テクスチャ読み込み
	//textureHandle_ = TextureManager::GetInstance()->Load("white.png");
	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	// 引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;
	// 引数で受け取った速度をメンバ変数に代入
	velocity_ = velocity;
	worldTransform_.UpdateMatrix();
}

void PlayerBullet::Update()
{

	// 弾の移動処理
	worldTransform_.translation_ += velocity_;

	// 寿命タイマーを減らす
	--deathTimer_;
	if (deathTimer_ <= 0) {
		isDead_ = true;
	}

	// ワールドトランスフォームの行列を更新
	worldTransform_.UpdateMatrix();
#ifdef _DEBUG

	ImGui::Begin("PlayerBullet");
	ImGui::DragFloat3("translate", &worldTransform_.translation_.x, 0.01f);
	ImGui::DragFloat3("rotate", &worldTransform_.rotation_.x, 0.01f);
	ImGui::DragFloat3("scale", &worldTransform_.scale_.x, 0.01f);
	ImGui::End();

#endif // _DEBUG
}

void PlayerBullet::Draw(Camera* camera)
{

	// モデルを描画
	object_->Update(worldTransform_, camera);
	object_->Draw();
}

void PlayerBullet::OnCollision()
{
	isDead_ = true;
}

Vector3 PlayerBullet::GetWorldPosition()
{
	// ワールド座標を入れる変数
	Vector3 worldPos = {};
	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}
