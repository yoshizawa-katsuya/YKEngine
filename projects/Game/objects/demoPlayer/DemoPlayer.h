#pragma once
#include "WorldTransform.h"
#include "Skin3dObject.h"
#include "SRTAnimator.h"
#include "StateMachine.hpp"
#include "DemoPlayerStateContext.h"
#include "Animation.h"

namespace YKEngine
{
	class GlobalVariables;
}

/// <summary>
/// GameScene以外でのデモ用プレイヤークラス
/// </summary>
class DemoPlayer : private DemoPlayerStateContext
{
public:

	/// <summary>
	/// 初期化。
	/// </summary>
	/// <param name="parent">親のワールド変換</param>
	void Initialize(YKEngine::WorldTransform* parent);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera">描画に使用するカメラ</param>
	void Draw(YKEngine::Camera* camera);

	/// <summary>
	/// シーンが終わるときの処理
	/// </summary>
	void SetIsSceneEnd(bool isSceneEnd) { isSceneEnd_ = isSceneEnd; }

private:

	/// <summary>
	/// 開始部の更新
	/// </summary>
	void UpdateStart() override;

	/// <summary>
	/// メインの更新
	/// </summary>
	void UpdateMain() override;

	/// <summary>
	/// 終了部の更新
	/// </summary>
	void UpdateEnd() override;

	/// <summary>
	/// アニメーションが終了したらtrueを返す
	/// </summary>
	bool IsEndAnimation() const override { return accessAnimator_->GetIsEnd(); }

	/// <summary>
	/// シーンが終わるときtrueを返す
	/// </summary>
	bool IsSceneEnd() const override { return isSceneEnd_; }

	/// <summary>
	/// 開始部の処理が完了した後の処理
	/// </summary>
	void AfterStartComplete() override;

	/// <summary>
	/// 終了部の更新処理が始まる直前の処理
	/// </summary>
	void BeforeEnd() override;

	// オブジェクト
	std::unique_ptr<YKEngine::Skin3dObject> object_;
	// アニメーション
	std::unique_ptr<YKEngine::Animation> animation_;

	//ワールド変換データ
	YKEngine::WorldTransform worldTransform_;

	//ステートマシン
	std::unique_ptr<YKEngine::StateMachine<DemoPlayerStateContext>> stateMachine_;

	//接近アニメーション用のアニメーター
	std::unique_ptr<YKEngine::SRTAnimator> accessAnimator_;
	//往復アニメーション用のアニメーター
	std::unique_ptr<YKEngine::SRTAnimator> pingPongAnimator_;
	//往復アニメーションの開始地点とdemoPlayerの位置を補正するためのアニメーター
	std::unique_ptr<YKEngine::SRTAnimator> correctionAnimator_;

	// シーンが終了していたらtrue
	bool isSceneEnd_ = false;

	YKEngine::GlobalVariables* globalVariables_ = nullptr;
};