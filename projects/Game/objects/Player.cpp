#include "Player.h"
#include "imgui/imgui.h"
#include "Rigid3dObject.h"
#include "WinApp.h"
#include "Camera.h"
#include "Lerp.h"
#include "GameScene.h"
#include "ReticleController.h"
#include "TransformHelpers.h"

void Player::Initialize(BaseModel* model, Matrix4x4* viewPortMatrix, WorldTransform* parent, uint32_t heartTextureHandle, uint32_t heartEmptyTexturehandle) {

	BaseCharacter::Initialize(model);
	Collider::SetTypeID(CollisionTypeIdDef::kPlayer);

	input_ = Input::GetInstance();

	worldTransform_.parent_ = parent;
	//worldTransform_.translation_.z = 20.0f;
	
	BaseCharacter::Update();

	reticleController_ = std::make_unique<ReticleController>();
	reticleController_->Initialize(viewPortMatrix);

	HUDInitialize(heartTextureHandle, heartEmptyTexturehandle);

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

	
	HandleMoveInput();

	//移動限界座標
	const float kMoveLimitX = 14;
	const float kMoveLimitY = 7.6f;

	//範囲を超えない処理
	worldTransform_.translation_.x = std::clamp(worldTransform_.translation_.x, -kMoveLimitX, kMoveLimitX);
	worldTransform_.translation_.y = std::clamp(worldTransform_.translation_.y, -kMoveLimitY, kMoveLimitY);

	//回転
	Vector3 toPosition;
	if (reticleController_->IsLockOn())
	{
		toPosition = reticleController_->GetTargetPosition();
	}
	else
	{
		toPosition = reticleController_->Get3DReticlePosition();
	}
	direction_ = Subtract(toPosition, GetWorldPosition());
	Vector3 targetRotation = TransformHelpers::FaceToVelocityDirection(worldTransform_.rotation_, direction_);
	targetRotation -= worldTransform_.parent_->rotation_;
	worldTransform_.rotation_ = Lerp(worldTransform_.rotation_, targetRotation, 0.1f);

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

	//HPの表示
	for (int i = 0; i < maxHitPoint_; i++)
	{
		if (i < hitPoint_)
		{
			heratSprites_[i]->Draw();
		}
		else
		{
			heratEmptySprites_[i]->Draw();
		}
	}
}

void Player::SetLockOnTarget(const std::list<std::unique_ptr<Enemy>>& enemies, Camera* railCamera)
{
	reticleController_->SetLockOnTarget(enemies, railCamera);
}

Vector3 Player::GetWorldPosition() {

	return worldTransform_.GetWorldPosition();

}

void Player::HUDInitialize(uint32_t heartTextureHandle, uint32_t heartEmptyTexturehandle)
{
	heratSprites_.resize(maxHitPoint_);
	heratEmptySprites_.resize(maxHitPoint_);

	for (int i = 0; i < maxHitPoint_; i++)
	{
		heratSprites_[i] = std::make_unique<Sprite>();
		heratSprites_[i]->Initialize(heartTextureHandle);
		heratSprites_[i]->SetPosition(Vector2(50.0f + i * 50.0f, 50.0f)); //位置を設定
		heratSprites_[i]->SetSize(Vector2(50.0f, 50.0f)); //サイズを設定

		heratEmptySprites_[i] = std::make_unique<Sprite>();
		heratEmptySprites_[i]->Initialize(heartEmptyTexturehandle);
		heratEmptySprites_[i]->SetPosition(Vector2(50.0f + i * 50.0f, 50.0f)); //位置を設定
		heratEmptySprites_[i]->SetSize(Vector2(50.0f, 50.0f)); //サイズを設定
	}
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

		Vector3 velocity = Multiply(kBulletSpeed, Normalize(direction_));
		
		//弾を生成し、初期化
		gameScene_->AddPlayerbullet(GetWorldPosition(), velocity);

	}

}