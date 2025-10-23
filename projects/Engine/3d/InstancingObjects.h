#pragma once
#include "BaseModel.h"

/// <summary>
/// 3Dモデルのインスタンシング描画を行うクラス。
/// 複数の同じモデルを描画する際に使用する。
/// マップチップや草など、同じモデルを大量に描画する際に有効。
/// </summary>
class InstancingObjects
{
public:

	/// <summary>
	/// コンストラクタ。
	/// </summary>
	InstancingObjects();
	
	/// <summary>
	/// デストラクタ。
	/// </summary>
	~InstancingObjects();

	/// <summary>
	/// 初期化。
	/// </summary>
	/// <param name="model">描画するモデル</param>
	/// <param name="maxInstances">最大インスタンス数</param>
	void Initialize(BaseModel* model, uint32_t maxInstances);

	/// <summary>
	/// 更新前処理。
	/// インスタンス数のリセットを行う。
	/// </summary>
	void PreUpdate();

	/// <summary>
	/// ワールド変換行列の更新。
	/// </summary>
	/// <param name="worldTransform">ワールド変換行列</param>
	void WorldTransformUpdate(const WorldTransform& worldTransform);

	//void AddWorldTransform(const WorldTransform& worldTransform);

	/// <summary>
	/// カメラ情報の更新。
	/// </summary>
	/// <param name="camera">オブジェクトを映すカメラ</param>
	void CameraUpdate(Camera* camera);

	/// <summary>
	/// 描画処理。
	/// </summary>
	void Draw();

private:

	DirectXCommon* dxCommon_;
	SrvHeapManager* srvHeapManager_;

	//TransformationMatrix用のリソースを作る。モデル用
	Microsoft::WRL::ComPtr<ID3D12Resource> instancingResouce_;
	//データを書き込む
	//std::vector<TransformationMatrix*> instanceDatas_;
	TransformationMatrix* instancingData_;
	//Matrix4x4* WVPData_ = nullptr;
	uint32_t kNumMaxInstance_;
	uint32_t numInstance_;
	uint32_t instancingSrvIndex_;


	BaseModel* model_;
};

