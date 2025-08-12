#include "Player.h"
#include "imgui/imgui.h"
#include "Rigid3dObject.h"
#include "WinApp.h"
#include "Matrix.h"
#include "Camera.h"
#include "Lerp.h"
#include "GameScene.h"
#include "ReticleController.h"
#include "Curve.h"
#include "TransformHelpers.h"

void Player::Initialize(BaseModel* model, Matrix4x4* viewPortMatrix, const std::vector<Vector3>& controlPoints) {

	BaseCharacter::Initialize(model);
	Collider::SetTypeID(CollisionTypeIdDef::kPlayer);

	input_ = Input::GetInstance();

	CreateSplineCurve(controlPoints);
	// 初期位置をスプラインの最初のポイントに設定
	eye_ = pointsDrawing_[0]; // 初期位置をスプラインの最初のポイントに設定


	//ワールドトランスフォームの初期設定
	basePointWorldTransform_.Initialize();
	basePointWorldTransform_.translation_ = eye_;

	worldTransform_.parent_ = &basePointWorldTransform_;
	//worldTransform_.translation_.z = 20.0f;

	//向きを更新する処理
	UpdateRotate();

	UpdateOffset();

	basePointWorldTransform_.UpdateMatrix();
	BaseCharacter::Update();

	reticleController_ = std::make_unique<ReticleController>();
	reticleController_->Initialize(viewPortMatrix);


}

void Player::Update(Camera* railCamera) {


#ifdef _DEBUG

	ImGui::Begin("Player");
	if (ImGui::TreeNode("Object")) {
		ImGui::ColorEdit4("color", &object_->GetModel().GetMaterialDataAddress().color.x);
		ImGui::DragFloat3("translate", &worldTransform_.translation_.x, 0.01f);
		ImGui::DragFloat3("rotate", &worldTransform_.rotation_.x, 0.01f);
		ImGui::DragFloat3("scale", &worldTransform_.scale_.x, 0.01f);

		ImGui::TreePop();
	}
	ImGui::DragInt("HP", &hitPoint_, 1.0f, 0, 100);
	ImGui::End();


#endif // _DEBUG	

	if (pointsDrawing_.size() > moveCount_) {
		//残りの移動距離計算用の変数
		float remainingMoveDistance = speed_;

		// カメラの位置を更新する処理
		while (remainingMoveDistance > 0.0f && pointsDrawing_.size() > moveCount_)
		{
			Vector3 moveDirection = pointsDrawing_[moveCount_] - eye_;
			float distance = Length(moveDirection);

			// ほぼゼロ距離の場合はスキップ
			if (distance < 0.001f) {
				moveCount_++;
				continue;
			}

			if (distance < remainingMoveDistance)
			{
				// 次のポイントに移動する距離が残りの距離よりも短い場合、次のポイントに移動
				eye_ = pointsDrawing_[moveCount_];
				remainingMoveDistance -= distance;
				moveCount_++;

			}
			else
			{
				// 残りの距離が次のポイントまでの距離よりも長い場合、次のポイントに向かって移動
				Vector3 normalizeDirection = Normalize(moveDirection);
				// 残りの距離を考慮してカメラの位置を更新
				eye_ += normalizeDirection * remainingMoveDistance;
				remainingMoveDistance = 0.0f;
			}
		}

		basePointWorldTransform_.translation_ = eye_;

		//向きを更新する処理
		UpdateRotate();

		UpdateOffset();

	}

	HandleMoveInput();

	//移動限界座標
	const float kMoveLimitX = 14;
	const float kMoveLimitY = 7.6f;

	//範囲を超えない処理
	worldTransform_.translation_.x = std::clamp(worldTransform_.translation_.x, -kMoveLimitX, kMoveLimitX);
	worldTransform_.translation_.y = std::clamp(worldTransform_.translation_.y, -kMoveLimitY, kMoveLimitY);

	basePointWorldTransform_.UpdateMatrix();
	BaseCharacter::Update();

	//照準オブジェクトの更新
	ReticleUpdate(railCamera);

	//キャラクター攻撃処理
	Attack();

	
}

void Player::OnCollision(Collider* other)
{
	if (other->GetTypeID() == CollisionTypeIdDef::kEnemyBullet)
	{
		hitPoint_--;

		if (hitPoint_ > 0) {
			return;
		}
		isDead_ = true;
	}
}

void Player::DrawUI()
{
	reticleController_->Draw();
}

void Player::DrawRail(Camera* camera)
{
	//線の描画
	for (size_t i = 0; i < pointsDrawing_.size() - 1; i++) {
		Matrix4x4 point1 = MakeTranslateMatrix(pointsDrawing_[i]);
		Matrix4x4 point2 = MakeTranslateMatrix(pointsDrawing_[i + 1]);
		ModelPlatform::GetInstance()->LineDraw(point1, point2, camera);
	}
}

void Player::SetLockOnTarget(const std::list<std::unique_ptr<Enemy>>& enemies, Camera* railCamera)
{
	reticleController_->SetLockOnTarget(enemies, railCamera);
}

Vector3 Player::GetWorldPosition() {

	return worldTransform_.GetWorldPosition();

}

void Player::HandleMoveInput()
{
	//キャラクターの移動ベクトル
	Vector3 move = { 0, 0, 0 };

	move.x = input_->GetLeftStickX();
	move.y = input_->GetLeftStickY();

	if (move.x == 0 && move.y == 0) {
		//左スティックがニュートラルなら、キーボード入力を確認
		//押した方向で移動ベクトルを変更(左右)
		if (input_->PushKey(DIK_A)) {
			move.x = -1.0f;
		}
		else if (input_->PushKey(DIK_D)) {
			move.x = 1.0f;
		}

		// 押した方向で移動ベクトルを変更(上下)
		if (input_->PushKey(DIK_S)) {
			move.y = -1.0f;
		}
		else if (input_->PushKey(DIK_W)) {
			move.y = 1.0f;
		}

		move = Normalize(move); //移動ベクトルの正規化

	}

	//キャラクターの移動速さ
	const float kCharacterSpeed = 0.2f;
	//移動ベクトルの速さの適用
	move *= kCharacterSpeed;

	//座標移動(ベクトルの加算)
	worldTransform_.translation_ += move;
}

void Player::ReticleUpdate(Camera* railCamera)
{
	reticleController_->Update(railCamera);
}

void Player::Attack() {

	if (input_->TriggerKey(DIK_SPACE) || input_->TriggerButton(XINPUT_GAMEPAD_RIGHT_SHOULDER)) {

		//弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		//自機から照準オブジェクトへのベクトル
		if (reticleController_->IsLockOn()) {
			velocity = Subtract(reticleController_->GetTargetPosition(), GetWorldPosition());

		}
		else {
			velocity = Subtract(reticleController_->Get3DReticlePosition(), GetWorldPosition());

		}
		velocity = Multiply(kBulletSpeed, Normalize(velocity));
		//速度ベクトルを自機の向きに合わせて回転させる
		//velocity = TransformNormal(velocity, worldTransform_.matWorld_);

		//弾を生成し、初期化
		gameScene_->AddPlayerbullet(GetWorldPosition(), velocity);

	}

}

void Player::CreateSplineCurve(const std::vector<Vector3>& controlPoints)
{
	// レベルデータから制御点を取得
	for (Vector3 controlPoint : controlPoints) {
		controlPoints_.push_back(controlPoint);
	}
	// Catmull-Romスプラインのポイントを生成
	pointsDrawing_ = GenerateCatmullRomSplinePoints(controlPoints_, segmentCount_);
}

void Player::UpdateRotate()
{
	//カメラの向きを更新する処理
	if (pointsDrawing_.size() > moveCount_ + difference_) {
		target_ = pointsDrawing_[moveCount_ + difference_];
		forward_ = Subtract(target_, basePointWorldTransform_.translation_);

		basePointWorldTransform_.rotation_ = TransformHelpers::FaceToVelocityDirection(basePointWorldTransform_.rotation_, forward_);

	}
}

void Player::UpdateOffset()
{
	//オフセットを更新する処理
	basePointWorldTransform_.UpdateMatrix();

	offset_ = { 0.0f, 3.0f, 0.0f };
	offset_ = TransformNormal(offset_, basePointWorldTransform_.worldMatrix_);
	basePointWorldTransform_.translation_ += offset_;
}