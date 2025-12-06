#include "ParticleManager.h"
#include "TextureManager.h"
#include "Matrix.h"
#include <numbers>
#include "TransformHelpers.h"
#include "Lerp.h"
#include "Random.h"
#include "RootParams.h"

#ifdef USE_IMGUI
#include "imgui/imgui.h"
#endif // USE_IMGUI

using namespace YKEngine;

ParticleManager* ParticleManager::instance_ = nullptr;

ParticleManager* ParticleManager::GetInstance()
{
	if (instance_ == nullptr)
	{
		instance_ = new ParticleManager();
	}
	return instance_;
}

void ParticleManager::Finalize()
{
	//インスタンスを破棄
	delete instance_;
	instance_ = nullptr;
}

void ParticleManager::Initialize(DirectXCommon* dxCommon, SrvHeapManager* srvHeapManager, PrimitiveDrawer* primitiveDrawer)
{

	dxCommon_ = dxCommon;
	srvHeapManager_ = srvHeapManager;
	primitiveDrawer_ = primitiveDrawer;

	random_ = Random::GetInstance();
}

void ParticleManager::Update(Camera* camera, AccelerationField* accelerationField)
{

	Matrix4x4 cameraMatrix = camera->GetWorldMatrix();
	Matrix4x4 viewprojectionMatrix = camera->GetViewProjection();
	Matrix4x4 backToFrontMatrix = MakeRotateYMatrix(std::numbers::pi_v<float>);
	Matrix4x4 billboardMatrix = Multiply(backToFrontMatrix, cameraMatrix);
	billboardMatrix.m[3][0] = 0.0f;	//平行移動成分はいらない
	billboardMatrix.m[3][1] = 0.0f;
	billboardMatrix.m[3][2] = 0.0f;

	for (std::unordered_map<std::string, ParticleGroup>::iterator particleGroupIterator = particleGroups_.begin();
		particleGroupIterator != particleGroups_.end(); ++particleGroupIterator) {

		particleGroupIterator->second.numInstance = 0;

		for (std::list<Particle>::iterator particleIterator = particleGroupIterator->second.particles.begin();
			particleIterator != particleGroupIterator->second.particles.end(); ) {

			if (particleIterator->lifeTime <= particleIterator->currentTime) {
				particleIterator = particleGroupIterator->second.particles.erase(particleIterator);	//生存時間が過ぎたParticleはlistから消す。戻り値が次のイテレータとなる
				continue;
			}

			//Fieldの範囲内のParticleには加速度を適用する
			if (useAccelerationField_ && accelerationField) {
				if (IsCollision(accelerationField->area, particleIterator->transform.translation)) {
					particleIterator->velocity += accelerationField->accerelation * kDeltaTime_;
				}
			}

			particleIterator->currentTime += kDeltaTime_;	//経過時間を足す
			if (particleGroupIterator->second.behavior->isdownVelocity)
			{
				particleIterator->transform.translation += Lerp(particleIterator->velocity, Vector3{ 0.0f, 0.0f, 0.0f }, particleIterator->currentTime / particleIterator->lifeTime) * kDeltaTime_;
			}
			else 
			{
				//速度をそのまま適用する
				particleIterator->transform.translation += particleIterator->velocity * kDeltaTime_;
			}
			
			if (particleGroupIterator->second.numInstance < particleGroupIterator->second.kNumMaxInstance) {
				Matrix4x4 scaleMatrix{};
				if (particleGroupIterator->second.behavior->isScaleToDisappear) 
				{
					//時間経過で小さくなる
					float t = ApplyEasing(particleGroupIterator->second.behavior->easingTypeForScale, particleIterator->currentTime / particleIterator->lifeTime);
					scaleMatrix = MakeScaleMatrix(Lerp(particleIterator->transform.scale, Vector3{ 0.0f, 0.0f, 0.0f}, t));
				}
				else if (particleGroupIterator->second.behavior->isScaleToAppear)
				{
					//時間経過で大きくなる
					float t = ApplyEasing(particleGroupIterator->second.behavior->easingTypeForScale, particleIterator->currentTime / particleIterator->lifeTime);
					scaleMatrix = MakeScaleMatrix(Lerp(Vector3{ 0.0f, 0.0f, 0.0f }, particleIterator->transform.scale, t));
				}
				else
				{
					scaleMatrix = MakeScaleMatrix(particleIterator->transform.scale);
				}
				Matrix4x4 translateMatrix = MakeTranslateMatrix(particleIterator->transform.translation);

				//回転を更新
				particleIterator->transform.rotation += particleIterator->rotationVelocity;
				Matrix4x4 rotateMatrix = MakeRotateMatrix(particleIterator->transform.rotation);
				Matrix4x4 worldMatrix;
				if (particleGroupIterator->second.behavior->isUseBillboard) 
				{
					//カメラの方向を向く
					worldMatrix = scaleMatrix * rotateMatrix * billboardMatrix * translateMatrix;
				}
				else
				{
					worldMatrix = scaleMatrix * rotateMatrix * translateMatrix;
				}

				Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, viewprojectionMatrix);
				particleGroupIterator->second.instancingData[particleGroupIterator->second.numInstance].WVP = worldViewProjectionMatrix;
				particleGroupIterator->second.instancingData[particleGroupIterator->second.numInstance].World = worldMatrix;
				particleGroupIterator->second.instancingData[particleGroupIterator->second.numInstance].color = particleIterator->color;

				if (particleGroupIterator->second.behavior->isTimeFadeOut)
				{
					//時間経過で透明になる
					float alpha = 1.0f - (particleIterator->currentTime / particleIterator->lifeTime);
					particleGroupIterator->second.instancingData[particleGroupIterator->second.numInstance].color.a = alpha;
				}
				
				++particleGroupIterator->second.numInstance;
			}

			++particleIterator;

		}

	}

#ifdef USE_IMGUI

	ImGui::Begin("ParticleManager");
	ImGui::Checkbox("useAccelerationField", &useAccelerationField_);
	ImGui::End();

#endif // USE_IMGUI

}

void ParticleManager::Draw()
{

	dxCommon_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	for (std::unordered_map<std::string, ParticleGroup>::iterator particleGroupIterator = particleGroups_.begin();
		particleGroupIterator != particleGroups_.end(); ++particleGroupIterator) {

		primitiveDrawer_->SetPipelineSet(dxCommon_->GetCommandList(), particleDrawModes_[static_cast<int32_t>(particleGroupIterator->second.behavior->drawMode)]);

		
		//instancing用のDataを読むためにStructBufferのSRVを設定する
		srvHeapManager_->SetGraphicsRootDescriptorTable(static_cast<size_t>(ParticleRootParam::kParticleForGPU), particleGroupIterator->second.instancingSrvIndex);
		
		particleGroupIterator->second.model->InstancingDraw(particleGroupIterator->second.numInstance, particleGroupIterator->second.textureHandle);

	}

}


void ParticleManager::CreateParticleGroup(const std::string name, uint32_t textureHandle, std::shared_ptr<BaseModel> model, std::shared_ptr<ParticleBehavior> behavior)
{
	//名前とテクスチャが同じ場合パーティクルを使いまわす
	if (particleGroups_.contains(name))
	{
		assert(particleGroups_[name].textureHandle == textureHandle);

		particleGroups_[name].particles.clear();
		particleGroups_[name].numInstance = 0;
		particleGroups_[name].behavior = behavior;
		return;
	}

	ParticleGroup& particleGroup = particleGroups_[name];

	particleGroup.model = model;
	particleGroup.behavior = behavior;
	particleGroup.textureHandle = textureHandle;

	particleGroup.instancingResouce = dxCommon_->CreateBufferResource(sizeof(ParticleForGPU) * particleGroup.kNumMaxInstance);
	particleGroup.instancingResouce->Map(0, nullptr, reinterpret_cast<void**>(&particleGroup.instancingData));

	//単位行列を書き込んでおく
	for (uint32_t index = 0; index < particleGroup.kNumMaxInstance; ++index)
	{
		particleGroup.instancingData[index].WVP = MakeIdentity4x4();
		particleGroup.instancingData[index].World = MakeIdentity4x4();
		particleGroup.instancingData[index].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	}

	particleGroup.instancingSrvIndex = srvHeapManager_->Allocate();

	srvHeapManager_->CreateSRVforStructuredBuffer(particleGroup.instancingSrvIndex, particleGroup.instancingResouce.Get(), particleGroup.kNumMaxInstance, sizeof(ParticleForGPU));

}

void ParticleManager::Emit(const std::string name, const EulerTransform& transform, uint32_t count, const ParticleRandomizationFlags& randomFlags,
	const Color& color, const EmitterRangeParams& rangeParams)
{
	assert(particleGroups_.contains(name));
	ParticleGroup& particleGroup = particleGroups_[name];
	for (uint32_t i = 0; i < count; ++i) 
	{
		particleGroup.particles.push_back(MakeNewParticle(transform, randomFlags, color, rangeParams, *particleGroup.behavior.get()));
	}
}

void ParticleManager::ClearParticles(const std::string name)
{
	assert(particleGroups_.contains(name));
	ParticleGroup& particleGroup = particleGroups_[name];
	particleGroup.particles.clear();
	particleGroup.numInstance = 0;
}

void ParticleManager::ClearAllParticles()
{
	for (std::unordered_map<std::string, ParticleGroup>::iterator particleGroupIterator = particleGroups_.begin();
		particleGroupIterator != particleGroups_.end(); ++particleGroupIterator) 
	{
		particleGroupIterator->second.particles.clear();
		particleGroupIterator->second.numInstance = 0;
	}
}

Particle ParticleManager::MakeNewParticle(const EulerTransform& transform, const ParticleRandomizationFlags& randomFlags,
	const Color& color, const EmitterRangeParams& rangeParams, const ParticleBehavior& behavior)
{

	Particle particle{};

	//生存時間
	if (randomFlags.lifeTime)
	{
		//ランダムな生存時間を設定
		particle.lifeTime = random_->GetFloat(rangeParams.lifeTime.min, rangeParams.lifeTime.max);
	}
	else
	{
		particle.lifeTime = 1.0f;
	}

	if (randomFlags.velocity)
	{
		particle.velocity = random_->GetVector3(rangeParams.velocity.min, rangeParams.velocity.max);

		if (behavior.isConstantVelocity) 
		{
			particle.velocity = Normalize(particle.velocity) * behavior.speed;
		}
	}
	else
	{
		particle.velocity = { 0.0f, 0.0f, 0.0f };
	}

	if (randomFlags.speed)
	{
		particle.velocity = Normalize(particle.velocity) * random_->GetFloat(rangeParams.speed.min, rangeParams.speed.max);
	}

	if (randomFlags.scale) 
	{
		particle.transform.scale = transform.scale + random_->GetVector3(rangeParams.scale.min, rangeParams.scale.max);
	}
	else
	{
		particle.transform.scale = transform.scale;
	}
	if (randomFlags.rotate)
	{
		particle.transform.rotation = transform.rotation + random_->GetVector3(rangeParams.rotate.min, rangeParams.rotate.max);
	}
	else if (behavior.isFaceToVelocityDirection)
	{
		//進行方向を向く
		particle.transform.rotation = TransformHelpers::FaceToVelocityDirection(particle.transform.rotation, particle.velocity);
	}
	else 
	{
		particle.transform.rotation = transform.rotation;
	}

	if (randomFlags.rotationVelocity)
	{
		particle.rotationVelocity = random_->GetVector3(rangeParams.rotationVelocity.min, rangeParams.rotationVelocity.max);
	}

	if (randomFlags.translate)
	{
		particle.transform.translation = transform.translation + random_->GetVector3(rangeParams.translate.min, rangeParams.translate.max);
	}
	else
	{
		particle.transform.translation = transform.translation;
	}

	if (behavior.isfixedDistance)
	{
		//発生位置から一定距離離す
		particle.transform.translation = Normalize(particle.transform.translation - transform.translation) * behavior.distance + transform.translation;
	}

	if (behavior.isHeadCenter)
	{
		//発生位置から中心に向かう速度を設定
		particle.velocity = (transform.translation - particle.transform.translation) / particle.lifeTime;
	}

	if (randomFlags.color)
	{
		particle.color = { random_->GetFloat(0.0f, 1.0f), random_->GetFloat(0.0f, 1.0f), random_->GetFloat(0.0f, 1.0f), 1.0f};
	}
	else 
	{
		particle.color = color;
	}
	
	particle.currentTime = 0.0f;

	return particle;

}
