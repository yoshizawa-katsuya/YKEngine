#include "StageObjects.h"
#include "ModelPlatform.h"
#include "RootParams.h"
#include "GlobalVariables.h"
#include "JsonKeys.h"
#include "manager/CollisionManager.h"

using namespace YKEngine;

void StageObjects::Initialize(StageType stageType)
{
	ModelPlatform* modelPlatform = ModelPlatform::GetInstance();

	//ステージタイプの設定
	stageType_ = stageType;

	//グローバル変数に登録
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const std::string& groupName = JsonKey::StageObjects::kGroupName;
	globalVariables->CreateGroup(groupName);
	globalVariables->AddItem(groupName, JsonKey::StageObjects::kNightSkyBoxColor, Color(0.5f, 0.5f, 0.5f, 1.0f));
	globalVariables->AddItem(groupName, JsonKey::StageObjects::kDayTimeSkyBoxColor, Color(1.0f, 1.0f, 1.0f, 1.0f));
	globalVariables->AddItem(groupName, JsonKey::StageObjects::kGameOverSkyBoxColor, Color(0.5f, 0.5f, 0.5f, 1.0f));
	globalVariables->AddItem(groupName, JsonKey::StageObjects::kGroundEnvironmentCoefficient, 0.5f);
	globalVariables->AddItem(groupName, JsonKey::StageObjects::kGameOverGroundEnvironmentCoefficient, 0.05f);
	globalVariables->AddItem(groupName, JsonKey::StageObjects::kEnvironmentCoefficient, 0.5f);
	globalVariables->AddItem(groupName, JsonKey::StageObjects::kGameOverEnvironmentCoefficient, 0.05f);
	globalVariables->AddItem(groupName, JsonKey::StageObjects::kGroundAlpha, 0.5f);
	globalVariables->AddItem(groupName, JsonKey::StageObjects::kGroundScale, Vector3(100.0f, 1.0f, 100.0f));
	globalVariables->AddItem(groupName, JsonKey::StageObjects::kSkyBoxScale, 1000.0f);

	//ステージタイプごとの初期化処理をマップで管理
	const std::unordered_map<StageType, std::function<void()>> stageTypeInitializationMap = {
		{ 
			StageType::kDefault, [this, modelPlatform]() { 
				textureHandleSkyBox_ = TextureManager::GetInstance()->Load("./Resources/skyBox/night.dds");
				skyBoxModel_ = modelPlatform->CreateSkyBox(textureHandleSkyBox_, "Default"); 
			}
		},
		{ 
			StageType::kClear, [this, modelPlatform]() {
				textureHandleSkyBox_ = TextureManager::GetInstance()->Load("./Resources/skyBox/daytime.dds");
				skyBoxModel_ = modelPlatform->CreateSkyBox(textureHandleSkyBox_, "Clear");
			} 
		},
		{ 
			StageType::kGameOver, [this, modelPlatform]() { 
				textureHandleSkyBox_ = TextureManager::GetInstance()->Load("./Resources/skyBox/night.dds");
				skyBoxModel_ = modelPlatform->CreateSkyBox(textureHandleSkyBox_, "GameOver");
			} 
		},
	};

	//テクスチャの読み込み、スカイボックスモデルの生成
	stageTypeInitializationMap.at(stageType_)();

	//スカイボックスの生成
	skyBox_ = std::make_unique<My3dObject>();
	skyBox_->Initialize(skyBoxModel_.get());
	WorldTransform skyBoxTransform;
	skyBoxTransform.Initialize();
	const float kSkyBoxScale = globalVariables->GetFloatValue(groupName, JsonKey::StageObjects::kSkyBoxScale);
	skyBoxTransform.scale_ = { kSkyBoxScale, kSkyBoxScale, kSkyBoxScale };
	skyBoxTransform.UpdateMatrix();
	skyBox_->WorldTransformUpdate(skyBoxTransform);

	//地面モデルの生成
	groundModel_ = modelPlatform->CreateRigidModel("./Resources/ground", "Ground.obj");
	groundModel_->SetAlpha(globalVariables->GetFloatValue(groupName, JsonKey::StageObjects::kGroundAlpha));

	//地面の生成
	ground_ = std::make_unique<My3dObject>();
	ground_->Initialize(groundModel_.get());
	WorldTransform groundTransform;
	groundTransform.Initialize();
	groundTransform.scale_ = globalVariables->GetVector3Value(groupName, JsonKey::StageObjects::kGroundScale);
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
				factoryMap.at(key)(this, objectData, modelName, stageType_);
			}
			else
			{
				//ファクトリマップに存在しない場合のデフォルトの処理（必要に応じて変更）
				instancingObjects_.emplace(key, std::make_unique<InstancingObjects>());
				BaseModel* model = modelPlatform->CreateRigidModel(objectData.filePath, key).get();
				model->SetEnvironmentCoefficient(GetEnvironmentCoefficient());
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

	const std::unordered_map<StageType, std::function<void()>> stageTypeColorMap = {
		{ 
			StageType::kDefault, [this, globalVariables, groupName, &color]() {
				color = globalVariables->GetColorValue(groupName, JsonKey::StageObjects::kNightSkyBoxColor);
			} 
		},
		{ 
			StageType::kClear, [this, globalVariables, groupName, &color]() {
				color = globalVariables->GetColorValue(groupName, JsonKey::StageObjects::kDayTimeSkyBoxColor);
			} 
		},
		{ 
			StageType::kGameOver, [this, globalVariables, groupName, &color]() {
				color = globalVariables->GetColorValue(groupName, JsonKey::StageObjects::kGameOverSkyBoxColor);
			} 
		}
	};

	stageTypeColorMap.at(stageType_)();

	Vector4 colorVec4 = Vector4(color.r, color.g, color.b, color.a);
	skyBoxModel_->SetColor(colorVec4);

	//地面の環境光係数を設定
	if (stageType_ == StageType::kGameOver)
	{
		//ゲームオーバー時の環境光係数を設定
		groundModel_->SetEnvironmentCoefficient(globalVariables->GetFloatValue(groupName, JsonKey::StageObjects::kGameOverGroundEnvironmentCoefficient));
	}
	else
	{
		//通常時の環境光係数を設定
		groundModel_->SetEnvironmentCoefficient(globalVariables->GetFloatValue(groupName, JsonKey::StageObjects::kGroundEnvironmentCoefficient));
	}
}

float StageObjects::GetEnvironmentCoefficient() const
{
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const std::string& groupName = JsonKey::StageObjects::kGroupName;
	//ステージタイプに応じて環境光係数を取得
	if (stageType_ == StageType::kGameOver)
	{
		return globalVariables->GetFloatValue(groupName, JsonKey::StageObjects::kGameOverEnvironmentCoefficient);
	}
	
	return globalVariables->GetFloatValue(groupName, JsonKey::StageObjects::kEnvironmentCoefficient);
}

const std::unordered_map<std::string, StageObjects::InstancingObjectsFactory>& StageObjects::GetInstancingObjectsFactoryMap() const
{
	static const std::unordered_map<std::string, InstancingObjectsFactory> instancingObjectsFactoryMap = {
		{"primitiveCube", [](StageObjects* stageObjects, const ObjectData& objectData, const std::string& modelName, StageType stageType) {
			//インスタンシングオブジェクトの初期化
			stageObjects->instancingTriplanarObjects_.emplace(objectData.fileName, std::make_unique<InstancingObjects>());
			uint32_t textureHandle = TextureManager::GetInstance()->Load("./Resources/brick.png");
			BaseModel* model = ModelPlatform::GetInstance()->CreateCube(textureHandle, modelName).get();
			//マテリアルの設定
			model->SetEnvironmentCoefficient(stageObjects->GetEnvironmentCoefficient());
			//TODO:インスタンシングのmaxInstanceCountをオブジェクトの数に応じて変更する
			stageObjects->instancingTriplanarObjects_[objectData.fileName]->Initialize(model, 256);

		}},
		{"primitiveSphere", [](StageObjects* stageObjects, const ObjectData& objectData, const std::string& modelName, StageType stageType) {
			//インスタンシングオブジェクトの初期化	
			stageObjects->instancingTriplanarObjects_.emplace(objectData.fileName, std::make_unique<InstancingObjects>());
			uint32_t textureHandle = TextureManager::GetInstance()->Load("./Resources/gradation.png");
			BaseModel* model = ModelPlatform::GetInstance()->CreateSphere(textureHandle, modelName).get();
			//マテリアルの設定
			model->SetEnvironmentCoefficient(stageObjects->GetEnvironmentCoefficient());
			stageObjects->instancingTriplanarObjects_[objectData.fileName]->Initialize(model, 128);

		}},
		{"Sun.obj", [](StageObjects* stageObjects, const ObjectData& objectData, const std::string& modelName, StageType stageType) {
			//インスタンシングオブジェクトの初期化
			stageObjects->instancingObjects_.emplace(objectData.fileName, std::make_unique<InstancingObjects>());
			BaseModel* model = ModelPlatform::GetInstance()->CreateRigidModel(objectData.filePath, objectData.fileName).get();
			//マテリアルの設定
			model->SetShininess(10.0f);
			stageObjects->instancingObjects_[objectData.fileName]->Initialize(model, 128);

		}},
		{"gableRoof.obj", [](StageObjects* stageObjects, const ObjectData& objectData, const std::string& modelName, StageType stageType) {
			//インスタンシングオブジェクトの初期化
			stageObjects->instancingTriplanarObjects_.emplace(objectData.fileName, std::make_unique<InstancingObjects>());
			BaseModel* model = ModelPlatform::GetInstance()->CreateRigidModel(objectData.filePath, objectData.fileName).get();
			//マテリアルの設定
			model->SetEnvironmentCoefficient(stageObjects->GetEnvironmentCoefficient());
			stageObjects->instancingTriplanarObjects_[objectData.fileName]->Initialize(model, 128);
		}}
	};

	return instancingObjectsFactoryMap;
}
