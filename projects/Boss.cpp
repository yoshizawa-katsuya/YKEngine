#include "Boss.h"
#include "imgui/imgui.h"
#include "PlayerLockOn.h"
#include "Vector.h"
#include "Rigid3dObject.h"
#include "Matrix.h"

void Boss::Initialize(const std::vector<BaseModel*>& models)
{

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

	worldTransforms_[0].rotation_ = { 0.0f,1.5f,0.0f };
	worldTransforms_[0].translation_ = { 0.0f,2.52f,65.0f }; //Connection
	worldTransforms_[1].translation_ = { 0.0f,0.0f,0.0f };
	worldTransforms_[2].translation_ = { 0.0f,0.0f,0.0f };
	worldTransforms_[3].translation_ = { 0.0f,0.0f,0.0f };
	worldTransforms_[4].translation_ = { 0.0f,0.0f,0.0f };
	worldTransforms_[5].translation_ = { 0.0f,0.0f,0.0f };
	worldTransforms_[6].translation_ = { 0.0f,0.0f,0.0f };

	worldTransforms_[1].parent_ = &worldTransforms_[0];     //BossBody
	worldTransforms_[2].parent_ = &worldTransforms_[0];     //BossHead
	worldTransforms_[3].parent_ = &worldTransforms_[2];     //BossGunR
	worldTransforms_[4].parent_ = &worldTransforms_[2];     //BossGunL
	worldTransforms_[5].parent_ = &worldTransforms_[1];     //TrackL
	worldTransforms_[6].parent_ = &worldTransforms_[1];     //TrackR

	worldTransform_.Initialize();

	worldTransform_.translation_ = { 0.0f,0.0f,65.0f };
	worldTransform_.rotation_ = { 0.0f,1.5f,0.0f };

	worldTransform_.UpdateMatrix();

}

void Boss::Update()
{
	for (auto& transform : worldTransforms_) {
		transform.UpdateMatrix();
	}
	// ロックオン
	if (lockOn_ && lockOn_->ExistTarget()) {
		// ロックオン座標
		Vector3 lockOnPosition = lockOn_->GetTargetPosition();
		// 追従対象からロックオン対象へのベクトル
		Vector3 sub = lockOnPosition - worldTransform_.translation_;

		// Y軸周り角度
		worldTransform_.rotation_.y = std::atan2(sub.x, sub.z);
	}

	worldTransform_.UpdateMatrix();

#ifdef _DEBUG

	ImGui::Begin("Boss");
		for (size_t i = 0; i < objects_.size(); ++i) {
			if (ImGui::TreeNode(("Model " + std::to_string(i)).c_str())) {
				ImGui::ColorEdit4("color", &objects_[i]->GetModel().GetMaterialDataAddress().color.x);
				ImGui::DragFloat3("translate", &worldTransforms_[i].translation_.x, 0.01f);
				ImGui::DragFloat3("rotate", &worldTransforms_[i].rotation_.x, 0.01f);
				ImGui::DragFloat3("scale", &worldTransforms_[i].scale_.x, 0.01f);
				ImGui::TreePop();
			}
		}
	ImGui::End();

#endif // _DEBUG	

}

void Boss::Draw(Camera* camera)
{

	for (size_t i = 0; i < objects_.size(); ++i) {
		objects_[i]->Update(worldTransforms_[i], camera);
		objects_[i]->Draw();
	}

	for (const auto& canon : canons_) {
		canon->Draw();
	}

}

void Boss::Attack(Camera* camera, BaseModel* model)
{
	// デスフラグが立った大砲を削除
	canons_.remove_if([](const std::unique_ptr<BossCanon>& canon) {return canon->IsDead(); });

	// 弾の速度
	const float kCanonSpeed = -1.0f;
	// ボスの座標を取得
	Vector3 bossTranslate = GetWorldPosition();
	// 自キャラの座標を取得
	Vector3 playerTranslate = player_->GetWorldPosition();
	// ボスから自キャラへの差分ベクトルを求める
	Vector3 diff = bossTranslate - playerTranslate;
	// ベクトルの正規化
	diff = Normalize(diff);
	// ベクトルの長さを速さに合わせる
	diff = diff * kCanonSpeed;

	Vector3 velocity = diff;

	// 速度ベクトルを自機の向きに合わせて回転させる
	Matrix4x4 matWorld = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	velocity = TransformNormal(velocity, matWorld);

	// 3秒間隔で砲撃
	float deltaTime = 1.0f / 60.0f;
	coolTime_ -= deltaTime;

	if (coolTime_ <= 0.0f) {
		auto newCanon = std::make_unique<BossCanon>();
		newCanon->Initialize(model, this, velocity);
		newCanon->SetPlayer(player_);

		// 登録
		canons_.push_back(std::move(newCanon));

		// タイマーを戻す
		coolTime_ = 3.0f;
	}

	for (const auto& canon : canons_) {
		canon->Update(camera);
	}

}

Vector3 Boss::GetWorldPosition()
{
	// ワールド座標を入れる変数
	Vector3 worldPos = {};
	Matrix4x4 matWorld = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = matWorld.m[3][0];
	worldPos.y = matWorld.m[3][1];
	worldPos.z = matWorld.m[3][2];

	return worldPos;
}

void Boss::OnCollision()
{
}

Vector3 Boss::TransformNormal(const Vector3& v, const Matrix4x4& m)
{
	Vector3 result{
		v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0],
		v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1],
		v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2],
	};
	return result;
}
