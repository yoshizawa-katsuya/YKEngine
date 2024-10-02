#include "Model.h"
#include "Matrix.h"
#include <fstream>
#include <sstream>
#include <cassert>
#include <numbers>
#include "ModelPlatform.h"
#include "Camera.h"
#include "Animation.h"
#include "Struct.h"

void Model::Initialize(ModelPlatform* modelPlatform)
{

	modelPlatform_ = modelPlatform;

}

void Model::CreateModel(const std::string& directoryPath, const std::string& filename) {

	//モデル読み込み
	LoadModelFile(directoryPath, filename);

	CreateVertexData();
	
	CreateIndexData();

	CreateMaterialData();

	//CreateTransformData();

	textureHandle_ = TextureManager::GetInstance()->Load(modelData_.material.textureFilePath);

}

void Model::CreateSkinModel(const std::string& directoryPath, const std::string& filename)
{

	//モデル読み込み
	LoadModelFile(directoryPath, filename);

	CreateVertexData();

	CreateIndexData();

	CreateMaterialData();

	//CreateTransformData();

	CreateSkelton();

	CreateSkinCluster();

	textureHandle_ = TextureManager::GetInstance()->Load(modelData_.material.textureFilePath);

	isSkinModel_ = true;

}

void Model::CreateSphere(uint32_t textureHandle)
{

	//球の分割数
	const uint32_t kSubdivision = 16;
	//円周率
	const float pi = std::numbers::pi_v<float>;

	//VertexResourceを生成
	vertexResource_ = modelPlatform_->GetDxCommon()->CreateBufferResource(sizeof(VertexData) * (kSubdivision * kSubdivision * 4));

	//リソースの先頭のアドレスから使う
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点3つ分のサイズ
	vertexBufferView_.SizeInBytes = UINT(sizeof(VertexData) * (kSubdivision * kSubdivision * 4));
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
			uint32_t start = (latIndex * kSubdivision + lonIndex) * 4;
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
			//b
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
			//c
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
			

		}
	}

	modelData_.vertices.resize(kSubdivision * kSubdivision * 4);


	indexResource_ = modelPlatform_->GetDxCommon()->CreateBufferResource(sizeof(uint32_t) * (kSubdivision * kSubdivision * 6));

	//リソースの先頭のアドレスから使う
	indexBufferView_.BufferLocation = indexResource_->GetGPUVirtualAddress();

	indexBufferView_.SizeInBytes = sizeof(uint32_t) * (kSubdivision * kSubdivision * 6);
	//インデックスはuint32_tとする
	indexBufferView_.Format = DXGI_FORMAT_R32_UINT;

	indexResource_->Map(0, nullptr, reinterpret_cast<void**>(&indexData_));

	for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex) {
		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex) {

			uint32_t index = (latIndex * kSubdivision + lonIndex) * 6;
			uint32_t index2 = (latIndex * kSubdivision + lonIndex) * 4;

			indexData_[index] = index2;
			uint32_t i = 1;
			indexData_[index + i] = index2 + 1;
			i++;
			indexData_[index + i] = index2 + 2;
			i++;
			indexData_[index + i] = index2 + 1;
			i++;
			indexData_[index + i] = index2 + 3;
			i++;
			indexData_[index + i] = index2 + 2;
		}
	}

	modelData_.indeces.resize(kSubdivision * kSubdivision * 6);

	CreateMaterialData();

	//CreateTransformData();

	textureHandle_ = textureHandle;


}

void Model::Draw(const EulerTransform& transform, Camera* camera) {

	Matrix4x4 worldMatrix = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
	
	Matrix4x4 worldViewProjectionMatrix;
	if (camera) {
		const Matrix4x4& viewProjectionMatrix = camera->GetViewProjection();
		worldViewProjectionMatrix = Multiply(worldMatrix, viewProjectionMatrix);
	}
	else {
		worldViewProjectionMatrix = worldMatrix;
	}
	/*
	transformationMatrixData_->WVP = worldViewProjectionMatrix;
	transformationMatrixData_->World = worldMatrix;

	//wvp用のCBufferの場所を設定
	modelPlatform_->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(1, transformationMatrixResource_->GetGPUVirtualAddress());
	*/

	modelPlatform_->ModelDraw(worldViewProjectionMatrix, worldMatrix, camera);

	if (isSkinModel_) {

		std::array<D3D12_VERTEX_BUFFER_VIEW, 2> vbvs = {
			vertexBufferView_,
			skinCluster_.influenceBufferView
		};

		modelPlatform_->GetDxCommon()->GetCommandList()->IASetVertexBuffers(0, 2, &vbvs.front());	//VBVを設定

		modelPlatform_->GetDxCommon()->GetCommandList()->SetGraphicsRootDescriptorTable(4, skinCluster_.paletteSrvHandle.second);

	}
	else {

		modelPlatform_->GetDxCommon()->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);	//VBVを設定

	}

	modelPlatform_->GetDxCommon()->GetCommandList()->IASetIndexBuffer(&indexBufferView_);
	//マテリアルのCBufferの場所を設定
	modelPlatform_->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	//SRVのDescriptorTableの先頭を設定。2はrootParameter[2]である。
	TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(textureHandle_);

	//描画1(DrawCall/ドローコール)。3頂点で1つのインスタンス。インスタンスについては今後
		//commandList_->DrawIndexedInstanced((kSubdivision * kSubdivision * 6), 1, 0, 0, 0);
	//modelPlatform_->GetDxCommon()->GetCommandList()->DrawInstanced(UINT(modelData_.vertices.size()), 1, 0, 0);
	modelPlatform_->GetDxCommon()->GetCommandList()->DrawIndexedInstanced(UINT(modelData_.indeces.size()), 1, 0, 0, 0);


}

void Model::Draw(const EulerTransform& transform, Camera* camera, Animation* animation)
{

	Matrix4x4 localMatrix = animation->Reproducing(this);

	Matrix4x4 worldMatrix = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);

	Matrix4x4 worldViewProjectionMatrix;
	if (camera) {
		const Matrix4x4& viewProjectionMatrix = camera->GetViewProjection();
		worldViewProjectionMatrix = Multiply(worldMatrix, viewProjectionMatrix);
	}
	else {
		worldViewProjectionMatrix = worldMatrix;
	}
	/*
	transformationMatrixData_->WVP = localMatrix * worldViewProjectionMatrix;
	transformationMatrixData_->World = localMatrix * worldMatrix;

	//wvp用のCBufferの場所を設定
	modelPlatform_->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(1, transformationMatrixResource_->GetGPUVirtualAddress());
	*/

	worldViewProjectionMatrix = localMatrix * worldViewProjectionMatrix;
	worldMatrix = localMatrix * worldMatrix;

	modelPlatform_->ModelDraw(worldViewProjectionMatrix, worldMatrix, camera);

	modelPlatform_->GetDxCommon()->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);	//VBVを設定
	
	modelPlatform_->GetDxCommon()->GetCommandList()->IASetIndexBuffer(&indexBufferView_);
	//マテリアルのCBufferの場所を設定
	modelPlatform_->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	//SRVのDescriptorTableの先頭を設定。2はrootParameter[2]である。
	TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(textureHandle_);

	//描画1(DrawCall/ドローコール)。3頂点で1つのインスタンス。インスタンスについては今後
		//commandList_->DrawIndexedInstanced((kSubdivision * kSubdivision * 6), 1, 0, 0, 0);
	//modelPlatform_->GetDxCommon()->GetCommandList()->DrawInstanced(UINT(modelData_.vertices.size()), 1, 0, 0);
	modelPlatform_->GetDxCommon()->GetCommandList()->DrawIndexedInstanced(UINT(modelData_.indeces.size()), 1, 0, 0, 0);


}

void Model::BoneDraw(const EulerTransform& transform, Camera* camera)
{

	Matrix4x4 worldMatrix = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);

	//全てのJointを更新。親が若いので通常ループで処理可能になっている
	for (Joint& joint : skeleton_.joints) {
		if (joint.parent) {	//親がいれば親の行列を掛ける

			Matrix4x4 jointWorldMatrix1, jointWorldMatrix2;

			jointWorldMatrix1 = joint.skeletonSpaceMatrix * worldMatrix;
			jointWorldMatrix2 = skeleton_.joints[joint.parent.value()].skeletonSpaceMatrix * worldMatrix;

			modelPlatform_->LineDraw(jointWorldMatrix1, jointWorldMatrix2, camera);

		}
	}

}

void Model::JointDraw(const EulerTransform& transform, Camera* camera)
{

	Matrix4x4 worldMatrix = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);

	//全てのJointを更新。親が若いので通常ループで処理可能になっている
	for (Joint& joint : skeleton_.joints) {
		
		Matrix4x4 jointWorldMatrix;

		jointWorldMatrix = joint.skeletonSpaceMatrix * worldMatrix;
		
		modelPlatform_->SphereDraw(jointWorldMatrix, camera);
	}

}

void Model::ApplyAnimation(Animation* animation)
{
	for (Joint& joint : skeleton_.joints) {
		//対象のJointのAnimationがあれば、値の適用を行う。下記のif文はC++17から可能になった初期化付きif文。
		if (auto it = animation->GetNodeAnimations().find(joint.name); it != animation->GetNodeAnimations().end()) {
			const NodeAnimation& rootNodeAnimation = (*it).second;
			joint.transform.translate = animation->CalculateValue(rootNodeAnimation.translate.keyframes, animation->GetAnimationTime());
			joint.transform.rotate = animation->CalculateValue(rootNodeAnimation.rotate.keyframes, animation->GetAnimationTime());
			joint.transform.scale = animation->CalculateValue(rootNodeAnimation.scale.keyframes, animation->GetAnimationTime());

		}
	}
}

void Model::CreateVertexData()
{

	//VertexResourceを生成
	vertexResource_ = modelPlatform_->GetDxCommon()->CreateBufferResource(sizeof(VertexData) * modelData_.vertices.size());

	//リソースの先頭のアドレスから使う
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点数分のサイズ
	vertexBufferView_.SizeInBytes = UINT(sizeof(VertexData) * modelData_.vertices.size());
	//1頂点当たりのサイズ
	vertexBufferView_.StrideInBytes = sizeof(VertexData);

	//頂点リソースにデータを書き込む
	//書き込むためのアドレスを取得
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));
	//頂点データをリソースにコピー
	std::memcpy(vertexData_, modelData_.vertices.data(), sizeof(VertexData) * modelData_.vertices.size());


}

void Model::CreateIndexData()
{

	indexResource_ = modelPlatform_->GetDxCommon()->CreateBufferResource(sizeof(uint32_t) * modelData_.indeces.size());

	indexBufferView_.BufferLocation = indexResource_->GetGPUVirtualAddress();
	indexBufferView_.SizeInBytes = UINT(sizeof(uint32_t) * modelData_.indeces.size());
	indexBufferView_.Format = DXGI_FORMAT_R32_UINT;

	indexResource_->Map(0, nullptr, reinterpret_cast<void**>(&indexData_));
	std::memcpy(indexData_, modelData_.indeces.data(), sizeof(uint32_t) * modelData_.indeces.size());

}

void Model::CreateMaterialData()
{

	//マテリアル用のリソースを作る。今回はcolor1つ分のサイズを用意する
	materialResource_ = modelPlatform_->GetDxCommon()->CreateBufferResource(sizeof(Material));
	//マテリアルにデータを書き込む
	//書き込むためのアドレスを取得
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	//白を書き込む
	materialData_->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	materialData_->enableLighting = true;
	materialData_->uvTransform = MakeIdentity4x4();

}
/*
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
*/
void Model::LoadModelFile(const std::string& directoryPath, const std::string& filename) {

	Assimp::Importer importer;
	std::string filepath = directoryPath + "/" + filename;
	const aiScene* scene = importer.ReadFile(filepath.c_str(), aiProcess_FlipWindingOrder | aiProcess_FlipUVs);

	assert(scene->HasMeshes());	//メッシュがないのは対応しない

	for (uint32_t meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex) {
		aiMesh* mesh = scene->mMeshes[meshIndex];
		assert(mesh->HasNormals());	//法線がないメッシュは今回は非対応
		assert(mesh->HasTextureCoords(0));	//TexcoordがないMeshは今回は非対応
		modelData_.vertices.resize(mesh->mNumVertices);	//最初に頂点数分のメモリを確保しておく
		for (uint32_t vertexIndex = 0; vertexIndex < mesh->mNumVertices; ++vertexIndex) {
			aiVector3D& position = mesh->mVertices[vertexIndex];
			aiVector3D& normal = mesh->mNormals[vertexIndex];
			aiVector3D& texcord = mesh->mTextureCoords[0][vertexIndex];
			//右手系->左手系の変換
			modelData_.vertices[vertexIndex].position = { -position.x, position.y, position.z, 1.0f };
			modelData_.vertices[vertexIndex].normal = { -normal.x, normal.y, normal.z };
			modelData_.vertices[vertexIndex].texcoord = { texcord.x, texcord.y };
		}

		for (uint32_t faceIndex = 0; faceIndex < mesh->mNumFaces; ++faceIndex) {
			aiFace& face = mesh->mFaces[faceIndex];
			assert(face.mNumIndices == 3);

			for (uint32_t element = 0; element < face.mNumIndices; ++element) {
				uint32_t vertexIndex = face.mIndices[element];
				modelData_.indeces.push_back(vertexIndex);
			}
		}

		//SkinCluster構築用のデータを取得
		for (uint32_t boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex) {
			aiBone* bone = mesh->mBones[boneIndex];
			std::string jointName = bone->mName.C_Str();
			JointWeightData& jointWeightData = modelData_.skinClusterData[jointName];

			aiMatrix4x4 bindPoseMatrixAssimp = bone->mOffsetMatrix.Inverse();	//BindPoseMatrixに戻す
			aiVector3D scale, translate;
			aiQuaternion rotate;
			bindPoseMatrixAssimp.Decompose(scale, rotate, translate);	//成分を抽出
			//左手系のBindPoseMatrixを作る
			Matrix4x4 bindPoseMatrix = MakeAffineMatrix(
				{scale.x, scale.y, scale.z}, { rotate.x, -rotate.y, -rotate.z, rotate.w }, { -translate.x, translate.y, translate.z });
			//InverseBindPoseMatrixにする
			jointWeightData.inverseBindPoseMatrix = Inverse(bindPoseMatrix);

			for (uint32_t weightIndex = 0; weightIndex < bone->mNumWeights; ++weightIndex) {
				jointWeightData.vertexWeights.push_back({ bone->mWeights[weightIndex].mWeight, bone->mWeights[weightIndex].mVertexId });
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
	/*
	aiMatrix4x4 aiLocalMatrix = node->mTransformation;	//nodeのlocalMatrixを取得
	aiLocalMatrix.Transpose();	//列ベクトルを行ベクトル形式に転置
	for (uint32_t i = 0; i < 4; i++) {
		for (uint32_t j = 0; j < 4; j++) {
			result.localMatrix.m[i][j] = aiLocalMatrix[i][j];	//他の要素も同様に
		}
	}
	*/

	aiVector3D scale, translate;
	aiQuaternion rotate;
	node->mTransformation.Decompose(scale, rotate, translate);	//assimpの行列からSRTを抽出する関数を利用
	result.transform.scale = { scale.x, scale.y, scale.z };	//Scaleはそのまま
	result.transform.rotate = { rotate.x, -rotate.y, -rotate.z, rotate.w };	//x軸を反転、さらに回転方向が逆なので軸を反転させる
	result.transform.translate = { -translate.x, translate.y, translate.z };	//x軸を反転
	result.localMatrix = MakeAffineMatrix(result.transform.scale, result.transform.rotate, result.transform.translate);

	result.name = node->mName.C_Str();	//Node名を格納
	result.children.resize(node->mNumChildren);	//子供の数だけ確保
	for (uint32_t childIndex = 0; childIndex < node->mNumChildren; ++childIndex) {
		//再帰的に読んで階層構造を作っていく
		result.children[childIndex] = ReadNode(node->mChildren[childIndex]);
	}

	return result;
}

void Model::CreateSkelton()
{

	skeleton_.root = CreateJoint(modelData_.rootNode, {});

	//名前とindexのマッピングを行いアクセスしやすくする
	for (const Joint& joint : skeleton_.joints) {
		skeleton_.jointMap.emplace(joint.name, joint.index);
	}

	SkeletonUpdate();

}

int32_t Model::CreateJoint(const Node& node, const std::optional<int32_t>& parent)
{
	
	Joint joint;
	joint.name = node.name;
	joint.localMatrix = node.localMatrix;
	joint.skeletonSpaceMatrix = MakeIdentity4x4();
	joint.transform = node.transform;
	joint.index = int32_t(skeleton_.joints.size());	//現在登録されている数をIndexに
	joint.parent = parent;
	skeleton_.joints.push_back(joint);	//SkeletonのJoints列に追加
	for (const Node& child : node.children) {
		//子Jointを作成し、そのIndexを登録
		int32_t childIndex = CreateJoint(child, joint.index);
		skeleton_.joints[joint.index].children.push_back(childIndex);
	}
	//自身のIndexを返す
	return joint.index;
}

void Model::SkeletonUpdate()
{

	//全てのJointを更新。親が若いので通常ループで処理可能になっている
	for (Joint& joint : skeleton_.joints) {
		joint.localMatrix = MakeAffineMatrix(joint.transform.scale, joint.transform.rotate, joint.transform.translate);
		if (joint.parent) {	//親がいれば親の行列を掛ける
			joint.skeletonSpaceMatrix = joint.localMatrix * skeleton_.joints[*joint.parent].skeletonSpaceMatrix;
		}
		else {	//親がいないのでlocalMatrixとskeltonSpaceMatrixは一致する
			joint.skeletonSpaceMatrix = joint.localMatrix;
		}
	}

}

void Model::CreateSkinCluster()
{

	SrvHeapManager* srvHeapManager = modelPlatform_->GetSrvHeapManager();

	//palette用のResourceを確保
	skinCluster_.paletteResource = modelPlatform_->GetDxCommon()->CreateBufferResource(sizeof(WellForGPU) * skeleton_.joints.size());
	WellForGPU* mappedPalette = nullptr;
	skinCluster_.paletteResource->Map(0, nullptr, reinterpret_cast<void**>(&mappedPalette));
	skinCluster_.mappedPalette = { mappedPalette, skeleton_.joints.size() };	//spanを使ってアクセスするようにする

	int32_t srvIndex = srvHeapManager->Allocate();

	skinCluster_.paletteSrvHandle.first = srvHeapManager->GetCPUDescriptorHandle(srvIndex);
	skinCluster_.paletteSrvHandle.second = srvHeapManager->GetGPUDescriptorHandle(srvIndex);

	//palette用のsrvを作成。StructuredBufferでアクセスできるようにする。
	D3D12_SHADER_RESOURCE_VIEW_DESC palletteSrvDesc{};
	palletteSrvDesc.Format = DXGI_FORMAT_UNKNOWN;
	palletteSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	palletteSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	palletteSrvDesc.Buffer.FirstElement = 0;
	palletteSrvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	palletteSrvDesc.Buffer.NumElements = UINT(skeleton_.joints.size());
	palletteSrvDesc.Buffer.StructureByteStride = sizeof(WellForGPU);
	modelPlatform_->GetDxCommon()->GetDevice()->CreateShaderResourceView(skinCluster_.paletteResource.Get(), &palletteSrvDesc, skinCluster_.paletteSrvHandle.first);

	//influence用のResourceを確保。頂点ごとにinfluence情報を追加できるようにする
	skinCluster_.influenceResource = modelPlatform_->GetDxCommon()->CreateBufferResource(sizeof(VertexInfluence) * modelData_.vertices.size());
	VertexInfluence* mappedInfluence = nullptr;
	skinCluster_.influenceResource->Map(0, nullptr, reinterpret_cast<void**>(&mappedInfluence));
	std::memset(mappedInfluence, 0, sizeof(VertexInfluence) * modelData_.vertices.size());	//0埋め。weightを0にしておく
	skinCluster_.mappedInfluence = { mappedInfluence, modelData_.vertices.size() };

	//Influence用のVBVを確保
	skinCluster_.influenceBufferView.BufferLocation = skinCluster_.influenceResource->GetGPUVirtualAddress();
	skinCluster_.influenceBufferView.SizeInBytes = UINT(sizeof(VertexInfluence) * modelData_.vertices.size());
	skinCluster_.influenceBufferView.StrideInBytes = sizeof(VertexInfluence);

	//InverseBindPoseMatrixを格納する場所を作成して、単位行列で埋める
	skinCluster_.inverseBindPoseMatrices.resize(skeleton_.joints.size());
	std::generate(skinCluster_.inverseBindPoseMatrices.begin(), skinCluster_.inverseBindPoseMatrices.end(), MakeIdentity4x4);

	//ModelDataのSkinCluster情報を解析してInfluenceの中身を埋める
	for (const auto& jointWeight : modelData_.skinClusterData) {	//ModelのSkinClusterの情報を解析
		auto it = skeleton_.jointMap.find(jointWeight.first);	//jointWeight.firstはjoint名なので、skeltonに対象となるjointが含まれているか判断
		if (it == skeleton_.jointMap.end()) {	//そんな名前のJointは存在しない。なので次に回す
			continue;
		}
		//(*it).secondにはjointのindexが入っているので、外套のindexのinverseBindPoseMatrixを代入
		skinCluster_.inverseBindPoseMatrices[(*it).second] = jointWeight.second.inverseBindPoseMatrix;
		for (const auto& vertexWeight : jointWeight.second.vertexWeights) {
			auto& currentInfluence = skinCluster_.mappedInfluence[vertexWeight.vertexIndex];	//該当のvertexIndex
			for (uint32_t index = 0; index < kNumMaxInfluence; ++index) {	//空いているところに入れる
				if (currentInfluence.weights[index] == 0.0f) {	//weight==0が空いている状態なので、その場所にweightとjointのindexを代入
					currentInfluence.weights[index] = vertexWeight.weight;
					currentInfluence.jointIndices[index] = (*it).second;
					break;
				}
			}
		}
	}

}

void Model::SkinClusterUpdate()
{

	for (size_t jointIndex = 0; jointIndex < skeleton_.joints.size(); ++jointIndex) {
		assert(jointIndex < skinCluster_.inverseBindPoseMatrices.size());
		skinCluster_.mappedPalette[jointIndex].skeletonSpaceMatrix = 
			skinCluster_.inverseBindPoseMatrices[jointIndex] * skeleton_.joints[jointIndex].skeletonSpaceMatrix;
		skinCluster_.mappedPalette[jointIndex].skeletonSpaceInverseTransposeMatrix =
			Transpose(Inverse(skinCluster_.mappedPalette[jointIndex].skeletonSpaceMatrix));
	}

}
