#include "SkinModel.h"
#include "ModelPlatform.h"
#include "Matrix.h"
#include "Animation.h"
#include "Camera.h"
#include "RootParams.h"

SkinModel::~SkinModel()
{
}

void SkinModel::Draw(bool usedMaterial)
{

	modelPlatform_->GetDxCommon()->GetCommandList()->IASetIndexBuffer(&indexBufferView_);
	if (!usedMaterial)
	{
		//マテリアルのCBufferの場所を設定
		modelPlatform_->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(static_cast<size_t>(SkinModelRootParam::kMaterial), materialResource_->GetGPUVirtualAddress());

	}
	//テクスチャハンドルを設定
	TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(static_cast<uint32_t>(SkinModelRootParam::kTexture), textureHandle_);

	//描画1(DrawCall/ドローコール)。	
	modelPlatform_->GetDxCommon()->GetCommandList()->DrawIndexedInstanced(indecesNum_, 1, 0, 0, 0);
}

void SkinModel::Draw(uint32_t textureHandle, bool usedMaterial)
{

	modelPlatform_->GetDxCommon()->GetCommandList()->IASetIndexBuffer(&indexBufferView_);
	if (!usedMaterial)
	{
		//マテリアルのCBufferの場所を設定
		modelPlatform_->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(static_cast<size_t>(SkinModelRootParam::kMaterial), materialResource_->GetGPUVirtualAddress());

	}
	//テクスチャハンドルを設定
	TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(static_cast<uint32_t>(SkinModelRootParam::kTexture), textureHandle);

	//描画1(DrawCall/ドローコール)。
	modelPlatform_->GetDxCommon()->GetCommandList()->DrawIndexedInstanced(indecesNum_, 1, 0, 0, 0);

}

void SkinModel::SetSkinCluster(const SkinCluster& skinCluster)
{
	//頂点バッファビュー配列を作成
	std::array<D3D12_VERTEX_BUFFER_VIEW, 2> vbvs = {
			vertexBufferView_,
			skinCluster.influenceBufferView
	};

	modelPlatform_->GetDxCommon()->GetCommandList()->IASetVertexBuffers(0, 2, &vbvs.front());	//VBVを設定

	modelPlatform_->GetDxCommon()->GetCommandList()->SetGraphicsRootDescriptorTable(9, skinCluster.paletteSrvHandle.second);

}

void SkinModel::LoadMeshData(aiMesh* mesh)
{
	BaseModel::LoadMeshData(mesh);

	LoadSkinCluster(mesh);

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

