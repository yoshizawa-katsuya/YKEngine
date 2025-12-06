#pragma once
#include "SrvHeapManager.h"
#include "PrimitiveDrawer.h"
#include "Struct.h"
#include "Camera.h"
#include "ParticleTypes.h"
#include "BaseModel.h"
#include "Color.h"

namespace YKEngine
{
class Random;

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

	/// <summary>
	/// シングルトンのインスタンスを取得。
	/// </summary>
	/// <returns>シングルトンインスタンス</returns>
	static ParticleManager* GetInstance();

	/// <summary>
	/// 終了処理。
	/// </summary>
	void Finalize();

	/// <summary>
	/// 初期化。
	/// </summary>
	/// <param name="dxCommon">DirectX共通クラス</param>
	/// <param name="srvHeapManager">SRVヒープマネージャー</param>
	/// <param name="primitiveDrawer">プリミティブ描画クラス</param>
	void Initialize(DirectXCommon* dxCommon, SrvHeapManager* srvHeapManager, PrimitiveDrawer* primitiveDrawer);

	/// <summary>
	/// パーティクルの更新。
	/// </summary>
	/// <param name="camera">カメラ</param>
	/// <param name="accelerationField">加速度フィールド</param>
	void Update(Camera* camera, AccelerationField* accelerationField = nullptr);

	/// <summary>
	/// パーティクルの描画。
	/// </summary>
	void Draw();

	/// <summary>
	/// パーティクルグループの作成。
	/// </summary>
	/// <param name="name">パーティクルグループの名前</param>
	/// <param name="textureHandle">パーティクルのテクスチャハンドル</param>
	/// <param name="model">パーティクルのモデル</param>
	/// <param name="behavior">パーティクルの挙動</param>
	void CreateParticleGroup(const std::string name, uint32_t textureHandle, std::shared_ptr<BaseModel> model, std::shared_ptr<ParticleBehavior> behavior);

	/// <summary>
	/// パーティクルの発生。
	/// </summary>
	/// <param name="name">パーティクルグループの名前</param>
	/// <param name="transform">エミッターの座標変換情報</param>
	/// <param name="count">発生させるパーティクルの数</param>
	/// <param name="randomFlags">ランダム化フラグ</param>
	/// <param name="color">パーティクルの色</param>
	/// <param name="rangeParams">エミッター範囲パラメータ</param>
	void Emit(const std::string name, const EulerTransform& transform, uint32_t count, const ParticleRandomizationFlags& randomFlags,
		const Color& color, const EmitterRangeParams& rangeParams);

	/// <summary>
	/// パーティクルを削除。
	/// </summary>
	/// <param name="name">パーティクルグループの名前</param>
	void ClearParticles(const std::string name);

	/// <summary>
	/// 全てのパーティクルを削除。
	/// </summary>
	void ClearAllParticles();

	void SetUseAccelerationField(bool useAccelerationField) { useAccelerationField_ = useAccelerationField; }

	bool& GetUseAccelerationField() { return useAccelerationField_; }
	bool GetUseAccelerationField() const { return useAccelerationField_; }

private:

	// シングルトンインスタンス
	static ParticleManager* instance_;

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
	struct ParticleGroup 
	{
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

	//パーティクル用の描画モード配列
	std::vector<DrawMode> particleDrawModes_ = {
		DrawMode::kBlendModeAddParticle,
		DrawMode::kBlendModeNormalParticle,
		DrawMode::kBlendModeAddBackDrawParticle,
		DrawMode::kBlendModeNormalBackDrawParticle
	};

	//乱数生成エンジンへのポインタ
	Random* random_ = nullptr;

	std::unordered_map<std::string, ParticleGroup> particleGroups_;

	bool useAccelerationField_ = false;

	
};

} // namespace YKEngine