#include "ParticleManager.h"
#include "TextureManager.h"
#include "Matrix.h"
#include <numbers>
#include "imgui/imgui.h"

ParticleManager* ParticleManager::GetInstance()
{
	static ParticleManager instance;
	return &instance;
}

void ParticleManager::Finalize()
{

}

void ParticleManager::Initialize(DirectXCommon* dxCommon, SrvHeapManager* srvHeapManager, PrimitiveDrawer* primitiveDrawer)
{

	dxCommon_ = dxCommon;
	srvHeapManager_ = srvHeapManager;
	primitiveDrawer_ = primitiveDrawer;

	std::mt19937 randomEngine(seedGenerator_());
	randomEngine_ = randomEngine;

	//Create();
	/*
	accelerationField_.accerelation = { 15.0f, 0.0f, 0.0f };
	accelerationField_.area.min = { -1.0f, -1.0f, -1.0f };
	accelerationField_.area.max = { 1.0f, 1.0f, 1.0f };
	*/
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

			particleIterator->transform.translation += particleIterator->velocity * kDeltaTime_;
			particleIterator->currentTime += kDeltaTime_;	//経過時間を足す

			if (particleGroupIterator->second.numInstance < particleGroupIterator->second.kNumMaxInstance) {
				Matrix4x4 scaleMatrix = MakeScaleMatrix(particleIterator->transform.scale);
				Matrix4x4 translateMatrix = MakeTranslateMatrix(particleIterator->transform.translation);
				Matrix4x4 rotateMatrix = MakeRotateMatrix(particleIterator->transform.rotation);
				//Matrix4x4 worldMatrix = MakeAffineMatrix(particles_[index].transform.scale, particles_[index].transform.rotate, particles_[index].transform.translate);
				Matrix4x4 worldMatrix;
				if (particleGroupIterator->second.useBillboard) 
				{
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

				float alpha = 1.0f - (particleIterator->currentTime / particleIterator->lifeTime);
				particleGroupIterator->second.instancingData[particleGroupIterator->second.numInstance].color.w = alpha;

				++particleGroupIterator->second.numInstance;
			}

			++particleIterator;

		}

	}

#ifdef _DEBUG

	ImGui::Begin("ParticleManager");
	ImGui::Checkbox("useAccelerationField", &useAccelerationField_);
	ImGui::End();

#endif // _DEBUG

}

void ParticleManager::Draw()
{

	primitiveDrawer_->SetPipelineSet(dxCommon_->GetCommandList(), BlendMode::kBlendModeAddParticle);

	dxCommon_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//dxCommon_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);	//VBVを設定

	//dxCommon_->GetCommandList()->IASetIndexBuffer(&indexBufferView_);

	//マテリアルのCBufferの場所を設定
	//dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());

	for (std::unordered_map<std::string, ParticleGroup>::iterator particleGroupIterator = particleGroups_.begin();
		particleGroupIterator != particleGroups_.end(); ++particleGroupIterator) {


		//SRVのDescriptorTableの先頭を設定。2はrootParameter[2]である。
		//TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(particleGroupIterator->second.textureHandle);
		
		//instancing用のDataを読むためにStructBufferのSRVを設定する
		srvHeapManager_->SetGraphicsRootDescriptorTable(1, particleGroupIterator->second.instancingSrvIndex);
		
		particleGroupIterator->second.model->InstancingDraw(particleGroupIterator->second.numInstance, particleGroupIterator->second.textureHandle);

		//描画!6頂点のポリゴンを、numInstanceだけInstance描画を行う
		//dxCommon_->GetCommandList()->DrawInstanced(6, particleGroupIterator->second.numInstance, 0, 0);
		//dxCommon_->GetCommandList()->DrawIndexedInstanced(6, particleGroupIterator->second.numInstance, 0, 0, 0);


	}

}


void ParticleManager::CreateParticleGroup(const std::string name, uint32_t textureHandle, std::shared_ptr<BaseModel> model, bool useBillboard)
{
	//名前とテクスチャが同じ場合パーティクルを使いまわす
	if (particleGroups_.contains(name))
	{
		assert(particleGroups_[name].textureHandle == textureHandle);

		particleGroups_[name].particles.clear();
		particleGroups_[name].numInstance = 0;
		return;
	}

	ParticleGroup& particleGroup = particleGroups_[name];

	particleGroup.model = model;
	particleGroup.useBillboard = useBillboard;
	particleGroup.textureHandle = textureHandle;

	particleGroup.instancingResouce = dxCommon_->CreateBufferResource(sizeof(ParticleForGPU) * particleGroup.kNumMaxInstance);
	particleGroup.instancingResouce->Map(0, nullptr, reinterpret_cast<void**>(&particleGroup.instancingData));

	//単位行列を書き込んでおく
	for (uint32_t index = 0; index < particleGroup.kNumMaxInstance; ++index) {
		particleGroup.instancingData[index].WVP = MakeIdentity4x4();
		particleGroup.instancingData[index].World = MakeIdentity4x4();
		particleGroup.instancingData[index].color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	}

	particleGroup.instancingSrvIndex = srvHeapManager_->Allocate();

	srvHeapManager_->CreateSRVforStructuredBuffer(particleGroup.instancingSrvIndex, particleGroup.instancingResouce.Get(), particleGroup.kNumMaxInstance, sizeof(ParticleForGPU));

}

void ParticleManager::Emit(const std::string name, const EulerTransform& transform, uint32_t count, const ParticleRandomizationFlags& randomFlags,
	const Vector4& color, const Vector3& translateMin, const Vector3& translateMax)
{
	assert(particleGroups_.contains(name));
	for (uint32_t i = 0; i < count; ++i) {
		particleGroups_[name].particles.push_back(MakeNewParticle(transform, randomFlags, color, translateMin, translateMax));
	}
}

Particle ParticleManager::MakeNewParticle(const EulerTransform& transform, const ParticleRandomizationFlags& randomFlags,
	const Vector4& color,const Vector3& translateMin, const Vector3& translateMax)
{

	Particle particle;

	if (randomFlags.scale) 
	{
		std::uniform_real_distribution<float> distScale(-0.6f, 0.5f);

		particle.transform.scale = transform.scale + Vector3{0.0f, distScale(randomEngine_), 0.0f};
	}
	else
	{
		particle.transform.scale = transform.scale;
	}
	if (randomFlags.rotate)
	{
		std::uniform_real_distribution<float> diatRotate(-std::numbers::pi_v<float>, std::numbers::pi_v<float>);

		particle.transform.rotation = { 0.0f, 0.0f, diatRotate(randomEngine_) };
	}
	else 
	{
		particle.transform.rotation = { 0.0f, 0.0f, 0.0f };
	}

	if (randomFlags.translate)
	{
		std::uniform_real_distribution<float> distributionX(translateMin.x, translateMax.x);
		std::uniform_real_distribution<float> distributionY(translateMin.y, translateMax.y);
		std::uniform_real_distribution<float> distributionZ(translateMin.z, translateMax.z);

		Vector3 randomTranslate{ distributionX(randomEngine_), distributionY(randomEngine_), distributionZ(randomEngine_) };
		particle.transform.translation = transform.translation + randomTranslate;
	}
	else
	{
		particle.transform.translation = transform.translation;
	}

	if (randomFlags.velocity)
	{
		std::uniform_real_distribution<float> distribution(-1.0f, 1.0f);

		particle.velocity = { distribution(randomEngine_), distribution(randomEngine_), distribution(randomEngine_) };
	}
	else 
	{
		particle.velocity = { 0.0f, 0.0f, 0.0f };
	}

	if (randomFlags.color)
	{
		std::uniform_real_distribution<float> distcolor(0.0f, 1.0f);
		particle.color = { distcolor(randomEngine_), distcolor(randomEngine_), distcolor(randomEngine_), 1.0f };
	}
	else 
	{
		particle.color = color;
	}

	if (randomFlags.lifeTime)
	{
		std::uniform_real_distribution<float> distTime(1.0f, 3.0f);
		particle.lifeTime = distTime(randomEngine_);
	}
	else
	{
		particle.lifeTime = 1.0f;
	}
	
	particle.currentTime = 0.0f;

	return particle;

}
