#include "StageObjects.h"
#include "ModelPlatform.h"
#include "My3dObject.h"

using namespace YKEngine;

void StageObjects::Initialize()
{
	ModelPlatform* modelPlatform = ModelPlatform::GetInstance();

	//テクスチャの読み込み
	textureHandleSkyBox_ = TextureManager::GetInstance()->Load("./Resources/skyBox.dds");

	//スカイボックスの生成
	skyBox_ = std::make_unique<My3dObject>();
	skyBox_->Initialize(modelPlatform->CreateSkyBox(textureHandleSkyBox_).get());
	WorldTransform skyBoxTransform;
	skyBoxTransform.Initialize();
	const float kSkyBoxScale = 1000.0f;
	skyBoxTransform.scale_ = { kSkyBoxScale, kSkyBoxScale, kSkyBoxScale };
	skyBoxTransform.UpdateMatrix();
	skyBox_->WorldTransformUpdate(skyBoxTransform);

	//地面モデルの生成
	std::shared_ptr<BaseModel> modelGround = modelPlatform->CreateRigidModel("./Resources/ground", "Ground.obj");
	const float kGroundUVScale = 800.0f;
	modelGround->SetUVTransform({ {kGroundUVScale, kGroundUVScale, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} });
	modelGround->SetEnvironmentCoefficient(0.8f);

	//地面の生成
	ground_ = std::make_unique<My3dObject>();
	ground_->Initialize(modelGround.get());
	WorldTransform groundTransform;
	groundTransform.Initialize();
	groundTransform.scale_ = { 100.0f, 1.0f, 100.0f };
	groundTransform.UpdateMatrix();
	ground_->WorldTransformUpdate(groundTransform);
}

void StageObjects::Draw(YKEngine::Camera* camera)
{
	//地面の描画
	ground_->CameraUpdate(camera);
	ground_->Draw();
}

void StageObjects::DrawSkyBox(YKEngine::Camera* camera)
{
	skyBox_->CameraUpdate(camera);
	skyBox_->Draw();
}

void StageObjects::GetInstancingObject(const std::vector<YKEngine::ObjectData>& objectDatas)
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
				instancingObjects_[key]->Initialize(model, 128);
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

void StageObjects::InstancingDraw(YKEngine::Camera* camera)
{
	for (const auto& [name, instancingObject] : instancingObjects_)
	{
		instancingObject->CameraUpdate(camera);
		instancingObject->Draw();
	}
}
