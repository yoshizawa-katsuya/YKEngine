#pragma once
#include "SrvHeapManager.h"
#include "PrimitiveDrawer.h"
#include "Struct.h"
#include "Camera.h"
#include <random>

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

	void CreateParticleGroup(const std::string name, uint32_t textureHandle);

	//void Emit(const std::string name, const EulerTransform& transform, uint32_t count, bool isRandomColor);

	//void Emit(const std::string name, const EulerTransform& transform, uint32_t count, bool isRandomColor, const Vector4& color);

	void Emit(const std::string name, const EulerTransform& transform, uint32_t count, bool isRandomColor, 
		bool isRandomTranslate, bool isRandomVelocity, bool isRandomRotate, bool isRandomScale,
		const Vector4& color,const Vector3& translateMin, const Vector3& translateMax);

	void SetUseAccelerationField(bool useAccelerationField) { useAccelerationField_ = useAccelerationField; }

	bool& GetUseAccelerationField() { return useAccelerationField_; }
	bool GetUseAccelerationField() const { return useAccelerationField_; }

private:

	ParticleManager() = default;
	~ParticleManager() = default;
	ParticleManager(ParticleManager&) = default;
	ParticleManager& operator=(ParticleManager&) = default;

	void Create();

	Particle MakeNewParticle(const EulerTransform& transform, bool isRandomColor, bool isRandomTranslate,
		bool isRandomVelocity, bool isRandomRotate, bool isRandomScale, const Vector4& color, 
		const Vector3& translateMin = { -1.0f, -1.0f, -1.0f }, const Vector3& translateMax = { 1.0f, 1.0f, 1.0f });


	struct ParticleGroup {
		uint32_t textureHandle;
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

	VertexData* vertexData_;

	//VertexResourceを生成
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource_;
	//頂点バッファビューを作成する
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};

	//マテリアル用のリソースを作る
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_;
	//マテリアルにデータを書き込む
	Material* materialData_ = nullptr;

	std::unordered_map<std::string, ParticleGroup> particleGroups_;

	bool useBillboard_ = true;

	//AccelerationField accelerationField_;
	bool useAccelerationField_ = false;

};

