#pragma once
#include "DirectXCommon.h"
#include "PipelineManager.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include <array>
#include "SrvHeapManager.h"
#include "BaseModel.h"

namespace YKEngine
{
class Camera;

/// <summary>
/// モデルの基盤クラス。
/// モデルの生成、管理、描画前処理などを行う。
/// シングルトン。
/// </summary>
class ModelPlatform
{
public:

	/// <summary>
	/// シングルトンインスタンスの取得。
	/// </summary>
	/// <returns>シングルトンインスタンス</returns>
	static ModelPlatform* GetInstance();

	/// <summary>
	/// 終了処理。
	/// </summary>
	void Finalize();

	/// <summary>
	/// 初期化。
	/// </summary>
	/// <param name="dxCommon">DirectX共通クラス</param>
	/// <param name="primitiveDrawer">プリミティブ描画クラス</param>
	/// <param name="srvHeapManager">SRVヒープマネージャー</param>
	void Initialize(DirectXCommon* dxCommon, PipelineManager* primitiveDrawer, SrvHeapManager* srvHeapManager);

	/// <summary>
	/// フレーム終了処理。
	/// </summary>
	void EndFrame();

	/// <summary>
	/// 描画前処理。
	/// </summary>
	void PreDraw();

	/// <summary>
	/// スカイボックス描画前処理。
	/// </summary>
	void SkyBoxPreDraw();

	/// <summary>
	/// スキンモデル描画前処理。
	/// </summary>
	void SkinPreDraw();

	/// <summary>
	/// インスタンシング描画前処理。
	/// </summary>
	void InstancingPreDraw();

	/// <summary>
	/// インスタンシング描画前処理(トリプラナーマッピング用)。
	/// </summary>
	void InstancingTriplanarPreDraw();

	/// <summary>
	/// 線分描画前処理。
	/// </summary>
	void LinePreDraw();

	/// <summary>
	/// 線分描画。
	/// </summary>
	/// <param name="worldMatrix1">線分の始点のワールド行列</param>
	/// <param name="worldMatrix2">線分の終点のワールド行列</param>
	/// <param name="camera">カメラ</param>
	void LineDraw(const Matrix4x4& worldMatrix1, const Matrix4x4& worldMatrix2, Camera* camera);

	/// <summary>
	/// 球(ワイヤーフレーム)描画前処理。
	/// </summary>
	void SpherePreDraw();

	/// <summary>
	/// 球(ワイヤーフレーム)描画。
	/// </summary>
	/// <param name="worldMatrix">球のワールド行列</param>
	/// <param name="camera">カメラ</param>
	void SphereDraw(const Matrix4x4& worldMatrix, Camera* camera);

	/// <summary>
	/// リジッドモデル生成。
	/// </summary>
	/// <param name="directoryPath">モデルファイルのディレクトリパス</param>
	/// <param name="filename">モデルファイル名</param>
	/// <param name="color">モデルの色</param>
	/// <returns>リジッドモデル</returns>
	std::shared_ptr<BaseModel> CreateRigidModel(const std::string& directoryPath, const std::string& filename, const Vector4& color = { 1.0f, 1.0f, 1.0f, 1.0f });

	/// <summary>
	/// スキンモデル生成。
	/// </summary>
	/// <param name="directoryPath">モデルファイルのディレクトリパス</param>
	/// <param name="filename">モデルファイル名</param>
	/// <param name="color">モデルの色</param>
	/// <returns>スキンモデル</returns>
	std::shared_ptr<BaseModel> CreateSkinModel(const std::string& directoryPath, const std::string& filename, const Vector4& color = { 1.0f, 1.0f, 1.0f, 1.0f });

	/// <summary>
	/// 球モデル生成。
	/// </summary>
	/// <param name="textureHandle">テクスチャハンドル</param>
	/// <param name="modelName">モデル名</param>
	/// <returns>球モデル</returns>
	std::shared_ptr<BaseModel> CreateSphere(uint32_t textureHandle, const std::string& modelName = "");

	/// <summary>
	/// 立方体モデル生成。
	/// </summary>
	/// <param name="textureHandle">テクスチャハンドル</param>
	/// <param name="modelName">モデル名</param>
	/// <returns>立方体モデル</returns>
	std::shared_ptr<BaseModel> CreateCube(uint32_t textureHandle, const std::string& modelName = "");

	/// <summary>
	/// 平面モデル生成。
	/// </summary>
	/// <param name="textureHandle">テクスチャハンドル</param>
	/// <param name="modelName">モデル名</param>
	/// <returns>平面モデル</returns>
	std::shared_ptr<BaseModel> CreatePlane(uint32_t textureHandle, const std::string& modelName = "");

	/// <summary>
	/// リングモデル生成。
	/// </summary>
	/// <param name="textureHandle">テクスチャハンドル</param>
	/// <param name="modelName">モデル名</param>
	/// <returns>リングモデル</returns>
	std::shared_ptr<BaseModel> CreateRing(uint32_t textureHandle, const std::string& modelName = "");

	/// <summary>
	/// 円柱モデル生成。
	/// </summary>
	/// <param name="textureHandle">テクスチャハンドル</param>
	/// <param name="modelName">モデル名</param>
	/// <returns>円柱モデル</returns>
	std::shared_ptr<BaseModel> CreateCylinder(uint32_t textureHandle, const std::string& modelName = "");

	/// <summary>
	/// スカイボックスモデル生成。
	/// キューブマップテクスチャを使用する。
	/// </summary>
	/// <param name="textureHandle">テクスチャハンドル</param>
	/// <param name="modelName">モデル名</param>
	/// <returns>スカイボックスモデル</returns>
	std::shared_ptr<BaseModel> CreateSkyBox(uint32_t textureHandle, const std::string& modelName = "");

	DirectXCommon* GetDxCommon() const { return dxCommon_; }

	SrvHeapManager* GetSrvHeapManager() const { return srvHeapManager_; }

	/// <summary>
	/// ライト描画前処理。
	/// ライトカウントをリセットする。
	/// </summary>
	void LightPreUpdate();

	/// <summary>
	/// 平行光源更新。
	/// </summary>
	/// <param name="directionalLight">平行光源データ</param>
	void DirectionalLightUpdate(const DirectionalLight& directionalLight);

	/// <summary>
	/// 点光源更新。
	/// </summary>
	/// <param name="pointLight">点光源データ</param>
	void PointLightUpdate(const PointLight& pointLight);

	/// <summary>
	/// スポット光源更新。
	/// </summary>
	/// <param name="spotLight">スポット光源データ</param>
	void SpotLightUpdate(const SpotLight& spotLight);

	void SetCamera(Camera* camera) { camera_ = camera; }

	void SetDrawMode(DrawMode drawMode);

	//コンストラクタに渡すための鍵
	class ConstructorKey {
	private:
		ConstructorKey() = default;
		friend class ModelPlatform;
	};

	//PassKeyを受け取るコンストラクタ
	explicit ModelPlatform(ConstructorKey key) {}

private:

	/// <summary>
	/// 線分のワールド・ビュー・プロジェクション行列構造体。
	/// </summary>
	struct LineWVP
	{
		Matrix4x4 WVP1;
		Matrix4x4 WVP2;
	};

	/// <summary>
	/// 光源数構造体。
	/// </summary>
	/// <param name="directional">平行光源の数</param>
	/// <param name="point">点光源の数</param>
	/// <param name="spot">スポット光源の数</param>
	struct LightCount
	{
		uint32_t directional;	// 平行光源の数
		uint32_t point;			// 点光源の数
		uint32_t spot;			// スポット光源の数
		float padding;			// パディング（16バイトアラインメントのため）
	};

	/// <summary>
	/// 描画前処理共通。
	/// </summary>
	void CommonPreDraw(bool isSkin);

	template<class T, class F>
	std::shared_ptr<BaseModel> CreateModelCommon(const std::string& name, F createFunc);
	

	// シングルトンインスタンス
	static std::unique_ptr<ModelPlatform> instance_;
	friend struct std::default_delete<ModelPlatform>;

	~ModelPlatform() = default;
	ModelPlatform(ModelPlatform&) = delete;
	ModelPlatform& operator=(ModelPlatform&) = delete;

	DirectXCommon* dxCommon_;

	SrvHeapManager* srvHeapManager_;

	PipelineManager* primitiveDrawer_;

	std::unordered_map<std::string, std::shared_ptr<BaseModel>> models_;

	LightCount* lightCount_;
	Microsoft::WRL::ComPtr<ID3D12Resource> lightCountResource_;

	Microsoft::WRL::ComPtr<ID3D12Resource> directionalLightResouce_;
	DirectionalLight* directionalLightDatas_ = nullptr;
	uint32_t kNumMaxDirectionalLight_ = 100;
	uint32_t directionalLightSrvIndex_;

	Microsoft::WRL::ComPtr<ID3D12Resource> pointLightResouce_;
	PointLight* pointLightDatas_ = nullptr;
	uint32_t kNumMaxPointLight_ = 100;
	uint32_t pointLightSrvIndex_;

	Microsoft::WRL::ComPtr<ID3D12Resource> spotLightResouce_;
	SpotLight* spotLightDatas_ = nullptr;
	uint32_t kNumMaxSpotLight_ = 100;
	uint32_t spotLightSrvIndex_;


	Camera* camera_;

	//VertexResourceを生成
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource_;

	Vector4* vertexData_ = nullptr;
	//頂点バッファビューを作成する
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};

	//デバッグ用の球と線分のリソース数。
	static const uint32_t kResourceNum_ = 1;

	//TransformationMatrix用のリソースを作る 線分用
	std::array<Microsoft::WRL::ComPtr<ID3D12Resource>, kResourceNum_> LineWVPResources_;
	//データを書き込む
	std::array<LineWVP*, kResourceNum_> LineWVPDatas_;

	uint32_t lineIndex_ = 0;

	//TransformationMatrix用のリソースを作る。球用
	std::array<Microsoft::WRL::ComPtr<ID3D12Resource>, kResourceNum_> SphereWVPResources_;
	//データを書き込む
	std::array<Matrix4x4*, kResourceNum_> SphereWVPDatas_;

	uint32_t sphereIndex_ = 0;
	
};

template<class T, class F>
inline std::shared_ptr<BaseModel> ModelPlatform::CreateModelCommon(const std::string& name, F createFunc)
{
	// すでに同じ名前のモデルがあればそれを返す
	if (models_.contains(name)) {
		return models_[name];
	}

	// モデル生成
	std::shared_ptr<T> model = std::make_shared<T>();

	// 生成処理（ラムダで受ける）
	createFunc(model.get());

	models_[name] = model;
	return model;
}

} // namespace YKEngine