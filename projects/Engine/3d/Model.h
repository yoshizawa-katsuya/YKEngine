#pragma once
#include "Struct.h"
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

class Model
{
public:

	
	void Initialize(ModelPlatform* modelPlatform);

	void CreateModel(const std::string& directoryPath, const std::string& filename);

	void CreateSphere(uint32_t textureHandle);

	void Draw(const Transforms& transform, Camera* camera);

	void Draw(const Transforms& transform, Camera* camera, Animation* animation);

	Material& GetMaterialDataAddress() { return *materialData_; }

	const Node& GetRootNode() { return modelData_.rootNode; }

	//Transforms& GetTransformAddress() { return transform_; }

	//void SetRotate(const Vector3& rotate) { transform_.rotate = rotate; }

private:

	//頂点データ作成
	void CreateVertexData();

	//マテリアルデータ作成
	void CreateMaterialData();

	//座標行列変換データ作成
	void CreateTransformData();

	//objファイルの読み込み
	void LoadModelFile(const std::string& directoryPath, const std::string& filename);

	//mtlファイルの読み込み
	void LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename);

	Node ReadNode(aiNode* node);
	
	ModelPlatform* modelPlatform_ = nullptr;

	ModelData modelData_;

	//VertexResourceを生成
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource_;

	VertexData* vertexData_ = nullptr;
	//頂点バッファビューを作成する
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};

	//マテリアル用のリソースを作る
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_;
	//マテリアルにデータを書き込む
	Material* materialData_ = nullptr;

	//TransformationMatrix用のリソースを作る
	Microsoft::WRL::ComPtr<ID3D12Resource> transformationMatrixResource_;
	//データを書き込む
	TransformationMatrix* transformationMatrixData_ = nullptr;

	

	uint32_t textureHandle_;

};

