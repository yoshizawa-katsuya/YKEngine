#include "Boss.h"
#include "imgui/imgui.h"

void Boss::Initialize(const std::vector<BaseModel*>& models)
{
	input_ = Input::GetInstance();
	models_ = models;
	worldTransforms_.resize(models.size());
	for (auto& transform : worldTransforms_) {
		transform.Initialize();
		transform.UpdateMatrix();
	}
	worldTransforms_[0].rotation_ = { 0.0f,1.5f,0.0f };
	worldTransforms_[0].translation_ = { 0.0f,0.0f,65.0f }; //Connection
	worldTransforms_[1].parent_ = &worldTransforms_[0];     //BossBody
	worldTransforms_[2].parent_ = &worldTransforms_[0];     //BossHead
	worldTransforms_[3].parent_ = &worldTransforms_[2];     //BossGunR
	worldTransforms_[4].parent_ = &worldTransforms_[2];     //BossGunL
	worldTransforms_[5].parent_ = &worldTransforms_[1];     //TrackL
	worldTransforms_[6].parent_ = &worldTransforms_[1];     //TrackR

	bulletModel_ = std::make_unique<RigidModel>();
	
	bulletModel_->CreateModel("./resources/Boss/bullet", "bullet.obj");

}

void Boss::Update()
{

	for (auto& transform : worldTransforms_) {
		transform.UpdateMatrix();
	}

#ifdef _DEBUG

	ImGui::Begin("Boss");
	for (size_t i = 0; i < models_.size(); ++i) {
		if (ImGui::TreeNode(("Model" + std::to_string(i)).c_str())) {
			ImGui::ColorEdit4("color", &models_[i]->GetMaterialDataAddress().color.x);
			ImGui::DragFloat3("translate", &worldTransforms_[i].translation_.x, 0.01f);
			ImGui::DragFloat3("rotate", &worldTransforms_[i].rotation_.x, 0.01f);
			ImGui::DragFloat3("scale", &worldTransforms_[i].scale_.x, 0.01f);
			ImGui::TreePop();
		}
	}
	ImGui::End();

#endif // _DEBUG	

	GunAttack();
	for (auto it = bullets_.begin(); it != bullets_.end(); ) {
		(*it)->Update();

		

		++it; 
	}

}

void Boss::Draw(Camera* camera)
{


	for (size_t i = 0; i < models_.size(); ++i) {
		models_[i]->Draw(worldTransforms_[i], camera);
	}


	for (const auto& bullet : bullets_) {
		bullet->Draw(camera);
	}

}

void Boss::GunAttack()
{

	//if (bulletCount_ >= maxBulletCount_) {
	//	cooldownTimer_ = cooldownDuration_;
	//	bulletCount_ = 0;
	//	return;
	//}
	if (input_->TriggerKey(DIK_SPACE)) {

		const float kBulletSpeed = 0.01f;
		Vector3 velocity(kBulletSpeed, 0.0f, 0.0f);
		velocity = TransFormNormal(velocity, worldTransforms_[3].worldMatrix_);
		
		auto bullet = std::make_unique<BossBullet>();
		bullet->Initialize(bulletModel_.get(), velocity);

		bullet->SetPosition(worldTransforms_[3].translation_);

		bullets_.push_back(std::move(bullet));
	}

}

Vector3 Boss::TransFormNormal(const Vector3& v, const Matrix4x4& m)
{
	Vector3 result{
		v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0],
		v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1],
		v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2]

	};
	return result;
}
