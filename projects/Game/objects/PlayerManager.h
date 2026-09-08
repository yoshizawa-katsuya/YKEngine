#pragma once
#include "LeftPlayer.h"
#include "RightPlayer.h"
#include "Laser.h"
#include "Sprite.h"

class PlayerManager
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera">カメラ</param>
	void Draw(YKEngine::Camera* camera);

	/// <summary>
	/// HUDの描画
	/// </summary>
	void DrawHUD();

	int32_t GetHp() const { return hp_; }

	const LeftPlayer* GetLeftPlayer() const { return leftPlayer_.get(); }
	const RightPlayer* GetRightPlayer() const { return rightPlayer_.get(); }

private:

	/// <summary>
	///	HUDの初期化
	/// </summary>
	void HUDInitialize();

	//プレイヤー
	std::unique_ptr<LeftPlayer> leftPlayer_;
	std::unique_ptr<RightPlayer> rightPlayer_;

	//レーザー
	std::unique_ptr<Laser> laser_;

	int32_t hp_ = 5;
	const int32_t kMaxHp_ = 5;

	std::vector<std::unique_ptr<YKEngine::Sprite>> hpSprites_;	// 体力表示用のスプライト
	std::vector<std::unique_ptr<YKEngine::Sprite>> hpBackSprites_;	// 体力表示用の背景スプライト
};

