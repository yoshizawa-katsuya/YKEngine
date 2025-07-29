#include "ReticleController.h"
#include "Input.h"
#include <algorithm>
#include "Camera.h"
#include "Matrix.h"
#include "Enemy.h"
#include "Collision.h"

void ReticleController::Initialize(Matrix4x4* viewPortMatrix)
{
	//3Dレティクルのワールドトランスフォーム初期化
	worldTransform3DReticle_.Initialize();

	//レティクル用テクスチャ取得
	uint32_t textureLargeReticle = TextureManager::GetInstance()->Load("./Resources/largeReticle.png");
	uint32_t textureSmallReticle = TextureManager::GetInstance()->Load("./Resources/smallReticle.png");

	//スプライト生成
	spriteLargeReticle_ = std::make_unique<Sprite>();
	spriteLargeReticle_->Initialize(textureLargeReticle);
	spriteLargeReticle_->SetPosition({ static_cast<float>(WinApp::kClientWidth) / 2.0f , static_cast<float>(WinApp::kClientHeight) / 2.0f });
	spriteLargeReticle_->SetAnchorPoint({ 0.5f, 0.5f });

	spriteSmallReticle_ = std::make_unique<Sprite>();
	spriteSmallReticle_->Initialize(textureSmallReticle);
	spriteSmallReticle_->SetPosition({ static_cast<float>(WinApp::kClientWidth) / 2.0f , static_cast<float>(WinApp::kClientHeight) / 2.0f });
	spriteSmallReticle_->SetAnchorPoint({ 0.5f, 0.5f });

	viewPortMatrix_ = viewPortMatrix;

	input_ = Input::GetInstance();
}

void ReticleController::Update(Camera* railCamera)
{
	spriteSmallReticle_->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });

	//スプライトの現在座標を取得
	Vector2 spritePosition = spriteLargeReticle_->GetPosition();

	Vector3 move = { 0, 0, 0 };
	const float kReticleSpeed = 12.0f;

	//押した方向で移動ベクトルを変更(左右)
	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kReticleSpeed;
	}
	else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kReticleSpeed;
	}

	// 押した方向で移動ベクトルを変更(上下)
	if (input_->PushKey(DIK_DOWN)) {
		move.y += kReticleSpeed;
	}
	else if (input_->PushKey(DIK_UP)) {
		move.y -= kReticleSpeed;
	}

	spritePosition.x += move.x;
	spritePosition.y += move.y;

	//範囲を超えない処理
	spritePosition.x = std::clamp(spritePosition.x, 0.0f, static_cast<float>(WinApp::kClientWidth));
	spritePosition.y = std::clamp(spritePosition.y, 0.0f, static_cast<float>(WinApp::kClientHeight));

	//スプライトの座標変更を反映
	spriteLargeReticle_->SetPosition(spritePosition);
	spriteSmallReticle_->SetPosition(spritePosition);

	//ビュー行列とプロジェクション行列、ビューポート行列を合成する
	Matrix4x4 matViewProjectionViewport = Multiply(railCamera->GetViewProjection(), *viewPortMatrix_);

	//合成行列の逆行列の計算をする
	Matrix4x4 matInveraseVPV = Inverse(matViewProjectionViewport);

	//スクリーン座標
	Vector3 posNear = Vector3(spritePosition.x, spritePosition.y, 0);
	Vector3 posFar = Vector3(spritePosition.x, spritePosition.y, 1);

	//スクリーン座標系からワールド座標系へ
	posNear = Transform(posNear, matInveraseVPV);
	posFar = Transform(posFar, matInveraseVPV);

	//マウスレイの方向
	Vector3 mouseDirection = Subtract(posFar, posNear);
	mouseDirection = Normalize(mouseDirection);
	//カメラから照準オブジェクトへの距離
	const float kDistanceTestObject = 50.0f;
	worldTransform3DReticle_.translation_ = Add(posNear, Multiply(kDistanceTestObject, mouseDirection));
	worldTransform3DReticle_.UpdateMatrix();
}

void ReticleController::Draw()
{
	spriteLargeReticle_->Draw();
	spriteSmallReticle_->Draw();
}

void ReticleController::SetLockOnTarget(const std::list<std::unique_ptr<Enemy>>& enemies, Camera* railCamera)
{
	Vector2 ScreenPosA = spriteLargeReticle_->GetPosition();
	Vector2 SizeA = spriteLargeReticle_->GetSize();

	/*player_->SetIsLockOn(false);*/
	isLockOn_ = false;
	float closestDistance = (std::numeric_limits<float>::max)();
	Vector2 targetScreenPosition = { 0.0f, 0.0f };
	Vector3 targetWorldPosition = { 0.0f, 0.0f , 0.0f};

	for (const std::unique_ptr<Enemy>& enemy : enemies) {
		Vector2 ScreenPosB = { enemy->GetScreenPosition(railCamera).x, enemy->GetScreenPosition(railCamera).y };

		if (IsCollision(Square(ScreenPosA - SizeA / 2.0f, ScreenPosA + SizeA / 2.0f), ScreenPosB)) {
			//一番近い敵を探す
			float distance = Length(ScreenPosA - ScreenPosB);

			if (closestDistance < distance) {
				continue; // 既に近い敵がいるのでスキップ
			}

			closestDistance = distance;
			targetScreenPosition = ScreenPosB;
			targetWorldPosition = enemy->GetWorldPosition();
			isLockOn_ = true;
			/*player_->LockOn(ScreenPosB, enemy->GetWorldPosition());*/
		}

	}

	if (isLockOn_)
	{
		LockOn(targetScreenPosition, targetWorldPosition);
	}
}

void ReticleController::LockOn(const Vector2& position, const Vector3& targetPosition)
{
	spriteSmallReticle_->SetPosition(position);
	spriteSmallReticle_->SetColor({ 1.0f, 0.0f, 0.0f, 1.0f });

	target_ = targetPosition;
}
