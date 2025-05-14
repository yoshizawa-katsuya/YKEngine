#include "Player.h"
#include "imgui/imgui.h"
#include "Rigid3dObject.h"
#include "WinApp.h"
#include "Matrix.h"
#include "Camera.h"
#include "Lerp.h"
#include "GameScene.h"

void Player::Initialize(BaseModel* model) {

	BaseCharacter::Initialize(model);

	input_ = Input::GetInstance();

	worldTransform_.translation_.z = 20.0f;

	//3Dレティクルのワールドトランスフォーム初期化
	worldTransform3DReticle_.Initialize();

	//レティクル用テクスチャ取得
	uint32_t textureReticle = TextureManager::GetInstance()->Load("./Resources/reticle.png");

	//スプライト生成
	sprite2DReticle_ = std::make_unique<Sprite>();
	sprite2DReticle_->Initialize(textureReticle);
	sprite2DReticle_->SetPosition({ static_cast<float>(WinApp::kClientWidth) / 2.0f , static_cast<float>(WinApp::kClientHeight) / 2.0f });
	sprite2DReticle_->SetAnchorPoint({ 0.5f, 0.5f });

}

void Player::Update(Camera* railCamera) {


#ifdef _DEBUG

	ImGui::Begin("Player");
	if (ImGui::TreeNode("Model")) {
		ImGui::ColorEdit4("color", &object_->GetModel().GetMaterialDataAddress().color.x);
		ImGui::DragFloat3("translate", &worldTransform_.translation_.x, 0.01f);
		ImGui::DragFloat3("rotate", &worldTransform_.rotation_.x, 0.01f);
		ImGui::DragFloat3("scale", &worldTransform_.scale_.x, 0.01f);

		ImGui::TreePop();
	}
	ImGui::End();


#endif // _DEBUG	

	Rotate();

	//キャラクターの移動ベクトル
	Vector3 move = { 0, 0, 0 };

	//キャラクターの移動速さ
	const float kCharacterSpeed = 0.2f;

	//押した方向で移動ベクトルを変更(左右)
	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;
	}
	else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
	}

	// 押した方向で移動ベクトルを変更(上下)
	if (input_->PushKey(DIK_DOWN)) {
		move.y -= kCharacterSpeed;
	}
	else if (input_->PushKey(DIK_UP)) {
		move.y += kCharacterSpeed;
	}

	//座標移動(ベクトルの加算)
	worldTransform_.translation_ = Add(worldTransform_.translation_, move);

	//移動限界座標
	const float kMoveLimitX = 34;
	const float kMoveLimitY = 18;

	//範囲を超えない処理
	worldTransform_.translation_.x = (std::max)(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = (std::min)(worldTransform_.translation_.x, kMoveLimitX);
	worldTransform_.translation_.y = (std::max)(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = (std::min)(worldTransform_.translation_.y, kMoveLimitY);

	BaseCharacter::Update();

	//自機のワールド座標から3Dレティクルのワールド座標を計算
	{
		//自機から3Dレティクルへの距離
		const float kDistancePlayerTo3DReticle = 50.0f;
		//自機から3Dレティクルへのオフセット(Z+向き)
		Vector3 offset = { 0, 0, 1.0f };
		//自機のワールド行列の回転を反映
		offset = TransformNormal(offset, worldTransform_.worldMatrix_);
		//ベクトルの長さを整える
		offset = Multiply(kDistancePlayerTo3DReticle, Normalize(offset));
		//3Dレティクルの座標を設定
		worldTransform3DReticle_.translation_ = Add(GetWorldPosition(), offset);
		worldTransform3DReticle_.UpdateMatrix();

	}

	//3Dレティクルのワールド座標から2Dレティクルのスクリーン座標を計算
	{
		Vector3 positionReticle = worldTransform3DReticle_.GetWorldPosition();

		//ビューポート行列
		Matrix4x4 matViewport = MakeViewportMatrix(0, 0, WinApp::kClientWidth, WinApp::kClientHeight, 0, 1);

		//ビュー行列とプロジェクション行列、ビューポート行列を合成する
		Matrix4x4 matViewProjectionViewport = Multiply(railCamera->GetViewProjection(), matViewport);

		//ワールド→スクリーン座標変換(ここで3Dから2Dになる)
		positionReticle = Transform(positionReticle, matViewProjectionViewport);

		//スプライトのレティクルに座標設定
		positionReticle = Lerp({ sprite2DReticle_->GetPosition().x, sprite2DReticle_->GetPosition().y, 0.0f }, positionReticle, 0.2f);
		sprite2DReticle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));
		sprite2DReticle_->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
	}

	//キャラクター攻撃処理
	Attack();

	
}

void Player::Rotate() {



	//回転速さ
	const float kRotSpeed = 0.02f;

	//押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_A)) {
		worldTransform_.rotation_.y -= kRotSpeed;
	}
	else if (input_->PushKey(DIK_D)) {
		worldTransform_.rotation_.y += kRotSpeed;
	}

}

void Player::Attack() {

	if (input_->TriggerKey(DIK_SPACE)) {

		//弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		//自機から照準オブジェクトへのベクトル
		if (isLockOn_) {
			velocity = Subtract(target_, GetWorldPosition());

		}
		else {
			velocity = Subtract(worldTransform3DReticle_.translation_, GetWorldPosition());

		}
		velocity = Multiply(kBulletSpeed, Normalize(velocity));
		//速度ベクトルを自機の向きに合わせて回転させる
		//velocity = TransformNormal(velocity, worldTransform_.matWorld_);

		//弾を生成し、初期化
		gameScene_->AddPlayerbullet(GetWorldPosition(), velocity);

	}



}

void Player::OnCollision() {

}

void Player::DrawUI() {

	sprite2DReticle_->Draw();

}

void Player::LockOn(const Vector2& position, const Vector3& targetPosition) {

	isLockOn_ = true;
	sprite2DReticle_->SetPosition(position);
	sprite2DReticle_->SetColor({ 1.0f, 0.0f, 0.0f, 1.0f });

	target_ = targetPosition;
}

void Player::SetParent(WorldTransform* parent) {

	//親子関係を結ぶ
	worldTransform_.parent_ = parent;

}

Vector3 Player::GetWorldPosition() {

	return worldTransform_.GetWorldPosition();;

}