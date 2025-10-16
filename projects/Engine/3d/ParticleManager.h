#pragma once
#include "SrvHeapManager.h"
#include "PrimitiveDrawer.h"
#include "Struct.h"
#include "Camera.h"
#include <random>
#include "ParticleTypes.h"
#include "BaseModel.h"
#include "Color.h"

/// <summary>
/// パーティクル管理クラス。
/// シングルトン。
/// パーティクルの生成、更新、描画を行う。
/// パーティクルはグループごとに管理される。
/// パーティクルの生成はエミッターから行う。
/// </summary>
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

	void CreateParticleGroup(const std::string name, uint32_t textureHandle, std::shared_ptr<BaseModel> model, std::shared_ptr<ParticleBehavior> behavior);

	void Emit(const std::string name, const EulerTransform& transform, uint32_t count, const ParticleRandomizationFlags& randomFlags,
		const Color& color, const EmitterRangeParams& rangeParams);

	void SetUseAccelerationField(bool useAccelerationField) { useAccelerationField_ = useAccelerationField; }

	bool& GetUseAccelerationField() { return useAccelerationField_; }
	bool GetUseAccelerationField() const { return useAccelerationField_; }

private:

	ParticleManager() = default;
	~ParticleManager() = default;
	ParticleManager(ParticleManager&) = default;
	ParticleManager& operator=(ParticleManager&) = default;

	Particle MakeNewParticle(const EulerTransform& transform, const ParticleRandomizationFlags& randomFlags,
		const Color& color, const EmitterRangeParams& rangeParams, const ParticleBehavior& behavior);

	/// <summary>
	/// パーティクルグループ
	/// </summary>
	/// <param name="model">パーティクルのモデル</param>
	/// <param name="textureHandle">パーティクルのテクスチャハンドル</param>
	/// <param name="particles">パーティクルのリスト</param>
	/// <param name="instancingSrvIndex">インスタンシング用SRVヒープのインデックス</param>
	/// <param name="instancingResouce">インスタンシング用リソース</param>
	/// <param name="kNumMaxInstance">インスタンスの最大数</param>
	/// <param name="numInstance">現在のインスタンス数</param>
	/// <param name="instancingData">インスタンシング用データ</param>
	/// <param name="behavior">パーティクルの挙動</param>
	struct ParticleGroup {
		std::shared_ptr<BaseModel> model;
		uint32_t textureHandle;
		std::list<Particle> particles;
		uint32_t instancingSrvIndex;
		Microsoft::WRL::ComPtr<ID3D12Resource> instancingResouce;
		const uint32_t kNumMaxInstance = 500;
		uint32_t numInstance;
		ParticleForGPU* instancingData;
		std::shared_ptr<ParticleBehavior> behavior;
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

