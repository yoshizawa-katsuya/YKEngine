#include "SkinModel.h"
#include "ModelPlatform.h"
#include "Matrix.h"
#include "Animation.h"

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
