#pragma once
#include "BaseScene.h"
#include "DirectXCommon.h"
#include "Audio.h"
#include "Input.h"
#include "TextureManager.h"
#include "ModelPlatform.h"
#include "SpritePlatform.h"
#include "AnimatedSprite.h"
#include "DebugCamera.h"
#include "Camera.h"
#include "Rigid3dObject.h"
#include "RailMover.h"
#include "RailCamera.h"
#include "InstancingObjects.h"

/// <summary>
/// タイトル画面のクラス。
/// </summary>
class TitleScene : public BaseScene
{
public:

	~TitleScene() override;

	void Initialize() override;

	void Update() override;

	void Draw() override;

	void Finalize() override;

private:

	void UpdateStart();

	void UpdateMain();

	void UpdateEnd();

	void CreateLevel();

	//デバイス
	DirectXCommon* dxCommon_;

	Audio* audio_;
	Input* input_;

	SpritePlatform* spritePlatform_;
	ModelPlatform* modelPlatform_;
	
	//平行光源
	std::unique_ptr<DirectionalLight> directionalLight_;

	//カメラ
	Camera* mainCamera_ = nullptr;

	std::unique_ptr<Camera> camera_;
	std::unique_ptr<Camera> camera2_;

	std::unique_ptr<DebugCamera> debugCamera_;

	bool isActiveDebugCamera_ = false;
	std::unique_ptr<Sprite> spriteTitle_;//タイトルのスプライト

	std::unique_ptr<AnimatedSprite> spriteSceneChange_;//シーンチェンジのスプライト

	std::shared_ptr<BaseModel> modelGround_;

	//テクスチャハンドル
	uint32_t textureHandle_;
	uint32_t textureHandleSkyBox_;

	//スカイボックス
	std::unique_ptr<Rigid3dObject> skyBox_;

	//地面
	std::unique_ptr<Rigid3dObject> ground_;

	//レールムーバー
	std::unique_ptr<RailMover> railMover_;

	//レールカメラ
	std::unique_ptr<RailCamera> railCamera_;

	//オブジェクト
	std::map<std::string, std::unique_ptr<InstancingObjects>> instancingObjects_;

	//シーンのフェーズ
	enum class Phase {
		kStart,	//開始部
		kMain,	//メイン部
		kEnd,	//終了部
	};

	//現在のフェーズ
	Phase phase_ = Phase::kStart;

};

