#include "Model.h"
#include "Matrix.h"
#include <fstream>
#include <sstream>
#include <cassert>
#include <numbers>
#include "ModelPlatform.h"
#include "Camera.h"

void Model::Initialize(ModelPlatform* modelPlatform)
{

	modelPlatform_ = modelPlatform;

}

void Model::CreateModel(const std::string& directoryPath, const std::string& filename) {

	//モデル読み込み
	LoadModelFile(directoryPath, filename);

	CreateVertexData();
	
	CreateMaterialData();

	CreateTransformData();

	textureHandle_ = TextureManager::GetInstance()->Load(modelData_.material.textureFilePath);

}

void Model::CreateSphere(uint32_t textureHandle)
{

	//球の分割数
	const uint32_t kSubdivision = 16;
	//円周率
	const float pi = std::numbers::pi_v<float>;

	//VertexResourceを生成
	vertexResource_ = modelPlatform_->GetDxCommon()->CreateBufferResource(sizeof(VertexData) * (kSubdivision * kSubdivision * 6));

	//リソースの先頭のアドレスから使う
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点3つ分のサイズ
	vertexBufferView_.SizeInBytes = UINT(sizeof(VertexData) * (kSubdivision * kSubdivision * 6));
	//1頂点当たりのサイズ
	vertexBufferView_.StrideInBytes = sizeof(VertexData);

	//頂点リソースにデータを書き込む
	//書き込むためのアドレスを取得
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));
	//頂点データをリソースにコピー
	//std::memcpy(vertexData_, modelData_.vertices.data(), sizeof(VertexData) * modelData_.vertices.size());

	//経度分割1つ分の角度 φ
	const float kLonEvery = pi * 2.0f / float(kSubdivision);
	//緯度分割1つ分の角度 θ
	const float kLatEvery = pi / float(kSubdivision);
	//緯度の方向に分割
	for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex) {
		float lat = -pi / 2.0f + kLatEvery * latIndex;	//θ
		//経度の方向に分割しながら線を描く
		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex) {
			uint32_t start = (latIndex * kSubdivision + lonIndex) * 6;
			float lon = lonIndex * kLonEvery;	//φ
			//頂点にデータを入力する。基準点a
			vertexData_[start].position.x = cos(lat) * cos(lon);
			vertexData_[start].position.y = sin(lat);
			vertexData_[start].position.z = cos(lat) * sin(lon);
			vertexData_[start].position.w = 1.0f;
			vertexData_[start].texcoord.x = float(lonIndex) / float(kSubdivision);
			vertexData_[start].texcoord.y = 1.0f - float(latIndex) / float(kSubdivision);
			vertexData_[start].normal.x = vertexData_[start].position.x;
			vertexData_[start].normal.y = vertexData_[start].position.y;
			vertexData_[start].normal.z = vertexData_[start].position.z;
			//b1
			uint32_t i = 1;
			vertexData_[start + i].position.x = cos(lat + kLatEvery) * cos(lon);
			vertexData_[start + i].position.y = sin(lat + kLatEvery);
			vertexData_[start + i].position.z = cos(lat + kLatEvery) * sin(lon);
			vertexData_[start + i].position.w = 1.0f;
			vertexData_[start + i].texcoord.x = float(lonIndex) / float(kSubdivision);
			vertexData_[start + i].texcoord.y = 1.0f - float(latIndex + 1) / float(kSubdivision);
			vertexData_[start + i].normal.x = vertexData_[start + i].position.x;
			vertexData_[start + i].normal.y = vertexData_[start + i].position.y;
			vertexData_[start + i].normal.z = vertexData_[start + i].position.z;
			//c1
			i++;
			vertexData_[start + i].position.x = cos(lat) * cos(lon + kLonEvery);
			vertexData_[start + i].position.y = sin(lat);
			vertexData_[start + i].position.z = cos(lat) * sin(lon + kLonEvery);
			vertexData_[start + i].position.w = 1.0f;
			vertexData_[start + i].texcoord.x = float(lonIndex + 1) / float(kSubdivision);
			vertexData_[start + i].texcoord.y = 1.0f - float(latIndex) / float(kSubdivision);
			vertexData_[start + i].normal.x = vertexData_[start + i].position.x;
			vertexData_[start + i].normal.y = vertexData_[start + i].position.y;
			vertexData_[start + i].normal.z = vertexData_[start + i].position.z;
			//b2
			i++;
			vertexData_[start + i] = vertexData_[start + 1];
			//d
			i++;
			vertexData_[start + i].position.x = cos(lat + kLatEvery) * cos(lon + kLonEvery);
			vertexData_[start + i].position.y = sin(lat + kLatEvery);
			vertexData_[start + i].position.z = cos(lat + kLatEvery) * sin(lon + kLonEvery);
			vertexData_[start + i].position.w = 1.0f;
			vertexData_[start + i].texcoord.x = float(lonIndex + 1) / float(kSubdivision);
			vertexData_[start + i].texcoord.y = 1.0f - float(latIndex + 1) / float(kSubdivision);
			vertexData_[start + i].normal.x = vertexData_[start + i].position.x;
			vertexData_[start + i].normal.y = vertexData_[start + i].position.y;
			vertexData_[start + i].normal.z = vertexData_[start + i].position.z;
			//c2
			i++;
			vertexData_[start + i] = vertexData_[start + 2];

		}
	}

	modelData_.vertices.resize(kSubdivision * kSubdivision * 6);


	

	CreateMaterialData();

	CreateTransformData();

	textureHandle_ = textureHandle;


}

void Model::Draw(const Transforms& transform, Camera* camera) {

	Matrix4x4 worldMatrix = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
	
	Matrix4x4 worldViewProjectionMatrix;
	if (camera) {
		const Matrix4x4& viewProjectionMatrix = camera->GetViewProjection();
		worldViewProjectionMatrix = Multiply(worldMatrix, viewProjectionMatrix);
	}
	else {
		worldViewProjectionMatrix = worldMatrix;
	}

	transformationMatrixData_->WVP = worldViewProjectionMatrix;
	transformationMatrixData_->World = worldMatrix;

	modelPlatform_->GetDxCommon()->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);	//VBVを設定
	//マテリアルのCBufferの場所を設定
	modelPlatform_->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	//wvp用のCBufferの場所を設定
	modelPlatform_->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(1, transformationMatrixResource_->GetGPUVirtualAddress());
	//SRVのDescriptorTableの先頭を設定。2はrootParameter[2]である。
	TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(textureHandle_);

	//描画1(DrawCall/ドローコール)。3頂点で1つのインスタンス。インスタンスについては今後
		//commandList_->DrawIndexedInstanced((kSubdivision * kSubdivision * 6), 1, 0, 0, 0);
	modelPlatform_->GetDxCommon()->GetCommandList()->DrawInstanced(UINT(modelData_.vertices.size()), 1, 0, 0);


}

void Model::CreateVertexData()
{

	//VertexResourceを生成
	vertexResource_ = modelPlatform_->GetDxCommon()->CreateBufferResource(sizeof(VertexData) * modelData_.vertices.size());

	//リソースの先頭のアドレスから使う
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点3つ分のサイズ
	vertexBufferView_.SizeInBytes = UINT(sizeof(VertexData) * modelData_.vertices.size());
	//1頂点当たりのサイズ
	vertexBufferView_.StrideInBytes = sizeof(VertexData);

	//頂点リソースにデータを書き込む
	//書き込むためのアドレスを取得
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));
	//頂点データをリソースにコピー
	std::memcpy(vertexData_, modelData_.vertices.data(), sizeof(VertexData) * modelData_.vertices.size());


}

void Model::CreateMaterialData()
{

	//マテリアル用のリソースを作る。今回はcolor1つ分のサイズを用意する
	materialResource_ = modelPlatform_->GetDxCommon()->CreateBufferResource(sizeof(Material));
	//マテリアルにデータを書き込む
	//書き込むためのアドレスを取得
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	//白を書き込む
	materialData_->color = Vector4(1.0f, 1.0f, 1.0f, 0.5f);
	materialData_->enableLighting = true;
	materialData_->uvTransform = MakeIdentity4x4();

}

void Model::CreateTransformData()
{

	//transformationMatrixのリソースを作る
	transformationMatrixResource_ = modelPlatform_->GetDxCommon()->CreateBufferResource(sizeof(TransformationMatrix));
	//データを書き込む
	//書き込むためのアドレスを取得
	transformationMatrixResource_->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixData_));
	//単位行列を書き込んでおく
	transformationMatrixData_->WVP = MakeIdentity4x4();

}

void Model::LoadModelFile(const std::string& directoryPath, const std::string& filename) {

	Assimp::Importer importer;
	std::string filepath = directoryPath + "/" + filename;
	const aiScene* scene = importer.ReadFile(filepath.c_str(), aiProcess_FlipWindingOrder | aiProcess_FlipUVs);

	assert(scene->HasMeshes());	//メッシュがないのは対応しない

	for (uint32_t meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex) {
		aiMesh* mesh = scene->mMeshes[meshIndex];
		assert(mesh->HasNormals());	//法線がないメッシュは今回は非対応
		assert(mesh->HasTextureCoords(0));	//TexcoordがないMeshは今回は非対応

		//ここからMeshの中身(Face)の解析を行っていく
		for (uint32_t faceIndex = 0; faceIndex < mesh->mNumFaces; ++faceIndex) {
			aiFace& face = mesh->mFaces[faceIndex];
			assert(face.mNumIndices == 3);	//三角形のみサポート

			//ここからFaceの中身(Vertex)の解析を行っていく
			for (uint32_t element = 0; element < face.mNumIndices; ++element) {
				uint32_t vertexIndex = face.mIndices[element];
				aiVector3D& position = mesh->mVertices[vertexIndex];
				aiVector3D& normal = mesh->mNormals[vertexIndex];
				aiVector3D& texcord = mesh->mTextureCoords[0][vertexIndex];
				VertexData vertex;
				vertex.position = { position.x, position.y, position.z, 1.0f };
				vertex.normal = { normal.x, normal.y, normal.z };
				vertex.texcoord = { texcord.x, texcord.y };
				//aiProcess_MakeLeftHandedはz*=-1で、右手->左手に変換するので手動で対処
				vertex.position.x *= -1.0f;
				vertex.normal.x *= -1.0f;
				modelData_.vertices.push_back(vertex);
			}
		}
	}

	for (uint32_t materialIndex = 0; materialIndex < scene->mNumMaterials; ++materialIndex) {
		aiMaterial* material = scene->mMaterials[materialIndex];
		if (material->GetTextureCount(aiTextureType_DIFFUSE) != 0) {
			aiString textureFilePath;
			material->GetTexture(aiTextureType_DIFFUSE, 0, &textureFilePath);
			modelData_.material.textureFilePath = directoryPath + "/" + textureFilePath.C_Str();
		}
	}

	modelData_.rootNode = ReadNode(scene->mRootNode);

	/*
	//1.中で必要となる変数の宣言
	
	std::vector<Vector4> positions;	//位置
	std::vector<Vector3> normals;	//法線
	std::vector<Vector2> texcords;	//テクスチャ座標
	std::string line;	//ファイルから読んだ1行を格納するもの

	//2.ファイルを開く
	std::ifstream file(directoryPath + "/" + filename);	//ファイルを開く
	assert(file.is_open());	//とりあえず開けなかったら止める

	//3.実際にファイルを読み、ModelDataを構築していく
	while (std::getline(file, line)) {
		std::string identifier;
		std::istringstream s(line);
		s >> identifier;	//先頭の識別子を読む

		if (identifier == "v") {
			Vector4 position;
			s >> position.x >> position.y >> position.z;
			position.x *= -1.0f;
			position.w = 1.0f;
			positions.push_back(position);
		}
		else if (identifier == "vt") {
			Vector2 texcord;
			s >> texcord.x >> texcord.y;
			texcord.y = 1.0f - texcord.y;
			texcords.push_back(texcord);
		}
		else if (identifier == "vn") {
			Vector3 normal;
			s >> normal.x >> normal.y >> normal.z;
			normal.x *= -1.0f;
			normals.push_back(normal);
		}
		else if (identifier == "f") {
			VertexData triangle[3];
			//面は三角形限定。その他は未対応
			for (int32_t faceVertex = 0; faceVertex < 3; ++faceVertex) {
				std::string vertexDefinition;
				s >> vertexDefinition;
				//頂点への要素へのIndexは「位置/UV/法線」で格納されているので、分解してIndexを取得する
				std::istringstream v(vertexDefinition);
				uint32_t elementIndices[3];
				for (int32_t element = 0; element < 3; ++element) {
					std::string index;
					std::getline(v, index, '/');	//区切りでインデックスを読んでいく
					elementIndices[element] = std::stoi(index);
				}
				//要素へのIndexから、実際の要素の値を取得して、頂点を構築する
				Vector4 position = positions[elementIndices[0] - 1];
				Vector2 texcord = texcords[elementIndices[1] - 1];
				Vector3 normal = normals[elementIndices[2] - 1];
				//VertexData vertex = { position, texcord, normal };
				//modelData.vertices.push_back(vertex);
				triangle[faceVertex] = { position, texcord, normal };
			}
			modelData_.vertices.push_back(triangle[2]);
			modelData_.vertices.push_back(triangle[1]);
			modelData_.vertices.push_back(triangle[0]);
		}
		else if (identifier == "mtllib") {
			//materialTemplateLibraryファイルの名前を取得する
			std::string materialFilename;
			s >> materialFilename;
			//基本的にobjファイルと同一階層にmtlは存在させるので、ディレクトリ名とファイル名を渡す
			LoadMaterialTemplateFile(directoryPath, materialFilename);
		}

	}
	*/
	
}

void Model::LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename) {

	//1.中で必要となる変数の宣言
	std::string line;	//ファイルから読んだ1行を格納するもの

	//2.ファイルを開く
	std::ifstream file(directoryPath + "/" + filename);	//ファイルを開く
	assert(file.is_open());	//とりあえず開けなかったら止める

	//3.実際にファイルを読み、ModelDataを構築していく
	while (std::getline(file, line)) {
		std::string identifier;
		std::istringstream s(line);
		s >> identifier;

		//identifierに応じた処理
		if (identifier == "map_Kd") {
			std::string textureFilename;
			s >> textureFilename;
			//連結してファイルパスにする
			modelData_.material.textureFilePath = directoryPath + "/" + textureFilename;
		}
	}

	
}

Node Model::ReadNode(aiNode* node)
{
	Node result;
	aiMatrix4x4 aiLocalMatrix = node->mTransformation;	//nodeのlocalMatrixを取得
	aiLocalMatrix.Transpose();	//列ベクトルを行ベクトル形式に転置
	for (uint32_t i = 0; i < 4; i++) {
		for (uint32_t j = 0; j < 4; j++) {
			result.localMatrix.m[i][j] = aiLocalMatrix[i][j];	//他の要素も同様に
		}
	}
	result.name = node->mName.C_Str();	//Node名を格納
	result.children.resize(node->mNumChildren);	//子供の数だけ確保
	for (uint32_t childIndex = 0; childIndex < node->mNumChildren; ++childIndex) {
		//再帰的に読んで階層構造を作っていく
		result.children[childIndex] = ReadNode(node->mChildren[childIndex]);
	}

	return result;
}
