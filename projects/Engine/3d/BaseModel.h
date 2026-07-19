#pragma once
#include "WorldTransform.h"
#include "TextureManager.h"
#include <Windows.h>
#include <d3d12.h>
#include <wrl.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace YKEngine
{
class ModelPlatform;
class Camera;
class Animation;

/// <summary>
/// スキンモデルの頂点ウェイト情報。
/// </summary>
/// <param name="weight">ウェイト値</param>
/// <param name="vertexIndex">頂点インデックス</param>
struct VertexWeightData 
{
	float weight;
	uint32_t vertexIndex;
};

/// <summary>
/// スキンモデルのジョイントウェイト情報。
/// </summary>
/// <param name="inverseBindPoseMatrix">逆バインドポーズ行列</param>
/// <param name="vertexWeights">頂点ウェイト情報の配列</param>
struct JointWeightData 
{
	Matrix4x4 inverseBindPoseMatrix;
	std::vector<VertexWeightData> vertexWeights;
};

/// <summary>
/// マテリアル情報。
/// </summary>
/// <param name="textureFilePath">テクスチャファイルのパス</param>
struct MaterialData
{
	std::string textureFilePath;
};

/// <summary>
/// ノード情報。
/// </summary>
/// <param name="transform">ノードの変換情報</param>
/// <param name="localMatrix">ノードのローカル変換行列</param>
/// <param name="name">ノードの名前</param>
/// <param name="children">子ノードの配列</param>
struct Node
{
	QuaternionTransform transform;
	Matrix4x4 localMatrix;
	std::string name;
	std::vector<Node> children;
};

/// <summary>
/// モデルデータ。
/// </summary>
/// <param name="skinClusterData">スキンクラスター情報のマップ</param>
/// <param name="vertices">頂点データの配列</param>
/// <param name="indices">インデックスデータの配列</param>
/// <param name="material">マテリアル情報</param>
/// <param name="rootNode">ルートノード情報</param>
struct ModelData
{
	std::map<std::string, JointWeightData> skinClusterData;
	std::vector<VertexData> vertices;
	std::vector<uint32_t> indices;
	MaterialData material;
	Node rootNode;
};

/// <summary>
/// 3Dモデルの基底クラス。
/// ModelPlatformクラスを介して使用する。
/// 3dObjectクラスからも使用される。
/// モデルデータの読み込み、頂点・インデックス・マテリアルデータの作成、描画を行う。
/// 描画は3DObjectクラスから呼び出される。
/// </summary>
class BaseModel
{
public:

	BaseModel();

	virtual ~BaseModel() = 0;

	/// <summary>
	/// モデルデータの読み込みと頂点・インデックス・マテリアルデータの作成を行う。
	/// ModelPlatformクラス経由で呼び出す。
	/// </summary>
	/// <param name="directoryPath">モデルファイルのディレクトリパス</param>
	/// <param name="filename">モデルファイル名</param>
	/// <param name="color">マテリアルの色</param>
	virtual void CreateModel(const std::string& directoryPath, const std::string& filename, const Vector4& color = {1.0f, 1.0f, 1.0f, 1.0f});

	/// <summary>
	/// 球モデルの作成を行う。
	/// </summary>
	/// <param name="textureHandle">テクスチャハンドル</param>
	virtual void CreateSphere(uint32_t textureHandle);

	/// <summary>
	///	キューブモデルの作成を行う。
	/// </summary>
	/// <param name="textureHandle">テクスチャハンドル</param>
	virtual void CreateCube(uint32_t textureHandle);

	/// <summary>
	/// 平面モデルの作成を行う。
	/// </summary>
	/// <param name="textureHandle">テクスチャハンドル</param>
	virtual void CreatePlane(uint32_t textureHandle);

	/// <summary>
	/// 輪っかモデルの作成を行う。
	/// </summary>
	/// <param name="textureHandle">テクスチャハンドル</param>
	virtual void CreateRing(uint32_t textureHandle);

	/// <summary>
	/// 円柱モデルの作成を行う。
	/// </summary>
	/// <param name="textureHandle">テクスチャハンドル</param>
	virtual void CreateCylinder(uint32_t textureHandle);

	/// <summary>
	/// スカイボックスモデルの作成を行う。
	/// キューブテクスチャを使用する。
	/// </summary>
	/// <param name="textureHandle">テクスチャハンドル</param>
	virtual void CreateSkyBox(uint32_t textureHandle);

	/// <summary>
	/// モデルの描画を行う。
	/// 3DObjectクラス経由で呼び出す。
	/// </summary>
	/// <param name="usedMaterial">3DObjectのマテリアルを使用するかどうか</param>
	virtual void Draw(bool usedMaterial);

	/// <summary>
	/// モデルの描画を行う。(テクスチャハンドル指定版)
	/// 3DObjectクラス経由で呼び出す。
	/// </summary>
	/// <param name="textureHandle">テクスチャハンドル</param>
	/// <param name="usedMaterial">3DObjectのマテリアルを使用するかどうか</param>
	virtual void Draw(uint32_t textureHandle, bool usedMaterial);

	/// <summary>
	/// インスタンシング描画を行う。
	/// </summary>
	/// <param name="numInstance">インスタンス数</param>
	virtual void InstancingDraw(uint32_t numInstance);

	/// <summary>
	/// インスタンシング描画を行う。(テクスチャハンドル指定版)
	/// </summary>
	/// <param name="numInstance">インスタンス数</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	virtual void InstancingDraw(uint32_t numInstance, uint32_t textureHandle);

	/// <summary>
	/// 初期化完了まで待機する。
	/// </summary>
	void WaitUntilInitialized();

	virtual void SetSkinCluster(const SkinCluster& skinCluster);

	/// <summary>
	/// マテリアルの色設定。
	/// </summary>
	/// <param name="color">色</param>
	void SetColor(const Vector4& color);

	/// <summary>
	/// マテリアルのアルファ値設定。
	/// </summary>
	/// <param name="alpha">アルファ値</param>
	void SetAlpha(float alpha);

	//TODO:mtlファイルからuvTransform情報を読み込めるようにする
	virtual void SetUVTransform(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

	virtual void SetUVTransform(const EulerTransform& uvTransform);

	virtual void SetEnableLighting(bool enableLighting);

	void SetShininess(float shininess);

	virtual void SetEnvironmentCoefficient(float environmentCoefficient);

	const Material& GetMaterialData() const { return *materialData_; }

	const Node& GetRootNode() const { return modelData_->rootNode; }

	const ModelData& GetModelData() const { return *modelData_; }

	uint32_t GetVerticesNum() { return verticesNum_; }

	ModelPlatform* GetModelPlatform() { return modelPlatform_; }

protected:

	/// <summary>
	/// 頂点データ作成。
	/// </summary>
	void CreateVertexData();

	/// <summary>
	/// インデックスデータ作成。
	/// </summary>
	void CreateIndexData();

	/// <summary>
	/// マテリアルデータ作成。
	/// </summary>
	/// <param name="color">マテリアルの色</param>
	void CreateMaterialData(const Vector4& color = {1.0f, 1.0f, 1.0f, 1.0f});
	
	/// <summary>
	/// モデルファイルの読み込み。
	/// </summary>
	/// <param name="directoryPath">モデルファイルのディレクトリパス</param>
	/// <param name="filename">モデルファイル名</param>
	void LoadModelFile(const std::string& directoryPath, const std::string& filename);

	/// <summary>
	/// 頂点データ読み込み。
	/// </summary>
	/// <param name="mesh">Assimpのメッシュデータ</param>
	/// <param name="vertexStartIndex">頂点データの開始位置</param>
	void LoadVertexData(aiMesh* mesh, uint32_t vertexStartIndex);

	/// <summary>
	/// インデックスデータ読み込み。
	/// </summary>
	/// <param name="mesh">Assimpのメッシュデータ</param>
	/// <param name="vertexStartIndex">頂点データの開始位置</param>
	void LoadIndexData(aiMesh* mesh, uint32_t vertexStartIndex);

	/// <summary>
	/// メッシュデータ読み込み。
	/// </summary>
	/// <param name="mesh">Assimpのメッシュデータ</param>
	/// <param name="vertexStartIndex">頂点データの開始位置</param>
	virtual void LoadMeshData(aiMesh* mesh, uint32_t vertexStartIndex);

	/// <summary>
	/// 頂点数設定。
	/// </summary>
	void SetVerticesNum();

	/// <summary>
	/// インデックス数設定。
	/// </summary>
	void SetIndicesNum();

	/// <summary>
	/// ノード読み込み。
	/// </summary>
	/// <param name="node">Assimpのノードデータ</param>
	/// <returns>ノード情報</returns>
	Node ReadNode(aiNode* node);
	
	/// <summary>
	/// 描画共通処理。
	/// </summary>
	void DrawCommonProcess(bool usedMaterial, uint32_t textureHandle, uint32_t numInstance = 1);

	ModelPlatform* modelPlatform_ = nullptr;

	std::unique_ptr<ModelData> modelData_;

	//VertexResourceを生成
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource_;

	VertexData* vertexData_ = nullptr;
	//頂点バッファビューを作成する
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};

	//indexResouceを生成
	Microsoft::WRL::ComPtr<ID3D12Resource> indexResource_;
	uint32_t* indexData_ = nullptr;
	D3D12_INDEX_BUFFER_VIEW indexBufferView_{};

	//マテリアル用のリソースを作る
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_;
	//マテリアルにデータを書き込む
	Material* materialData_ = nullptr;

	uint32_t verticesNum_;
	uint32_t indicesNum_;
	uint32_t textureHandle_;

	//Initializeの完了フラグ
	bool initialized_ = false;

	//Initializeの完了を待つための条件変数とミューテックス
	std::mutex initMutex_;
	std::condition_variable initCondition_;

};

} // namespace YKEngine