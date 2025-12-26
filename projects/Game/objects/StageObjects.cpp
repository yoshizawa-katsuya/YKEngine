#include "StageObjects.h"
#include "ModelPlatform.h"

using namespace YKEngine;

void StageObjects::Initialize(const std::vector<ObjectData>& objectDatas)
{
	ModelPlatform* modelPlatform = ModelPlatform::GetInstance();	
	uint32_t textureHandle = TextureManager::GetInstance()->Load("./Resources/white.png");

	for (const ObjectData& objectData : objectDatas)
	{

		std::string key = objectData.fileName;

		//インスタンスオブジェクトが存在しない場合は生成
		if (!instancingObjects_.contains(key))
		{
			instancingObjects_.emplace(key, std::make_unique<InstancingObjects>());
			//インスタンスオブジェクトの初期化
			if (key == "primitiveCube")
			{
				instancingObjects_[key]->Initialize(modelPlatform->CreateCube(textureHandle).get(), 128);
			}
			else if (key == "primitiveSphere")
			{
				instancingObjects_[key]->Initialize(modelPlatform->CreateSphere(textureHandle).get(), 128);
			}
			else if (key == "Sun.obj")
			{
				BaseModel* model = modelPlatform->CreateRigidModel(objectData.filePath, key).get();
				//マテリアルの設定
				model->SetShininess(10.0f);
				instancingObjects_[key]->Initialize(modelPlatform->CreateRigidModel(objectData.filePath, key).get(), 128);
			}
		}
		//ワールド変換の初期化
		WorldTransform transform;
		transform.Initialize();
		transform.rotation_ = objectData.transform.rotation;
		transform.translation_ = objectData.transform.translation;
		transform.scale_ = objectData.transform.scale;
		transform.UpdateMatrix();
		//インスタンスオブジェクトにワールド変換を設定
		instancingObjects_[key]->WorldTransformUpdate(transform);
	}
}

void StageObjects::Draw(YKEngine::Camera* camera)
{
	for (const auto& [name, instancingObject] : instancingObjects_)
	{
		instancingObject->CameraUpdate(camera);
		instancingObject->Draw();
	}
}
