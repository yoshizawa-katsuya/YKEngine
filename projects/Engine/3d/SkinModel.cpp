#include "SkinModel.h"
#include "ModelPlatform.h"
#include "Matrix.h"
#include "Animation.h"
#include "Camera.h"

SkinModel::~SkinModel()
{
}

void SkinModel::Draw()
{
	//modelPlatform_->ModelDraw(worldViewProjectionMatrix, worldTransform.worldMatrix_, camera);

	modelPlatform_->GetDxCommon()->GetCommandList()->IASetIndexBuffer(&indexBufferView_);
	//マテリアルのCBufferの場所を設定
	modelPlatform_->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	//SRVのDescriptorTableの先頭を設定。2はrootParameter[2]である。
	TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(textureHandle_);

	//描画1(DrawCall/ドローコール)。3頂点で1つのインスタンス。インスタンスについては今後
		//commandList_->DrawIndexedInstanced((kSubdivision * kSubdivision * 6), 1, 0, 0, 0);
	//modelPlatform_->GetDxCommon()->GetCommandList()->DrawInstanced(UINT(modelData_.vertices.size()), 1, 0, 0);
	modelPlatform_->GetDxCommon()->GetCommandList()->DrawIndexedInstanced(indecesNum_, 1, 0, 0, 0);
}

void SkinModel::Draw(uint32_t textureHandle)
{

	//modelPlatform_->ModelDraw(worldViewProjectionMatrix, worldTransform.worldMatrix_, camera);

	modelPlatform_->GetDxCommon()->GetCommandList()->IASetIndexBuffer(&indexBufferView_);
	//マテリアルのCBufferの場所を設定
	modelPlatform_->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	//SRVのDescriptorTableの先頭を設定。2はrootParameter[2]である。
	TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(textureHandle);

	//描画1(DrawCall/ドローコール)。3頂点で1つのインスタンス。インスタンスについては今後
		//commandList_->DrawIndexedInstanced((kSubdivision * kSubdivision * 6), 1, 0, 0, 0);
	//modelPlatform_->GetDxCommon()->GetCommandList()->DrawInstanced(UINT(modelData_.vertices.size()), 1, 0, 0);
	modelPlatform_->GetDxCommon()->GetCommandList()->DrawIndexedInstanced(indecesNum_, 1, 0, 0, 0);

}

void SkinModel::SetSkinCluster(const SkinCluster& skinCluster)
{

	std::array<D3D12_VERTEX_BUFFER_VIEW, 2> vbvs = {
			vertexBufferView_,
			skinCluster.influenceBufferView
	};

	modelPlatform_->GetDxCommon()->GetCommandList()->IASetVertexBuffers(0, 2, &vbvs.front());	//VBVを設定

	modelPlatform_->GetDxCommon()->GetCommandList()->SetGraphicsRootDescriptorTable(8, skinCluster.paletteSrvHandle.second);

}

void SkinModel::LoadModelFile(const std::string& directoryPath, const std::string& filename)
{
	modelData_ = std::make_unique<ModelData>();

	Assimp::Importer importer;
	std::string filepath = directoryPath + "/" + filename;
	const aiScene* scene = importer.ReadFile(filepath.c_str(), aiProcess_JoinIdenticalVertices | aiProcess_Triangulate | aiProcess_FlipWindingOrder | aiProcess_FlipUVs);

	assert(scene->HasMeshes());	//メッシュがないのは対応しない

	for (uint32_t meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex) {
		aiMesh* mesh = scene->mMeshes[meshIndex];
		assert(mesh->HasNormals());	//法線がないメッシュは今回は非対応
		assert(mesh->HasTextureCoords(0));	//TexcoordがないMeshは今回は非対応
		modelData_->vertices.resize(mesh->mNumVertices);	//最初に頂点数分のメモリを確保しておく

		LoadVertexData(mesh);

		LoadIndexData(mesh);

		LoadSkinCluster(mesh);
	}

	//現状だと1つのモデルに複数のテクスチャをつけるのは不可能
	for (uint32_t materialIndex = 0; materialIndex < scene->mNumMaterials; ++materialIndex) {
		aiMaterial* material = scene->mMaterials[materialIndex];
		if (material->GetTextureCount(aiTextureType_DIFFUSE) != 0) {
			aiString textureFilePath;
			material->GetTexture(aiTextureType_DIFFUSE, 0, &textureFilePath);
			modelData_->material.textureFilePath = directoryPath + "/" + textureFilePath.C_Str();
			break;
		}
		else {
			modelData_->material.textureFilePath = "./resources/white.png";
		}
	}

	modelData_->rootNode = ReadNode(scene->mRootNode);

}

void SkinModel::LoadSkinCluster(aiMesh* mesh)
{

	//SkinCluster構築用のデータを取得
	for (uint32_t boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex) {
		aiBone* bone = mesh->mBones[boneIndex];
		std::string jointName = bone->mName.C_Str();
		JointWeightData& jointWeightData = modelData_->skinClusterData[jointName];

		aiMatrix4x4 bindPoseMatrixAssimp = bone->mOffsetMatrix.Inverse();	//BindPoseMatrixに戻す
		aiVector3D scale, translate;
		aiQuaternion rotate;
		bindPoseMatrixAssimp.Decompose(scale, rotate, translate);	//成分を抽出
		//左手系のBindPoseMatrixを作る
		Matrix4x4 bindPoseMatrix = MakeAffineMatrix(
			{ scale.x, scale.y, scale.z }, { rotate.x, -rotate.y, -rotate.z, rotate.w }, { -translate.x, translate.y, translate.z });
		//InverseBindPoseMatrixにする
		jointWeightData.inverseBindPoseMatrix = Inverse(bindPoseMatrix);

		for (uint32_t weightIndex = 0; weightIndex < bone->mNumWeights; ++weightIndex) {
			jointWeightData.vertexWeights.push_back({ bone->mWeights[weightIndex].mWeight, bone->mWeights[weightIndex].mVertexId });
		}

	}
}

