#include "SkinModel.h"
#include "ModelPlatform.h"
#include "Matrix.h"
#include "Animation.h"
#include "Camera.h"

SkinModel::~SkinModel()
{
}

void SkinModel::CreateModel(const std::string& directoryPath, const std::string& filename)
{

	BaseModel::CreateModel(directoryPath, filename);

	CreateSkelton();

	CreateSkinCluster();

}

void SkinModel::Update()
{

	SkeletonUpdate();

	SkinClusterUpdate();

}

void SkinModel::Update(Animation* animation)
{

	ApplyAnimation(animation);

	Update();

}

void SkinModel::Draw(const WorldTransform& worldTransform, Camera* camera)
{

	Matrix4x4 worldViewProjectionMatrix;
	if (camera) {
		const Matrix4x4& viewProjectionMatrix = camera->GetViewProjection();
		worldViewProjectionMatrix = Multiply(worldTransform.worldMatrix_, viewProjectionMatrix);
	}
	else {
		worldViewProjectionMatrix = worldTransform.worldMatrix_;
	}
	/*
	transformationMatrixData_->WVP = worldViewProjectionMatrix;
	transformationMatrixData_->World = worldMatrix;

	//wvp用のCBufferの場所を設定
	modelPlatform_->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(1, transformationMatrixResource_->GetGPUVirtualAddress());
	*/

	modelPlatform_->ModelDraw(worldViewProjectionMatrix, worldTransform.worldMatrix_, camera);

	std::array<D3D12_VERTEX_BUFFER_VIEW, 2> vbvs = {
			vertexBufferView_,
			skinCluster_.influenceBufferView
	};

	modelPlatform_->GetDxCommon()->GetCommandList()->IASetVertexBuffers(0, 2, &vbvs.front());	//VBVを設定

	modelPlatform_->GetDxCommon()->GetCommandList()->SetGraphicsRootDescriptorTable(4, skinCluster_.paletteSrvHandle.second);


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

void SkinModel::Draw(const WorldTransform& worldTransform, Camera* camera, uint32_t textureHandle)
{

	Matrix4x4 worldViewProjectionMatrix;
	if (camera) {
		const Matrix4x4& viewProjectionMatrix = camera->GetViewProjection();
		worldViewProjectionMatrix = Multiply(worldTransform.worldMatrix_, viewProjectionMatrix);
	}
	else {
		worldViewProjectionMatrix = worldTransform.worldMatrix_;
	}
	/*
	transformationMatrixData_->WVP = worldViewProjectionMatrix;
	transformationMatrixData_->World = worldMatrix;

	//wvp用のCBufferの場所を設定
	modelPlatform_->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(1, transformationMatrixResource_->GetGPUVirtualAddress());
	*/

	modelPlatform_->ModelDraw(worldViewProjectionMatrix, worldTransform.worldMatrix_, camera);

	std::array<D3D12_VERTEX_BUFFER_VIEW, 2> vbvs = {
			vertexBufferView_,
			skinCluster_.influenceBufferView
	};

	modelPlatform_->GetDxCommon()->GetCommandList()->IASetVertexBuffers(0, 2, &vbvs.front());	//VBVを設定

	modelPlatform_->GetDxCommon()->GetCommandList()->SetGraphicsRootDescriptorTable(4, skinCluster_.paletteSrvHandle.second);


	modelPlatform_->GetDxCommon()->GetCommandList()->IASetIndexBuffer(&indexBufferView_);
	//マテリアルのCBufferの場所を設定
	modelPlatform_->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	//SRVのDescriptorTableの先頭を設定。2はrootParameter[2]である。
	TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(textureHandle);

	//描画1(DrawCall/ドローコール)。3頂点で1つのインスタンス。インスタンスについては今後
		//commandList_->DrawIndexedInstanced((kSubdivision * kSubdivision * 6), 1, 0, 0, 0);
	//modelPlatform_->GetDxCommon()->GetCommandList()->DrawInstanced(UINT(modelData_.vertices.size()), 1, 0, 0);
	modelPlatform_->GetDxCommon()->GetCommandList()->DrawIndexedInstanced(UINT(modelData_.indeces.size()), 1, 0, 0, 0);

}

void SkinModel::BoneDraw(const EulerTransform& transform, Camera* camera)
{

	Matrix4x4 worldMatrix = MakeAffineMatrix(transform.scale, transform.rotation, transform.translation);

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

void SkinModel::JointDraw(const EulerTransform& transform, Camera* camera)
{

	Matrix4x4 worldMatrix = MakeAffineMatrix(transform.scale, transform.rotation, transform.translation);

	//全てのJointを更新。親が若いので通常ループで処理可能になっている
	for (Joint& joint : skeleton_.joints) {

		Matrix4x4 jointWorldMatrix;

		jointWorldMatrix = joint.skeletonSpaceMatrix * worldMatrix;

		modelPlatform_->SphereDraw(jointWorldMatrix, camera);
	}

}

void SkinModel::CreateSkinCluster()
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

void SkinModel::LoadModelFile(const std::string& directoryPath, const std::string& filename)
{
	Assimp::Importer importer;
	std::string filepath = directoryPath + "/" + filename;
	const aiScene* scene = importer.ReadFile(filepath.c_str(), aiProcess_FlipWindingOrder | aiProcess_FlipUVs);

	assert(scene->HasMeshes());	//メッシュがないのは対応しない

	for (uint32_t meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex) {
		aiMesh* mesh = scene->mMeshes[meshIndex];
		assert(mesh->HasNormals());	//法線がないメッシュは今回は非対応
		assert(mesh->HasTextureCoords(0));	//TexcoordがないMeshは今回は非対応
		modelData_.vertices.resize(mesh->mNumVertices);	//最初に頂点数分のメモリを確保しておく

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
			modelData_.material.textureFilePath = directoryPath + "/" + textureFilePath.C_Str();
			break;
		}
		else {
			modelData_.material.textureFilePath = "./resources/white.png";
		}
	}

	modelData_.rootNode = ReadNode(scene->mRootNode);

}

void SkinModel::LoadSkinCluster(aiMesh* mesh)
{

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
			{ scale.x, scale.y, scale.z }, { -rotate.x, rotate.y, rotate.z, rotate.w }, { -translate.x, translate.y, translate.z });
		//InverseBindPoseMatrixにする
		jointWeightData.inverseBindPoseMatrix = Inverse(bindPoseMatrix);

		for (uint32_t weightIndex = 0; weightIndex < bone->mNumWeights; ++weightIndex) {
			jointWeightData.vertexWeights.push_back({ bone->mWeights[weightIndex].mWeight, bone->mWeights[weightIndex].mVertexId });
		}

	}
}

void SkinModel::ApplyAnimation(Animation* animation)
{
	for (Joint& joint : skeleton_.joints) {
		//対象のJointのAnimationがあれば、値の適用を行う。下記のif文はC++17から可能になった初期化付きif文。
		if (auto it = animation->GetNodeAnimations().find(joint.name); it != animation->GetNodeAnimations().end()) {
			const NodeAnimation& rootNodeAnimation = (*it).second;
			joint.transform.translation = animation->CalculateValue(rootNodeAnimation.translate.keyframes, animation->GetAnimationTime());
			joint.transform.rotation = animation->CalculateValue(rootNodeAnimation.rotate.keyframes, animation->GetAnimationTime());
			joint.transform.scale = animation->CalculateValue(rootNodeAnimation.scale.keyframes, animation->GetAnimationTime());

		}
	}
}

void SkinModel::SkeletonUpdate()
{
	//全てのJointを更新。親が若いので通常ループで処理可能になっている
	for (Joint& joint : skeleton_.joints) {
		joint.localMatrix = MakeAffineMatrix(joint.transform.scale, joint.transform.rotation, joint.transform.translation);
		if (joint.parent) {	//親がいれば親の行列を掛ける
			joint.skeletonSpaceMatrix = joint.localMatrix * skeleton_.joints[*joint.parent].skeletonSpaceMatrix;
		}
		else {	//親がいないのでlocalMatrixとskeltonSpaceMatrixは一致する
			joint.skeletonSpaceMatrix = joint.localMatrix;
		}
	}
}

void SkinModel::SkinClusterUpdate()
{

	for (size_t jointIndex = 0; jointIndex < skeleton_.joints.size(); ++jointIndex) {
		assert(jointIndex < skinCluster_.inverseBindPoseMatrices.size());
		skinCluster_.mappedPalette[jointIndex].skeletonSpaceMatrix =
			skinCluster_.inverseBindPoseMatrices[jointIndex] * skeleton_.joints[jointIndex].skeletonSpaceMatrix;
		skinCluster_.mappedPalette[jointIndex].skeletonSpaceInverseTransposeMatrix =
			Transpose(Inverse(skinCluster_.mappedPalette[jointIndex].skeletonSpaceMatrix));
	}

}

void SkinModel::CreateSkelton()
{

	skeleton_.root = CreateJoint(modelData_.rootNode, {});

	//名前とindexのマッピングを行いアクセスしやすくする
	for (const Joint& joint : skeleton_.joints) {
		skeleton_.jointMap.emplace(joint.name, joint.index);
	}

	SkeletonUpdate();

}

int32_t SkinModel::CreateJoint(const Node& node, const std::optional<int32_t>& parent)
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
