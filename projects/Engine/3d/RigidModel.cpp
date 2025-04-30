#include "RigidModel.h"
#include "ModelPlatform.h"
#include <numbers>

RigidModel::~RigidModel()
{
}

void RigidModel::CreateSphere(uint32_t textureHandle)
{
	modelData_ = std::make_unique<ModelData>();

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

	modelData_->vertices.resize(kSubdivision * kSubdivision * 4);
	SetVerticesNum();

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

	modelData_->indeces.resize(kSubdivision * kSubdivision * 6);
	SetIndecesNum();

	CreateMaterialData();

	//CreateTransformData();

	textureHandle_ = textureHandle;


}

void RigidModel::CreatePlane(uint32_t textureHandle)
{

	modelData_ = std::make_unique<ModelData>();

	//VertexResourceを生成
	vertexResource_ = modelPlatform_->GetDxCommon()->CreateBufferResource(sizeof(VertexData) * 4);

	//リソースの先頭のアドレスから使う
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点6つ分のサイズ
	vertexBufferView_.SizeInBytes = UINT(sizeof(VertexData) * 4);
	//1頂点当たりのサイズ
	vertexBufferView_.StrideInBytes = sizeof(VertexData);

	//書き込むためのアドレスを取得
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));

	vertexData_[0] = { .position = {1.0f, 1.0f, 0.0f, 1.0f}, .texcoord = {0.0f, 0.0f}, .normal = {0.0f, 0.0f, 1.0f} };	//左上
	vertexData_[1] = { .position = {-1.0f, 1.0f, 0.0f, 1.0f}, .texcoord = {1.0f, 0.0f}, .normal = {0.0f, 0.0f, 1.0f} };	//右上
	vertexData_[2] = { .position = {1.0f, -1.0f, 0.0f, 1.0f}, .texcoord = {0.0f, 1.0f}, .normal = {0.0f, 0.0f, 1.0f} };	//左下
	vertexData_[3] = { .position = {-1.0f, -1.0f, 0.0f, 1.0f}, .texcoord = {1.0f, 1.0f}, .normal = {0.0f, 0.0f, 1.0f} };	//右下

	modelData_->vertices.resize(4);
	SetVerticesNum();
	/*
	vertexData_[0] = { .position = {1.0f, 1.0f, 0.0f, 1.0f}, .texcoord = {0.0f, 0.0f}, .normal = {0.0f, 0.0f, 1.0f} };	//左上
	vertexData_[1] = { .position = {-1.0f, 1.0f, 0.0f, 1.0f}, .texcoord = {1.0f, 0.0f}, .normal = {0.0f, 0.0f, 1.0f} };	//右上
	vertexData_[2] = { .position = {1.0f, -1.0f, 0.0f, 1.0f}, .texcoord = {0.0f, 1.0f}, .normal = {0.0f, 0.0f, 1.0f} };	//左下
	vertexData_[3] = { .position = {1.0f, -1.0f, 0.0f, 1.0f}, .texcoord = {0.0f, 1.0f}, .normal = {0.0f, 0.0f, 1.0f} };	//左下
	vertexData_[4] = { .position = {-1.0f, 1.0f, 0.0f, 1.0f}, .texcoord = {1.0f, 0.0f}, .normal = {0.0f, 0.0f, 1.0f} };	//右上
	vertexData_[5] = { .position = {-1.0f, -1.0f, 0.0f, 1.0f}, .texcoord = {1.0f, 1.0f}, .normal = {0.0f, 0.0f, 1.0f} };	//右下
	*/

	//IndexResources作成
	indexResource_ = modelPlatform_->GetDxCommon()->CreateBufferResource(sizeof(uint32_t) * 6);

	indexBufferView_.BufferLocation = indexResource_->GetGPUVirtualAddress();
	indexBufferView_.SizeInBytes = UINT(sizeof(uint32_t) * 6);
	indexBufferView_.Format = DXGI_FORMAT_R32_UINT;

	indexResource_->Map(0, nullptr, reinterpret_cast<void**>(&indexData_));

	indexData_[0] = 0;	//左上
	indexData_[1] = 1;	//右上
	indexData_[2] = 2;	//左下
	indexData_[3] = 2;	//左下
	indexData_[4] = 1;	//右上
	indexData_[5] = 3;	//右下

	modelData_->indeces.resize(6);
	SetIndecesNum();

	CreateMaterialData();

	textureHandle_ = textureHandle;
}

void RigidModel::CreateRing(uint32_t textureHandle)
{

	modelData_ = std::make_unique<ModelData>();

	//分割数
	const uint32_t kRingDivide = 32;
	//外側の半径
	const float kOuterRadius = 1.0f;
	//内側の半径
	const float kInnerRadius = 0.2f;
	//一欠けらごとの円周
	const float radianPerDivide = 2.0f * std::numbers::pi_v<float> / static_cast<float>(kRingDivide);

	//VertexResourceを生成
	vertexResource_ = modelPlatform_->GetDxCommon()->CreateBufferResource(sizeof(VertexData) * kRingDivide * 4);

	//リソースの先頭のアドレスから使う
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点6つ分のサイズ
	vertexBufferView_.SizeInBytes = UINT(sizeof(VertexData) * kRingDivide * 4);
	//1頂点当たりのサイズ
	vertexBufferView_.StrideInBytes = sizeof(VertexData);

	//書き込むためのアドレスを取得
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));

	indexResource_ = modelPlatform_->GetDxCommon()->CreateBufferResource(sizeof(uint32_t) * kRingDivide * 6);

	//リソースの先頭のアドレスから使う
	indexBufferView_.BufferLocation = indexResource_->GetGPUVirtualAddress();

	indexBufferView_.SizeInBytes = sizeof(uint32_t) * kRingDivide * 6;
	//インデックスはuint32_tとする
	indexBufferView_.Format = DXGI_FORMAT_R32_UINT;

	indexResource_->Map(0, nullptr, reinterpret_cast<void**>(&indexData_));

	for (uint32_t index = 0; index < kRingDivide; ++index)
	{
		uint32_t vertexIndex = index * 4;
		uint32_t indecesIndex = index * 6;

		float sin = std::sin(index * radianPerDivide);
		float cos = std::cos(index * radianPerDivide);
		float sinNext = std::sin((index + 1)* radianPerDivide);
		float cosNext = std::cos((index + 1)* radianPerDivide);
		float u = static_cast<float>(index) / static_cast<float>(kRingDivide);
		float uNext = static_cast<float>(index + 1) / static_cast<float>(kRingDivide);

		vertexData_[vertexIndex] = { .position = {-sin * kOuterRadius, cos * kOuterRadius, 0.0f, 1.0f}, .texcoord = {u, 0.0f}, .normal = {0.0f, 0.0f, 1.0f} };	//左上
		vertexData_[vertexIndex + 1] = { .position = {-sinNext * kOuterRadius, cosNext * kOuterRadius, 0.0f, 1.0f}, .texcoord = {uNext, 0.0f}, .normal = {0.0f, 0.0f, 1.0f} };	//右上
		vertexData_[vertexIndex + 2] = { .position = {-sin * kInnerRadius, cos * kInnerRadius, 0.0f, 1.0f}, .texcoord = {u, 1.0f}, .normal = {0.0f, 0.0f, 1.0f} };	//左下
		vertexData_[vertexIndex + 3] = { .position = {-sinNext * kInnerRadius, cosNext * kInnerRadius, 0.0f, 1.0f}, .texcoord = {uNext, 1.0f}, .normal = {0.0f, 0.0f, 1.0f} };	//右下

		indexData_[indecesIndex] = vertexIndex;
		indexData_[indecesIndex + 1] = vertexIndex + 1;
		indexData_[indecesIndex + 2] = vertexIndex + 2;
		indexData_[indecesIndex + 3] = vertexIndex + 1;
		indexData_[indecesIndex + 4] = vertexIndex + 3;
		indexData_[indecesIndex + 5] = vertexIndex + 2;

	}

	modelData_->vertices.resize(kRingDivide * 4);
	SetVerticesNum();

	modelData_->indeces.resize(kRingDivide * 6);
	SetIndecesNum();

	CreateMaterialData();

	textureHandle_ = textureHandle;
}
