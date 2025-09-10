#pragma once
#include <cstdlib>
#include <memory>
#include <map>
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
#include "InstancingObjects.h"
#include "RigidModel.h"
#include "MapChipField.h"
#include "CameraController.h"
#include "Fade.h"
#include "Goal.h"

#include "GimmickManager.h"

class GameScene : public BaseScene
{
public:

	~GameScene() override;

	void Initialize() override;

	void Update() override;

	void Draw() override;

	void Finalize() override;

	//ステージ番号
	static uint32_t stageNum_;

private:
	void CreateLevel();

	void UpdateStart();

	void UpdateMain();

	void UpdateGameClear();

	void UpdateGameOver();

	bool CheckElectricCollision(MapChipType type);

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

	std::shared_ptr<BaseModel> modelPlayer_;
	std::shared_ptr<BaseModel> modelBlock_;
	std::shared_ptr<BaseModel> modelSpine_;
	std::shared_ptr<BaseModel> modelGoal_;
	std::shared_ptr<BaseModel>modelElectric_;

	uint32_t textureHandle_;
	uint32_t textureHandle2_;
	//std::unique_ptr<Sprite> sprite_;

	//プレイヤー
	std::unique_ptr<Player> player_;

	// マップチップブロック

	//ブロック
	std::unique_ptr<InstancingObjects> blocks_;

	//設置型トゲ
	std::unique_ptr<InstancingObjects> trapSpines_;

	
	//ゴール
	std::unique_ptr<Goal> goal_;

	

	//カメラコントローラー
	std::unique_ptr<CameraController> cameraController_;

	//マップチップフィールド
	std::unique_ptr<MapChipField> mapChipField_;
	
	//シーンのフェーズ
	enum class Phase {
		kStart,	//開始部
		kMain,	//メイン部
		kGameClear,	//クリア部
		kGameOver,	//ゲームオーバー部
	};

	//現在のフェーズ
	Phase phase_ = Phase::kStart;

	std::unique_ptr<Fade> fade_;

	std::unique_ptr<GimmickManager> gimmickManager_;
};