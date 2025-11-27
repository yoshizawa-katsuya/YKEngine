#pragma once
#include "BaseScene.h"
#include "AnimatedSprite.h"
#include "DebugCamera.h"
#include "Rigid3dObject.h"
#include "RailMover.h"
#include "RailCamera.h"
#include "InstancingObjects.h"
#include "DirectionalLight.h"
#include "DemoPlayer.h"
class Input;
class ModelPlatform;

/// <summary>
/// クリア画面を管理するクラス。
/// シーンの開始、更新、描画、終了を行う。
/// </summary>
class ClearScene : public BaseScene
{
public:

	/// <summary>
	/// デストラクタ。
	/// </summary>
	~ClearScene() override;

	/// <summary>
	/// 初期化。
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新。
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画。
	/// </summary>
	void Draw() override;

	/// <summary>
	/// 終了。
	/// </summary>
	void Finalize() override;

private:

	/// <summary>
	/// 開始部の更新。
	/// </summary>
	void UpdateStart();

	/// <summary>
	/// メイン部の更新。
	/// </summary>
	void UpdateMain();

	/// <summary>
	/// 終了部の更新。
	/// </summary>
	void UpdateEnd();

	/// <summary>
	/// レベルの生成。
	/// </summary>
	void CreateLevel();

	//デバイス
	DirectXCommon* dxCommon_;

	Input* input_;

	SpritePlatform* spritePlatform_;
	ModelPlatform* modelPlatform_;

	//平行光源
	DirectionalLight directionalLight_;

	//カメラ
	Camera* mainCamera_ = nullptr;

	std::unique_ptr<Camera> camera_;
	std::unique_ptr<Camera> camera2_;

	std::unique_ptr<DebugCamera> debugCamera_;

	bool isActiveDebugCamera_ = false;

	std::unique_ptr<Sprite> spriteBackGround_;

	std::unique_ptr<AnimatedSprite> spriteSceneChange_;//シーンチェンジのスプライト

	std::shared_ptr<BaseModel> modelGround_;
	std::shared_ptr<BaseModel> modelPlayer_;

	//テクスチャハンドル
	uint32_t textureHandleSkyBox_;

	//スカイボックス
	std::unique_ptr<Rigid3dObject> skyBox_;

	//地面
	std::unique_ptr<Rigid3dObject> ground_;

	//レールムーバー
	std::unique_ptr<RailMover> railMover_;

	//レールカメラ
	std::unique_ptr<RailCamera> railCamera_;

	//デモプレイヤー
	std::unique_ptr<DemoPlayer> demoPlayer_;

	//シーンのフェーズ
	enum class Phase 
	{
		kStart,	//開始部
		kMain,	//メイン部
		kEnd,	//終了部
	};

	//現在のフェーズ
	Phase phase_ = Phase::kStart;

};

