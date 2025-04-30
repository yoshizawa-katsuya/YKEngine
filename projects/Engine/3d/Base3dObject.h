#pragma once
#include "BaseModel.h"

class Base3dObject
{
public:

	Base3dObject();

	virtual ~Base3dObject() = 0;

	virtual void Initialize(BaseModel* model);

	//virtual void Update(const WorldTransform& worldTransform, Camera* camera);

	//virtual void Update(const WorldTransform& worldTransform, Camera* camera, Animation* animation);

	virtual void WorldTransformUpdate(const WorldTransform& worldTransform);

	//WorldTransformUpdateの後に実行する
	virtual void AnimationUpdate(Animation* animation);

	virtual void CameraUpdate(Camera* camera);

	virtual void Draw();

	virtual void Draw(uint32_t textureHandle);

	void SetUVTransform(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

	void SetUVTransform(const EulerTransform& uvTransform);

	void SetEnableLighting(bool enableLighting);

	const BaseModel& GetModel() const { return *model_; }
	BaseModel& GetModel() { return *model_; }

protected:

	//マテリアルデータ作成
	void CreateMaterialData();

	DirectXCommon* dxCommon_;

	//TransformationMatrix用のリソースを作る。モデル用
	Microsoft::WRL::ComPtr<ID3D12Resource> TransformationResource_;
	//データを書き込む
	TransformationMatrix* TransformationData_;
	//Matrix4x4* WVPData_ = nullptr;

	//マテリアル用のリソースを作る
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_;
	//マテリアルにデータを書き込む
	Material* materialData_ = nullptr;

	BaseModel* model_;

};

