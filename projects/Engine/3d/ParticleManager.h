#pragma once
#include "SrvHeapManager.h"
#include "PrimitiveDrawer.h"
#include "Struct.h"
#include "Camera.h"
#include <random>
#include "ParticleTypes.h"
#include "BaseModel.h"

class ParticleManager
{
public:

	//シングルトンインスタンスの取得
	static ParticleManager* GetInstance();

	//終了
	void Finalize();

	void Initialize(DirectXCommon* dxCommon, SrvHeapManager* srvHeapManager, PrimitiveDrawer* primitiveDrawer);

	void Update(Camera* camera, AccelerationField* accelerationField = nullptr);

	void Draw();

	void CreateParticleGroup(const std::string name, uint32_t textureHandle, std::shared_ptr<BaseModel> model, bool useBillboard);

	//void Emit(const std::string name, const EulerTransform& transform, uint32_t count, bool isRandomColor);

	//void Emit(const std::string name, const EulerTransform& transform, uint32_t count, bool isRandomColor, const Vector4& color);

	void Emit(const std::string name, const EulerTransform& transform, uint32_t count, const ParticleRandomizationFlags& randomFlags,
		const Vector4& color, const EmitterRangeParams& rangeParams, const ParticleBehavior& behavior);

	void SetUseAccelerationField(bool useAccelerationField) { useAccelerationField_ = useAccelerationField; }

	bool& GetUseAccelerationField() { return useAccelerationField_; }
	bool GetUseAccelerationField() const { return useAccelerationField_; }

private:

	ParticleManager() = default;
	~ParticleManager() = default;
	ParticleManager(ParticleManager&) = default;
	ParticleManager& operator=(ParticleManager&) = default;

	Particle MakeNewParticle(const EulerTransform& transform, const ParticleRandomizationFlags& randomFlags,
		const Vector4& color, const EmitterRangeParams& rangeParams, const ParticleBehavior& behavior);


	struct ParticleGroup {
		std::shared_ptr<BaseModel> model;
		uint32_t textureHandle;
		bool useBillboard;
		std::list<Particle> particles;
		uint32_t instancingSrvIndex;
		Microsoft::WRL::ComPtr<ID3D12Resource> instancingResouce;
		const uint32_t kNumMaxInstance = 500;
		uint32_t numInstance;
		ParticleForGPU* instancingData;
	};

	DirectXCommon* dxCommon_;
	SrvHeapManager* srvHeapManager_;
	PrimitiveDrawer* primitiveDrawer_;

	const float kDeltaTime_ = 1.0f / 60.0f;


	//ランダムエンジン
	std::random_device seedGenerator_;
	std::mt19937 randomEngine_;

	std::unordered_map<std::string, ParticleGroup> particleGroups_;

	//AccelerationField accelerationField_;
	bool useAccelerationField_ = false;

};

