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

struct VertexWeightData {
	float weight;
	uint32_t vertexIndex;
};

struct JointWeightData {
	Matrix4x4 inverseBindPoseMatrix;
	std::vector<VertexWeightData> vertexWeights;
};

struct MaterialData
{
	std::string textureFilePath;
};

struct Node {
	QuaternionTransform transform;
	Matrix4x4 localMatrix;
	std::string name;
	std::vector<Node> children;
};

struct ModelData {
	std::map<std::string, JointWeightData> skinClusterData;
	std::vector<VertexData> vertices;
	std::vector<uint32_t> indeces;
	MaterialData material;
	Node rootNode;
};

class BaseModel
{
public:

	BaseModel();

	virtual ~BaseModel() = 0;

	//void Initialize(ModelPlatform* modelPlatform);

	//ModelPlatformから呼び出す
	virtual void CreateModel(const std::string& directoryPath, const std::string& filename, const Vector4& color = {1.0f, 1.0f, 1.0f, 1.0f});

	virtual void CreateSphere(uint32_t textureHandle);

	virtual void CreatePlane(uint32_t textureHandle);

	//3dObjectクラス経由で呼び出す
	virtual void Draw();

	//3dObjectクラス経由で呼び出す
	virtual void Draw(uint32_t textureHandle);

	virtual void InstancingDraw(uint32_t numInstance);

	virtual void SetSkinCluster(const SkinCluster& skinCluster);

	//TODO:mtlファイルからuvTransform情報を読み込めるようにする
	virtual void SetUVTransform(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

	virtual void SetUVTransform(const EulerTransform& uvTransform);

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

