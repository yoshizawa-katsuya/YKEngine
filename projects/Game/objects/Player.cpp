#include "Player.h"
#include "imgui/imgui.h"
#include <RigidModel.h>
#include "Matrix.h"
#include "Rigid3dObject.h"
Player::~Player()
{
	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}

}

void Player::Initialize(const std::vector<BaseModel*>& models) {

	//bulletModel = std::make_shared<RigidModel>();
	bulletModel = std::make_unique<RigidModel>();
	bulletModel->CreateModel("./resources/player/PlayerBullet", "PlayerBullet.obj");
	// 各部位のWorldTransformを初期化
	worldTransforms_.resize(models.size());
	objects_.resize(models.size());
	for (auto& transform : worldTransforms_) {
		transform.Initialize();
		transform.UpdateMatrix();
	}

	for (std::unique_ptr<Base3dObject>& object : objects_) {
		object = std::make_unique<Rigid3dObject>();
	}

	for (uint32_t i = 0; i < models.size(); i++) {
		objects_[i]->Initialize(models[i]);
	}

	fireCoolTime = kCoolDownTime;
	/// <summary>
	/// 体のパーツの座標設定
	/// </summary>

	// 体及び全体
	worldTransforms_[0].scale_ = { 0.8f,0.8f,0.8f };
	worldTransforms_[0].rotation_.y = -1.56f;
	worldTransforms_[0].translation_.y = -0.2f;

	// 頭部
	worldTransforms_[1].translation_.y = 0.7f;

	// 右腕
	worldTransforms_[2].rotation_.z = 1.58f;
	worldTransforms_[2].translation_.y = 0.33f;
	worldTransforms_[2].translation_.z = -0.68f;

	// 左腕
	worldTransforms_[3].translation_.y = 0.34f;
	worldTransforms_[3].translation_.z = 0.68f;

	// 右足
	worldTransforms_[4].translation_.y = -0.4f;
	worldTransforms_[4].translation_.z = -0.18f;

	// 左足
	worldTransforms_[5].translation_.y = -0.4f;
	worldTransforms_[5].translation_.z = 0.22f;

	// ブースター
	worldTransforms_[6].translation_.x = -0.37f;
	worldTransforms_[6].translation_.y = 0.25f;

	// ジェット
	worldTransforms_[7].translation_.x = -0.1f;
	worldTransforms_[7].translation_.y = -0.05f;

	// 銃
	worldTransforms_[8].translation_.y = -1.72f;
	worldTransforms_[8].rotation_.z = -1.59f;

	// ミサイルランチャー
	worldTransforms_[9].translation_.y = 0.81f;
	worldTransforms_[9].translation_.z = -0.54f;

	bulletEmitter.translation_ = { 0.54f,0.17f,1.4f };

	// ペアレント設定
	worldTransforms_[1].parent_ = &worldTransforms_[0];
	worldTransforms_[2].parent_ = &worldTransforms_[0];
	worldTransforms_[3].parent_ = &worldTransforms_[0];
	worldTransforms_[4].parent_ = &worldTransforms_[0];
	worldTransforms_[5].parent_ = &worldTransforms_[0];
	worldTransforms_[6].parent_ = &worldTransforms_[0];
	worldTransforms_[7].parent_ = &worldTransforms_[6];
	worldTransforms_[8].parent_ = &worldTransforms_[2];
	worldTransforms_[9].parent_ = &worldTransforms_[0];
	bulletEmitter.parent_ = &worldTransforms_[8];
}

void Player::Update() {
	// デスフラグの立った弾を削除
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
		});
	// キャラクターの移動ベクトル
	Vector3 move = { 0, 0, 0 };

	// キャラクターの移動速さ
	const float kCharacterSpeed = 0.08f;

	// 弾丸の更新
	for (PlayerBullet* bullet : bullets_) {

		bullet->Update();
	}
	Attack();
	/*/////////////////////////////
	/// キーボードによる移動処理
	/////////////////////////////*/

#ifdef _DEBUG


	// 押した方向で移動ベクトルを変更(左右)
	if (input_->GetInstance()->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;
	} else if (input_->GetInstance()->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
	}


	// 押した方向で移動ベクトルを変更(上下)
	if (input_->GetInstance()->PushKey(DIK_DOWN)) {
		move.y -= kCharacterSpeed;
	} else if (input_->GetInstance()->PushKey(DIK_UP)) {
		move.y += kCharacterSpeed;
	}
#endif
	// 座標移動(ベクトルの加算)
	worldTransforms_[0].translation_ += move;

	bulletEmitter.translation_ += move;

	for (auto& transform : worldTransforms_) {
		transform.UpdateMatrix();
	}
	
#ifdef _DEBUG

	ImGui::Begin("Player");
	for (size_t i = 0; i < objects_.size(); ++i) {
		if (ImGui::TreeNode(("Model " + std::to_string(i)).c_str())) {
			ImGui::ColorEdit4("color", &objects_[i]->GetModel().GetMaterialDataAddress().color.x);
			ImGui::DragFloat3("translate", &worldTransforms_[i].translation_.x, 0.01f);
			ImGui::DragFloat3("rotate", &worldTransforms_[i].rotation_.x, 0.01f);
			ImGui::DragFloat3("scale", &worldTransforms_[i].scale_.x, 0.01f);
			ImGui::TreePop();
		}
	}
	ImGui::DragFloat3("bulletEmitter", &bulletEmitter.translation_.x, 0.01f);
	ImGui::InputFloat("fireCoolTime", &fireCoolTime);
	ImGui::End();

#endif // _DEBUG	

	
}

void Player::Draw(Camera* camera) {
	
	//model_->Draw(worldTransform_, camera);
	for (size_t i = 0; i < objects_.size(); ++i) {
		objects_[i]->Update(worldTransforms_[i], camera);
		objects_[i]->Draw();
	}

}

void Player::Attack()
{
}

void Player::OnCollision()
{
}

Vector3 Player::GetCenterPosition() const
{
	//ローカル座標でのオフセット
	const Vector3 offset = { 0.0f, 1.5f, 0.0f };
	Matrix4x4 matWorld = MakeAffineMatrix(worldTransforms_[0].scale_, worldTransforms_[0].rotation_, worldTransforms_[0].translation_);
	//ワールド座標に変換
	Vector3 worldPos = Transform(offset, matWorld);
	return worldPos;
}
