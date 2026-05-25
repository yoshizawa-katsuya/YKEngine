#include "StageObjects.h"
#include "ModelPlatform.h"
#include "My3dObject.h"
#include "RootParams.h"
#include "GlobalVariables.h"
#include "JsonKeys.h"
#include "manager/CollisionManager.h"

using namespace YKEngine;

void StageObjects::Initialize(bool isDayTime)
{
	ModelPlatform* modelPlatform = ModelPlatform::GetInstance();

	isDayTime_ = isDayTime;

	//グローバル変数に登録
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const std::string& groupName = JsonKey::StageObjects::kGroupName;
	globalVariables->CreateGroup(groupName);
	globalVariables->AddItem(groupName, JsonKey::StageObjects::kNightSkyBoxColor, Color(0.5f, 0.5f, 0.5f, 1.0f));
	globalVariables->AddItem(groupName, JsonKey::StageObjects::kDayTimeSkyBoxColor, Color(1.0f, 1.0f, 1.0f, 1.0f));
	globalVariables->AddItem(groupName, JsonKey::StageObjects::kGroundEnvironmentCoefficient, 0.5f);


	//テクスチャの読み込み、スカイボックスモデルの生成
	if (isDayTime_)
	{
		textureHandleSkyBox_ = TextureManager::GetInstance()->Load("./Resources/skyBox/daytime.dds");
		skyBoxModel_ = modelPlatform->CreateSkyBox(textureHandleSkyBox_, "DayTime");
	}
	else
	{
		textureHandleSkyBox_ = TextureManager::GetInstance()->Load("./Resources/skyBox/night.dds");
		skyBoxModel_ = modelPlatform->CreateSkyBox(textureHandleSkyBox_, "Night");
	}	

	//スカイボックスの生成
	skyBox_ = std::make_unique<My3dObject>();
	skyBox_->Initialize(skyBoxModel_.get());
	WorldTransform skyBoxTransform;
	skyBoxTransform.Initialize();
	const float kSkyBoxScale = 1000.0f;
	skyBoxTransform.scale_ = { kSkyBoxScale, kSkyBoxScale, kSkyBoxScale };
	skyBoxTransform.UpdateMatrix();
	skyBox_->WorldTransformUpdate(skyBoxTransform);

	//地面モデルの生成
	groundModel_ = modelPlatform->CreateRigidModel("./Resources/ground", "Ground.obj");
	groundModel_->SetAlpha(0.5f);

	//地面の生成
	ground_ = std::make_unique<My3dObject>();
	ground_->Initialize(groundModel_.get());
	WorldTransform groundTransform;
	groundTransform.Initialize();
	groundTransform.scale_ = { 100.0f, 1.0f, 100.0f };
	groundTransform.UpdateMatrix();
	ground_->WorldTransformUpdate(groundTransform);

	//jsonからステージオブジェクトの情報を読み込み
	LoadFromJson();

}

void StageObjects::Update()
{
#ifdef _DEBUG

	//jsonからステージオブジェクトの情報を読み込み
	LoadFromJson();

#endif // _DEBUG

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

	for (const ObjectData& objectData : objectDatas)
	{

		std::string key = objectData.fileName;

		//インスタンスオブジェクトが存在しない場合は生成
		if (!instancingObjects_.contains(key) && !instancingTriplanarObjects_.contains(key))
		{
			const std::string modelName = "decoreation";

			const std::unordered_map<std::string, InstancingObjectsFactory>& factoryMap = GetInstancingObjectsFactoryMap();

			if (factoryMap.contains(key))
			{
				factoryMap.at(key)(this, objectData, modelName);
			}
			else
			{
				//ファクトリマップに存在しない場合のデフォルトの処理（必要に応じて変更）
				instancingObjects_.emplace(key, std::make_unique<InstancingObjects>());
				BaseModel* model = modelPlatform->CreateRigidModel(objectData.filePath, key).get();
				model->SetEnvironmentCoefficient(0.5f);
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
		if (instancingObjects_.contains(key))
		{
			instancingObjects_[key]->WorldTransformUpdate(transform);
		}
		else if (instancingTriplanarObjects_.contains(key))
		{
			instancingTriplanarObjects_[key]->WorldTransformUpdate(transform);
		}

		if (key == "primitiveCube" && objectData.hasCollider)
		{
			//障害物の生成
			std::unique_ptr<CuboidObstacle> cuboidObstacle = std::make_unique<CuboidObstacle>();
			cuboidObstacle->Initialize(transform);
			cuboidObstacles_.push_back(std::move(cuboidObstacle));

			CollisionManager::GetInstance()->AddOBBCollider(cuboidObstacles_.back().get());
		}
	}
}

void StageObjects::InstancingDraw(YKEngine::Camera* camera)
{
	TextureManager::GetInstance()->SetEnvironmentMap(static_cast<size_t>(ModelRootParam::kEnvironmentMap), textureHandleSkyBox_);

	for (const auto& [name, instancingObject] : instancingObjects_)
	{
		instancingObject->CameraUpdate(camera);
		instancingObject->Draw();
	}
}

void StageObjects::InstancingTriplanarDraw(YKEngine::Camera* camera)
{
	TextureManager::GetInstance()->SetEnvironmentMap(static_cast<size_t>(ModelRootParam::kEnvironmentMap), textureHandleSkyBox_);

	for (const auto& [name, instancingObject] : instancingTriplanarObjects_)
	{
		instancingObject->CameraUpdate(camera);
		instancingObject->Draw();
	}
}

void StageObjects::LoadFromJson()
{
	// グローバル変数からステージオブジェクトの情報を取得
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const std::string& groupName = JsonKey::StageObjects::kGroupName;

	Color color;
	if (isDayTime_)
	{
		color = globalVariables->GetColorValue(groupName, JsonKey::StageObjects::kDayTimeSkyBoxColor);
	}
	else
	{
		color = globalVariables->GetColorValue(groupName, JsonKey::StageObjects::kNightSkyBoxColor);
	}
	Vector4 colorVec4 = Vector4(color.r, color.g, color.b, color.a);
	skyBoxModel_->SetColor(colorVec4);

	groundModel_->SetEnvironmentCoefficient(globalVariables->GetFloatValue(groupName, JsonKey::StageObjects::kGroundEnvironmentCoefficient));
}

const std::unordered_map<std::string, StageObjects::InstancingObjectsFactory>& StageObjects::GetInstancingObjectsFactoryMap() const
{
	static const std::unordered_map<std::string, InstancingObjectsFactory> instancingObjectsFactoryMap = {
		{"primitiveCube", [](StageObjects* stageObjects, const ObjectData& objectData, const std::string& modelName) {
			//インスタンシングオブジェクトの初期化
			stageObjects->instancingTriplanarObjects_.emplace(objectData.fileName, std::make_unique<InstancingObjects>());
			uint32_t textureHandle = TextureManager::GetInstance()->Load("./Resources/brick.png");
			BaseModel* model = ModelPlatform::GetInstance()->CreateCube(textureHandle, modelName).get();
			model->SetEnvironmentCoefficient(0.5f);
			stageObjects->instancingTriplanarObjects_[objectData.fileName]->Initialize(model, 256);

		}},
		{"primitiveSphere", [](StageObjects* stageObjects, const ObjectData& objectData, const std::string& modelName) {
			//インスタンシングオブジェクトの初期化	
			stageObjects->instancingTriplanarObjects_.emplace(objectData.fileName, std::make_unique<InstancingObjects>());
			uint32_t textureHandle = TextureManager::GetInstance()->Load("./Resources/gradation.png");
			BaseModel* model = ModelPlatform::GetInstance()->CreateSphere(textureHandle, modelName).get();
			model->SetEnvironmentCoefficient(0.5f);
			stageObjects->instancingTriplanarObjects_[objectData.fileName]->Initialize(model, 128);

		}},
		{"Sun.obj", [](StageObjects* stageObjects, const ObjectData& objectData, const std::string& modelName) {
			//インスタンシングオブジェクトの初期化
			stageObjects->instancingObjects_.emplace(objectData.fileName, std::make_unique<InstancingObjects>());
			BaseModel* model = ModelPlatform::GetInstance()->CreateRigidModel(objectData.filePath, objectData.fileName).get();
			//マテリアルの設定
			model->SetShininess(10.0f);
			stageObjects->instancingObjects_[objectData.fileName]->Initialize(model, 128);

		}}
	};

	return instancingObjectsFactoryMap;
}
