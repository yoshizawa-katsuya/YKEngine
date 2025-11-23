#include "BaseModel.h"
#include "Matrix.h"
#include <fstream>
#include <sstream>
#include <cassert>
#include <numbers>
#include "ModelPlatform.h"
#include "Camera.h"
#include "Animation.h"
#include "Struct.h"
#include "ThreadPool.h"
#include "RootParams.h"

BaseModel::BaseModel()
	: modelPlatform_(ModelPlatform::GetInstance())
{
}

BaseModel::~BaseModel()
{
}

void BaseModel::CreateModel(const std::string& directoryPath, const std::string& filename, const Vector4& color) {

	//モデル読み込み
	LoadModelFile(directoryPath, filename);
	
	//スレッドに処理を投げる
	ThreadPool::GetInstance()->enqueueTask([this, color]() {
		CreateVertexData();
		CreateIndexData();
		CreateMaterialData(color);
		textureHandle_ = TextureManager::GetInstance()->Load(modelData_->material.textureFilePath);
	});

	
}

void BaseModel::CreateSphere(uint32_t textureHandle)
{
}

void BaseModel::CreateCube(uint32_t textureHandle)
{
}

void BaseModel::CreatePlane(uint32_t textureHandle)
{
}

void BaseModel::CreateRing(uint32_t textureHandle)
{
}

void BaseModel::CreateCylinder(uint32_t textureHandle)
{
}

void BaseModel::CreateSkyBox(uint32_t textureHandle)
{
}

void BaseModel::Draw(bool usedMaterial) {
	
	modelPlatform_->GetDxCommon()->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);	//VBVを設定

	modelPlatform_->GetDxCommon()->GetCommandList()->IASetIndexBuffer(&indexBufferView_);
	if (!usedMaterial) 
	{
		//マテリアルのCBufferの場所を設定
		modelPlatform_->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(static_cast<size_t>(ModelRootParam::kMaterial), materialResource_->GetGPUVirtualAddress());

	}
	//テクスチャハンドルを設定
	TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(static_cast<uint32_t>(ModelRootParam::kTexture), textureHandle_);

	//描画1(DrawCall/ドローコール)。
	modelPlatform_->GetDxCommon()->GetCommandList()->DrawIndexedInstanced(indecesNum_, 1, 0, 0, 0);


}

void BaseModel::Draw(uint32_t textureHandle, bool usedMaterial)
{

	modelPlatform_->GetDxCommon()->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);	//VBVを設定

	modelPlatform_->GetDxCommon()->GetCommandList()->IASetIndexBuffer(&indexBufferView_);
	if (!usedMaterial)
	{
		//マテリアルのCBufferの場所を設定
		modelPlatform_->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(static_cast<size_t>(ModelRootParam::kMaterial), materialResource_->GetGPUVirtualAddress());

	}
	//テクスチャハンドルを設定
	TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(static_cast<uint32_t>(ModelRootParam::kTexture), textureHandle);

	//描画1(DrawCall/ドローコール)。
	modelPlatform_->GetDxCommon()->GetCommandList()->DrawIndexedInstanced(indecesNum_, 1, 0, 0, 0);

}

void BaseModel::InstancingDraw(uint32_t numInstance)
{

	if (numInstance == 0)
	{
		return; // インスタンス数が0の場合は描画しない
	}

	modelPlatform_->GetDxCommon()->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);	//VBVを設定

	modelPlatform_->GetDxCommon()->GetCommandList()->IASetIndexBuffer(&indexBufferView_);
	//マテリアルのCBufferの場所を設定
	modelPlatform_->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(static_cast<size_t>(ModelRootParam::kMaterial), materialResource_->GetGPUVirtualAddress());
	//テクスチャハンドルを設定
	TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(static_cast<uint32_t>(ModelRootParam::kTexture), textureHandle_);

	//描画1(DrawCall/ドローコール)。
	modelPlatform_->GetDxCommon()->GetCommandList()->DrawIndexedInstanced(indecesNum_, numInstance, 0, 0, 0);
}

void BaseModel::InstancingDraw(uint32_t numInstance, uint32_t textureHandle)
{

	if (numInstance == 0)
	{
		return; // インスタンス数が0の場合は描画しない
	}

	modelPlatform_->GetDxCommon()->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);	//VBVを設定

	modelPlatform_->GetDxCommon()->GetCommandList()->IASetIndexBuffer(&indexBufferView_);
	//マテリアルのCBufferの場所を設定
	modelPlatform_->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(static_cast<size_t>(ModelRootParam::kMaterial), materialResource_->GetGPUVirtualAddress());
	//テクスチャハンドルを設定
	TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(static_cast<uint32_t>(ModelRootParam::kTexture), textureHandle);

	//描画1(DrawCall/ドローコール)。
	modelPlatform_->GetDxCommon()->GetCommandList()->DrawIndexedInstanced(indecesNum_, numInstance, 0, 0, 0);
}

void BaseModel::SetSkinCluster(const SkinCluster& skinCluster)
{
}

void BaseModel::SetColor(const Vector4& color)
{
	ThreadPool::GetInstance()->waitForCompletion();
	materialData_->color = color;
}	

void BaseModel::SetAlpha(float alpha)
{
	ThreadPool::GetInstance()->waitForCompletion();
	materialData_->color.w = alpha;
}

void BaseModel::SetUVTransform(const Vector3& scale, const Vector3& rotate, const Vector3& translate)
{
	ThreadPool::GetInstance()->waitForCompletion();
	Matrix4x4 uvTransformMatrix = MakeAffineMatrix(scale, rotate, translate);
	materialData_->uvTransform = uvTransformMatrix;
}

void BaseModel::SetUVTransform(const EulerTransform& uvTransform)
{
	ThreadPool::GetInstance()->waitForCompletion();
	Matrix4x4 uvTransformMatrix = MakeAffineMatrix(uvTransform);
	materialData_->uvTransform = uvTransformMatrix;
}

void BaseModel::SetEnableLighting(bool enableLighting)
{
	ThreadPool::GetInstance()->waitForCompletion();
	materialData_->enableLighting = enableLighting;
}

void BaseModel::SetEnvironmentCoefficient(float environmentCoefficient)
{
	ThreadPool::GetInstance()->waitForCompletion();
	materialData_->enviromentCoefficient = environmentCoefficient;
}

void BaseModel::CreateVertexData()
{

	//VertexResourceを生成
	vertexResource_ = modelPlatform_->GetDxCommon()->CreateBufferResource(sizeof(VertexData) * modelData_->vertices.size());

	//リソースの先頭のアドレスから使う
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点数分のサイズ
	vertexBufferView_.SizeInBytes = UINT(sizeof(VertexData) * modelData_->vertices.size());
	//1頂点当たりのサイズ
	vertexBufferView_.StrideInBytes = sizeof(VertexData);

	//頂点リソースにデータを書き込む
	//書き込むためのアドレスを取得
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));
	//頂点データをリソースにコピー
	std::memcpy(vertexData_, modelData_->vertices.data(), sizeof(VertexData) * modelData_->vertices.size());
	SetVerticesNum();
}

void BaseModel::CreateIndexData()
{

	indexResource_ = modelPlatform_->GetDxCommon()->CreateBufferResource(sizeof(uint32_t) * modelData_->indeces.size());

	indexBufferView_.BufferLocation = indexResource_->GetGPUVirtualAddress();
	indexBufferView_.SizeInBytes = UINT(sizeof(uint32_t) * modelData_->indeces.size());
	indexBufferView_.Format = DXGI_FORMAT_R32_UINT;

	indexResource_->Map(0, nullptr, reinterpret_cast<void**>(&indexData_));
	std::memcpy(indexData_, modelData_->indeces.data(), sizeof(uint32_t) * modelData_->indeces.size());
	SetIndecesNum();
}

void BaseModel::CreateMaterialData(const Vector4& color)
{

	//マテリアル用のリソースを作る。今回はcolor1つ分のサイズを用意する
	materialResource_ = modelPlatform_->GetDxCommon()->CreateBufferResource(sizeof(Material));
	//マテリアルにデータを書き込む
	//書き込むためのアドレスを取得
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	//白を書き込む
	materialData_->color = color;
	materialData_->enableLighting = true;
	materialData_->shininess = 40.0f;
	materialData_->enviromentCoefficient = 0.0f;	//環境光の係数は0.0fにしておく
	materialData_->uvTransform = MakeIdentity4x4();
	
}

void BaseModel::LoadModelFile(const std::string& directoryPath, const std::string& filename) {

	modelData_ = std::make_unique<ModelData>();

	Assimp::Importer importer;
	std::string filepath = directoryPath + "/" + filename;
	const aiScene* scene = importer.ReadFile(filepath.c_str(), aiProcess_JoinIdenticalVertices | aiProcess_Triangulate | aiProcess_FlipWindingOrder | aiProcess_FlipUVs);

	assert(scene->HasMeshes());	//メッシュがないのは対応しない

	for (uint32_t meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex)
	{
		aiMesh* mesh = scene->mMeshes[meshIndex];
		assert(mesh->HasNormals());	//法線がないメッシュは今回は非対応
		assert(mesh->HasTextureCoords(0));	//TexcoordがないMeshは今回は非対応
		modelData_->vertices.resize(mesh->mNumVertices);	//最初に頂点数分のメモリを確保しておく

		LoadVertexData(mesh);

		LoadIndexData(mesh);

	}

	//マテリアル情報の読み込み
	for (uint32_t materialIndex = 0; materialIndex < scene->mNumMaterials; ++materialIndex)
	{
		aiMaterial* material = scene->mMaterials[materialIndex];
		if (material->GetTextureCount(aiTextureType_DIFFUSE) != 0) {
			aiString textureFilePath;
			material->GetTexture(aiTextureType_DIFFUSE, 0, &textureFilePath);
			modelData_->material.textureFilePath = directoryPath + "/" + textureFilePath.C_Str();
			
		}
		else {
			modelData_->material.textureFilePath = "./resources/white.png";
		}
	}

	modelData_->rootNode = ReadNode(scene->mRootNode);
	
}

void BaseModel::LoadVertexData(aiMesh* mesh)
{
	for (uint32_t vertexIndex = 0; vertexIndex < mesh->mNumVertices; ++vertexIndex) 
	{
		aiVector3D& position = mesh->mVertices[vertexIndex];
		aiVector3D& normal = mesh->mNormals[vertexIndex];
		aiVector3D& texcord = mesh->mTextureCoords[0][vertexIndex];
		//右手系->左手系の変換
		modelData_->vertices[vertexIndex].position = { -position.x, position.y, position.z, 1.0f };
		modelData_->vertices[vertexIndex].normal = { -normal.x, normal.y, normal.z };
		modelData_->vertices[vertexIndex].texcoord = { texcord.x, texcord.y };
	}
}

void BaseModel::LoadIndexData(aiMesh* mesh)
{
	//面ごとに情報を読み込む
	for (uint32_t faceIndex = 0; faceIndex < mesh->mNumFaces; ++faceIndex) 
	{
		aiFace& face = mesh->mFaces[faceIndex];
		assert(face.mNumIndices == 3);

		//面を構成する頂点インデックスを読み込む
		for (uint32_t element = 0; element < face.mNumIndices; ++element) 
		{
			uint32_t vertexIndex = face.mIndices[element];
			modelData_->indeces.push_back(vertexIndex);
		}
	}
}

void BaseModel::SetVerticesNum()
{
	verticesNum_ = static_cast<uint32_t>(modelData_->vertices.size());
	modelData_->vertices.clear();
}

void BaseModel::SetIndecesNum()
{
	indecesNum_ = static_cast<uint32_t>(modelData_->indeces.size());
	modelData_->indeces.clear();
}

Node BaseModel::ReadNode(aiNode* node)
{
	Node result;

	aiVector3D scale, translate;
	aiQuaternion rotate;
	node->mTransformation.Decompose(scale, rotate, translate);	//assimpの行列からSRTを抽出する関数を利用
	result.transform.scale = { scale.x, scale.y, scale.z };	//Scaleはそのまま
	result.transform.rotation = { rotate.x, -rotate.y, -rotate.z, rotate.w };	//x軸を反転、さらに回転方向が逆なので軸を反転させる
	result.transform.translation = { -translate.x, translate.y, translate.z };	//x軸を反転
	result.localMatrix = MakeAffineMatrix(result.transform.scale, result.transform.rotation, result.transform.translation);

	result.name = node->mName.C_Str();	//Node名を格納
	result.children.resize(node->mNumChildren);	//子供の数だけ確保
	for (uint32_t childIndex = 0; childIndex < node->mNumChildren; ++childIndex)
	{
		//再帰的に読んで階層構造を作っていく
		result.children[childIndex] = ReadNode(node->mChildren[childIndex]);
	}

	return result;
}