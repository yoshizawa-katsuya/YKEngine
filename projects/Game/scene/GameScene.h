#pragma once
#include <cstdlib>
#include <memory>
#include <map>
#include <vector>
#include "BaseScene.h"
#include "Audio.h"
#include "Input.h"
#include "ModelPlatform.h"
#include "SpritePlatform.h"
#include "BaseModel.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Sprite.h"
#include "Player.h"
#include "Camera.h"
#include "DebugCamera.h"
#include "ParticleEmitter.h"
#include "MapChipField.h"
#include "InstancingObjects.h"
#include "SelectScene.h"
#include "Rigid3dObject.h"
#include "Stone.h"
#include "Star.h"

class GameScene : public BaseScene
{
public:

	~GameScene() override;

	void Initialize() override;

	void Update() override;

	void Draw() override;

	void Finalize() override;

	void GenerateObjects();

	bool isItemBetween(const Stone& stone1, const Stone& stone2, const Star& item);
private:

	//デバイス
	DirectXCommon* dxCommon_;

	Audio* audio_;
	Input* input_;

	//LoopSoundData bgm1_;

	SpritePlatform* spritePlatform_;
	ModelPlatform* modelPlatform_;

	Camera* mainCamera_ = nullptr;


	std::unique_ptr<Camera> camera_;
	std::unique_ptr<Camera> camera2_;

	std::unique_ptr<DebugCamera> debugCamera_;

	bool isActiveDebugCamera_ = false;

	//平行光源
	std::unique_ptr<DirectionalLight> directionalLight_;

	//点光源
	std::unique_ptr<PointLight> pointLight_;

	//スポットライト
	std::unique_ptr<SpotLight> spotLight_;

	//モデル
	std::unique_ptr<BaseModel> modelPlayer_;
	std::unique_ptr<BaseModel> modelBox_;
	std::unique_ptr<BaseModel> modelFloor_;
	std::unique_ptr<BaseModel> modelstone_;
	std::unique_ptr<BaseModel> modelstar_;
	std::unique_ptr<BaseModel> modelhole_;
	std::unique_ptr<BaseModel> modelice_;
	//
	

	uint32_t textureHandle_;
	//std::unique_ptr<Sprite> sprite_;

	//マップチップ
	std::unique_ptr<MapChipField> mapChipField_;

	//プレイヤー
	std::unique_ptr<Player> player_;

	//ボックス
	std::vector<std::vector<std::unique_ptr<Base3dObject>>> boxes_;

	//床
	std::unique_ptr<InstancingObject> floors_;

	//stone
	std::vector<std::unique_ptr<Stone>> stones_;

	//アイテム
	std::vector<std::unique_ptr<Star>>star_;
	
	//BackGround
	uint32_t background_;
	std::unique_ptr<Sprite> backgroundSprite_;

	SelectScene* selectScene_ = nullptr;
	uint32_t selectedTutorial_ = 0;
	uint32_t selectedBundle_ = 0;
	uint32_t selectedStage_ = 0;
	//パーティクル
	/*
	std::unique_ptr<ParticleEmitter> emitter_;
	std::unique_ptr<AccelerationField> field_;
	Vector4 color_ = {1.0f, 1.0f, 1.0f, 1.0f};
	*/

	void CreateObject(std::unique_ptr<Base3dObject>& object, BaseModel* model, const Vector3& position, const Vector3& scale);
	void AddToInstancing(InstancingObject* instancingObject, const Vector3& position);
	Vector3 ConvertScreenToWorld(const Vector2& screenPos);
};