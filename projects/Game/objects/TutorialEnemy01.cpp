#include "TutorialEnemy01.h"
#include "Rigid3dObject.h"

TutorialEnemy01::TutorialEnemy01()
{

}

TutorialEnemy01::~TutorialEnemy01()
{
}

void TutorialEnemy01::Update()
{
	Collider::Update();

	HPGaugeUpdate();
}

void TutorialEnemy01::SetHPGaugeModel(BaseModel* model, BaseModel* frameModel)
{
	hpGauge_.object_ = std::make_unique<Rigid3dObject>();
	hpGauge_.object_->Initialize(model);

	hpGauge_.worldTransform_.Initialize();
	hpGauge_.worldTransform_.translation_.y = worldTransform_.translation_.y - 0.2f;

	hpGauge_.darkObject_ = std::make_unique<Rigid3dObject>();
	hpGauge_.darkObject_->Initialize(model);

	hpGauge_.darkWorldTransform_.Initialize();
	hpGauge_.darkWorldTransform_.translation_.y = worldTransform_.translation_.y - 0.2f;
	hpGauge_.darkWorldTransform_.translation_.z = 0.001f;

	hpGauge_.frameObject_ = std::make_unique<Rigid3dObject>();
	hpGauge_.frameObject_->Initialize(frameModel);

	hpGauge_.frameWorldTransform_.Initialize();
	hpGauge_.frameWorldTransform_.translation_.y = worldTransform_.translation_.y - 0.2f;
}
