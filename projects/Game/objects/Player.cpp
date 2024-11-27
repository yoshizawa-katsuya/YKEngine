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

	audio_ = Audio::GetInstance();

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
	// HPのセット
	playerHP = playerMaxHP;
	/// <summary>
	/// 体のパーツの座標設定
	/// </summary>

	// 体及び全体
	worldTransforms_[0].scale_ = { 0.8f,0.8f,0.8f };
	worldTransforms_[0].rotation_.y = -1.56f;
	worldTransforms_[0].translation_.y = 1.7f;

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

	bulletEmitter.translation_ = { 0.55f,2.02f,2.46f };

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


	hitSE01_ = audio_->SoundLoadWave("./resources/Sound/SE_01.wav");
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

	if (playerHP <= 0) {
		isDead_ = true;
	}
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
	// 移動限界座標
	const float kMoveLimitX = 3.6f;
	// 範囲を超えない処理
	worldTransforms_[0].translation_.x = std::clamp(worldTransforms_[0].translation_.x, -kMoveLimitX, +kMoveLimitX);
	worldTransforms_[0].translation_.y = std::clamp(worldTransforms_[0].translation_.y, 1.7f, 3.4f);

	bulletEmitter.translation_.x = std::clamp(bulletEmitter.translation_.x, -kMoveLimitX + 0.55f, +kMoveLimitX + 0.55f);
	bulletEmitter.translation_.y = std::clamp(bulletEmitter.translation_.y, 2.02f, 3.72f);
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
	int intPlayerHP = static_cast<int>(playerHP);
	ImGui::InputInt("playerHP", &intPlayerHP);
	playerHP = static_cast<uint32_t>(intPlayerHP);
	ImGui::End();

#endif // _DEBUG	

	
}

void Player::Draw(Camera* camera) {
	
	//model_->Draw(worldTransform_, camera);
	for (size_t i = 0; i < objects_.size(); ++i) {
		objects_[i]->Update(worldTransforms_[i], camera);
		objects_[i]->Draw();
	}
	// 弾描画
	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(camera);
	}
}

void Player::Attack()
{
	fireCoolTime++;
	if (input_->GetInstance()->PushKey(DIK_SPACE) && fireCoolTime >= kCoolDownTime) {
		fireCoolTime = 0;
		// 弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		audio_->SoundPlayWave(hitSE01_);

		// 速度ベクトルを自機の向きに合わせて回転させる
		//velocity = mathMatrix_->TransformNormal(velocity, worldTransform_.matWorld_);

		// 弾を生成し、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(bulletModel.get(), bulletEmitter.translation_, velocity);

		// 弾を登録する
		bullets_.push_back(newBullet);
	}
}

void Player::OnCollision()
{
	playerHP -= 1;
}

Vector3 Player::GetCenterPosition() const
{
	//ローカル座標でのオフセット
	const Vector3 offset = { 0.0f, 1.5f, 0.0f };
	
	//ワールド座標に変換
	Vector3 worldPos = Transform(offset, worldTransforms_[0].worldMatrix_);
	return worldPos;
}

Vector3 Player::GetWorldPosition()
{
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得
	worldPos.x = worldTransforms_[0].worldMatrix_.m[3][0];
	worldPos.y = worldTransforms_[0].worldMatrix_.m[3][1];
	worldPos.z = worldTransforms_[0].worldMatrix_.m[3][2];

	return worldPos;
}
