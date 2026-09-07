#include "PlayerManager.h"
#include "ModelPlatform.h"

#ifdef USE_IMGUI
#include "imgui/imgui.h"
#endif // USE_IMGUI

using namespace YKEngine;

void PlayerManager::Initialize()
{
	//モデルの生成
	ModelPlatform* modelPlatform = ModelPlatform::GetInstance();
	std::shared_ptr<BaseModel> modelLeftPlayer = modelPlatform->CreateRigidModel("./resources/leftPlayer", "leftPlayer.obj");
	std::shared_ptr<BaseModel> modelRightPlayer = modelPlatform->CreateRigidModel("./resources/rightPlayer", "rightPlayer.obj");

	//プレイヤーの初期化
	leftPlayer_ = std::make_unique<LeftPlayer>();
	leftPlayer_->Initialize(modelLeftPlayer.get(), &hp_);

	rightPlayer_ = std::make_unique<RightPlayer>();
	rightPlayer_->Initialize(modelRightPlayer.get(), &hp_);

	// プレイヤー同士の参照を設定
	leftPlayer_->SetOtherPlayerWorldTransform(rightPlayer_->GetWorldTransform());
	rightPlayer_->SetOtherPlayerWorldTransform(leftPlayer_->GetWorldTransform());

	//レーザーの初期化
	laser_ = std::make_unique<Laser>();
	laser_->Initialize(leftPlayer_->GetWorldTransform(), rightPlayer_->GetWorldTransform());

	//HUDの初期化
	HUDInitialize();

}

void PlayerManager::Update()
{
#ifdef USE_IMGUI

	ImGui::Begin("Player");
	ImGui::Text("HP: %d", hp_);
	ImGui::End();

#endif // USE_IMGUI

	//プレイヤーの更新
	leftPlayer_->Update();
	rightPlayer_->Update();

	//レーザーの更新
	laser_->Update();
}

void PlayerManager::Draw(Camera* camera)
{
	//プレイヤーの描画
	leftPlayer_->Draw(camera);
	rightPlayer_->Draw(camera);

	//レーザーの描画
	laser_->Draw(camera);
}

void PlayerManager::DrawHUD()
{
	//HPの描画
	for (int32_t i = 0; i < kMaxHp_; ++i) {
		hpBackSprites_[i]->Draw();
		if (i < hp_) 
		{
			hpSprites_[i]->Draw();
		}
	}
}

void PlayerManager::HUDInitialize()
{
	// テクスチャの読み込み
	uint32_t hpTextureHandle = TextureManager::GetInstance()->Load("./resources/heart.png");
	uint32_t hpBackTextureHandle = TextureManager::GetInstance()->Load("./resources/heartFrame.png");

	hpSprites_.reserve(kMaxHp_);
	hpBackSprites_.reserve(kMaxHp_);

	Vector2 heartPosition = { 20.0f, 20.0f }; // ハートの初期位置
	Vector2 heartSize = { 32.0f, 32.0f }; // ハートのサイズ
	float heartSpacing = 10.0f; // ハートの間隔

	for (int32_t i = 0; i < kMaxHp_; ++i) {
		// 背景スプライトの作成
		auto hpBackSprite = std::make_unique<Sprite>();
		hpBackSprite->Initialize(hpBackTextureHandle);
		hpBackSprite->SetPosition(heartPosition);
		hpBackSprite->SetSize(heartSize);
		hpBackSprites_.push_back(std::move(hpBackSprite));
		// ハートスプライトの作成
		auto hpSprite = std::make_unique<Sprite>();
		hpSprite->Initialize(hpTextureHandle);
		hpSprite->SetPosition(heartPosition);
		hpSprite->SetSize(heartSize);
		hpSprites_.push_back(std::move(hpSprite));
		// 次のハートの位置を計算
		heartPosition.x += heartSize.x + heartSpacing;
	}
}
