#pragma once
#include "WorldTransform.h"
#include "TextureManager.h"
#include <Windows.h>
#include <d3d12.h>
#include <wrl.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
class ModelPlatform;
class Camera;
class Animation;

/// <summary>
/// スキンモデルの頂点ウェイト情報。
/// </summary>
/// <param name="weight">ウェイト値</param>
/// <param name="vertexIndex">頂点インデックス</param>
struct VertexWeightData {
	float weight;
	uint32_t vertexIndex;
};

/// <summary>
/// スキンモデルのジョイントウェイト情報。
/// </summary>
/// <param name="inverseBindPoseMatrix">逆バインドポーズ行列</param>
/// <param name="vertexWeights">頂点ウェイト情報の配列</param>
struct JointWeightData {
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
struct Node {
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
/// <param name="indeces">インデックスデータの配列</param>
/// <param name="material">マテリアル情報</param>
/// <param name="rootNode">ルートノード情報</param>
struct ModelData {
	std::map<std::string, JointWeightData> skinClusterData;
	std::vector<VertexData> vertices;
	std::vector<uint32_t> indeces;
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

	//void Initialize(ModelPlatform* modelPlatform);

	//ModelPlatformから呼び出す
	virtual void CreateModel(const std::string& directoryPath, const std::string& filename, const Vector4& color = {1.0f, 1.0f, 1.0f, 1.0f});

	virtual void CreateSphere(uint32_t textureHandle);

	virtual void CreateCube(uint32_t textureHandle);

	virtual void CreatePlane(uint32_t textureHandle);

	virtual void CreateRing(uint32_t textureHandle);

	virtual void CreateCylinder(uint32_t textureHandle);

	virtual void CreateSkyBox(uint32_t textureHandle);

	//3dObjectクラス経由で呼び出す
	virtual void Draw(bool usedMaterial);

	//3dObjectクラス経由で呼び出す
	virtual void Draw(uint32_t textureHandle, bool usedMaterial);

	virtual void InstancingDraw(uint32_t numInstance);

	virtual void InstancingDraw(uint32_t numInstance, uint32_t textureHandle);

	virtual void SetSkinCluster(const SkinCluster& skinCluster);

	//TODO:mtlファイルからuvTransform情報を読み込めるようにする
	virtual void SetUVTransform(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

	virtual void SetUVTransform(const EulerTransform& uvTransform);

	virtual void SetEnableLighting(bool enableLighting);

	virtual void SetEnvironmentCoefficient(float environmentCoefficient);

	Material& GetMaterialDataAddress() { return *materialData_; }

	const Node& GetRootNode() const { return modelData_->rootNode; }

	const ModelData& GetModelData() const { return *modelData_; }
	ModelData& GetModelData() { return *modelData_; }

	uint32_t GetVerticesNum() { return verticesNum_; }

	ModelPlatform* GetModelPlatform() { return modelPlatform_; }

	//Transforms& GetTransformAddress() { return transform_; }

	//void SetRotate(const Vector3& rotate) { transform_.rotate = rotate; }

protected:

	//頂点データ作成
	void CreateVertexData();

	//インデックスデータ作成
	void CreateIndexData();

	//マテリアルデータ作成
	void CreateMaterialData(const Vector4& color = {1.0f, 1.0f, 1.0f, 1.0f});
	/*
	//座標行列変換データ作成
	void CreateTransformData();
	*/
	//objファイルの読み込み
	virtual void LoadModelFile(const std::string& directoryPath, const std::string& filename);

	//頂点データ読み込み
	void LoadVertexData(aiMesh* mesh);

	//インデックスデータ読み込み
	void LoadIndexData(aiMesh* mesh);

	void SetVerticesNum();

	void SetIndecesNum();

	Node ReadNode(aiNode* node);
	
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

	/*
	//TransformationMatrix用のリソースを作る
	Microsoft::WRL::ComPtr<ID3D12Resource> transformationMatrixResource_;
	//データを書き込む
	TransformationMatrix* transformationMatrixData_ = nullptr;
	*/

	uint32_t verticesNum_;
	uint32_t indecesNum_;
	uint32_t textureHandle_;

};

