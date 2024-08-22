#include "ParticleManager.h"
#include "TextureManager.h"
#include "Matrix.h"
#include "Vector.h"
#include <numbers>
#include "imgui/imgui.h"

ParticleManager* ParticleManager::instance_ = nullptr;

ParticleManager* ParticleManager::GetInstance()
{
	if (instance_ == nullptr) {
		instance_ = new ParticleManager;
	}
	return instance_;
}

void ParticleManager::Finalize()
{
	delete instance_;
	instance_ = nullptr;
}

void ParticleManager::Initialize(DirectXCommon* dxCommon, SrvHeapManager* srvHeapManager, PrimitiveDrawer* primitiveDrawer)
{

	dxCommon_ = dxCommon;
	srvHeapManager_ = srvHeapManager;
	primitiveDrawer_ = primitiveDrawer;

	std::mt19937 randomEngine(seedGenerator_());
	randomEngine_ = randomEngine;

	Create();

	accelerationField_.accerelation = { 15.0f, 0.0f, 0.0f };
	accelerationField_.area.min = { -1.0f, -1.0f, -1.0f };
	accelerationField_.area.max = { 1.0f, 1.0f, 1.0f };

}

void ParticleManager::Update(Camera* camera)
{

	Matrix4x4 cameraMatrix = camera->GetWorldMatrix();
	Matrix4x4 viewprojectionMatrix = camera->GetViewProjection();
	Matrix4x4 backToFrontMatrix = MakeRotateYMatrix(std::numbers::pi_v<float>);
	Matrix4x4 billboardMatrix = Multiply(backToFrontMatrix, cameraMatrix);
	billboardMatrix.m[3][0] = 0.0f;	//平行移動成分はいらない
	billboardMatrix.m[3][1] = 0.0f;
	billboardMatrix.m[3][2] = 0.0f;
	if (!useBillboard_) {
		billboardMatrix = MakeIdentity4x4();
	}

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
			if (useAccelerationField_) {
				if (IsCollision(accelerationField_.area, particleIterator->transform.translate)) {
					particleIterator->velocity += accelerationField_.accerelation * kDeltaTime_;
				}
			}

			particleIterator->transform.translate += particleIterator->velocity * kDeltaTime_;
			particleIterator->currentTime += kDeltaTime_;	//経過時間を足す

			if (particleGroupIterator->second.numInstance < particleGroupIterator->second.kNumMaxInstance) {
				Matrix4x4 scaleMatrix = MakeScaleMatrix(particleIterator->transform.scale);
				Matrix4x4 translateMatrix = MakeTranslateMatrix(particleIterator->transform.translate);
				//Matrix4x4 worldMatrix = MakeAffineMatrix(particles_[index].transform.scale, particles_[index].transform.rotate, particles_[index].transform.translate);
				Matrix4x4 worldMatrix = scaleMatrix * billboardMatrix * translateMatrix;

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

	ImGui::Begin("ParticleManager");
	ImGui::Checkbox("useBillboard", &useBillboard_);
	ImGui::Checkbox("useAccelerationField", &useAccelerationField_);
	ImGui::End();
}

void ParticleManager::Draw()
{

	primitiveDrawer_->SetPipelineSet(dxCommon_->GetCommandList(), BlendMode::kBlendModeAddParticle);

	dxCommon_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	dxCommon_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);	//VBVを設定

	for (std::unordered_map<std::string, ParticleGroup>::iterator particleGroupIterator = particleGroups_.begin();
		particleGroupIterator != particleGroups_.end(); ++particleGroupIterator) {


		//SRVのDescriptorTableの先頭を設定。2はrootParameter[2]である。
		TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(particleGroupIterator->second.textureHandle);
		//マテリアルのCBufferの場所を設定
		dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
		
		//instancing用のDataを読むためにStructBufferのSRVを設定する
		srvHeapManager_->SetGraphicsRootDescriptorTable(1, particleGroupIterator->second.instancingSrvIndex);
		
		//描画!6頂点のポリゴンを、kNumInstance(今回は10)だけInstance描画を行う
		dxCommon_->GetCommandList()->DrawInstanced(6, particleGroupIterator->second.numInstance, 0, 0);


	}

}


void ParticleManager::CreateParticleGroup(const std::string name, uint32_t textureHandle)
{

	assert(!particleGroups_.contains(name));

	ParticleGroup& particleGroup = particleGroups_[name];

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

void ParticleManager::Emit(const std::string name, const Vector3& position, uint32_t count)
{

	assert(particleGroups_.contains(name));
	for (uint32_t i = 0; i < count; ++i) {
		particleGroups_[name].particles.push_back(MakeNewParticle(position));
	}

}

void ParticleManager::Create()
{

	
	//VertexResourceを生成
	vertexResource_ = dxCommon_->CreateBufferResource(sizeof(VertexData) * 6);
	
	//リソースの先頭のアドレスから使う
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点6つ分のサイズ
	vertexBufferView_.SizeInBytes = UINT(sizeof(VertexData) * 6);
	//1頂点当たりのサイズ
	vertexBufferView_.StrideInBytes = sizeof(VertexData);

	//書き込むためのアドレスを取得
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));

	vertexData_[0] = { .position = {1.0f, 1.0f, 0.0f, 1.0f}, .texcoord = {0.0f, 0.0f}, .normal = {0.0f, 0.0f, 1.0f} };	//左上
	vertexData_[1] = { .position = {-1.0f, 1.0f, 0.0f, 1.0f}, .texcoord = {1.0f, 0.0f}, .normal = {0.0f, 0.0f, 1.0f} };	//右上
	vertexData_[2] = { .position = {1.0f, -1.0f, 0.0f, 1.0f}, .texcoord = {0.0f, 1.0f}, .normal = {0.0f, 0.0f, 1.0f} };	//左下
	vertexData_[3] = { .position = {1.0f, -1.0f, 0.0f, 1.0f}, .texcoord = {0.0f, 1.0f}, .normal = {0.0f, 0.0f, 1.0f} };	//左下
	vertexData_[4] = { .position = {-1.0f, 1.0f, 0.0f, 1.0f}, .texcoord = {1.0f, 0.0f}, .normal = {0.0f, 0.0f, 1.0f} };	//右上
	vertexData_[5] = { .position = {-1.0f, -1.0f, 0.0f, 1.0f}, .texcoord = {1.0f, 1.0f}, .normal = {0.0f, 0.0f, 1.0f} };	//右下


	//マテリアル用のリソースを作る。今回はcolor1つ分のサイズを用意する
	materialResource_ = dxCommon_->CreateBufferResource(sizeof(Material));
	//マテリアルにデータを書き込む
	//書き込むためのアドレスを取得
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	//白を書き込む
	materialData_->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	materialData_->enableLighting = true;
	materialData_->uvTransform = MakeIdentity4x4();

}

Particle ParticleManager::MakeNewParticle(const Vector3& position)
{
	
	
	std::uniform_real_distribution<float> distribution(-1.0f, 1.0f);

	Particle particle;

	particle.transform.scale = { 1.0f, 1.0f, 1.0f };
	particle.transform.rotate = { 0.0f, 0.0f, 0.0f };
	//particle.transform.translate = { index * 0.1f, index * 0.1f, index * 0.1f };
	Vector3 randomTranslate{ distribution(randomEngine_), distribution(randomEngine_), distribution(randomEngine_) };
	particle.transform.translate = position + randomTranslate;
	//particle.velocity = { 0.0f, 1.0f, 0.0f };
	particle.velocity = { distribution(randomEngine_), distribution(randomEngine_), distribution(randomEngine_) };

	std::uniform_real_distribution<float> distcolor(0.0f, 1.0f);
	particle.color = { distcolor(randomEngine_), distcolor(randomEngine_), distcolor(randomEngine_), 1.0f };

	std::uniform_real_distribution<float> distTime(1.0f, 3.0f);
	particle.lifeTime = distTime(randomEngine_);
	particle.currentTime = 0;

	return particle;

}
