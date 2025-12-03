#pragma once
#include "WorldTransform.h"
#include "My3dObject.h"
#include "SRTAnimator.h"

/// <summary>
/// GameScene以外でのデモ用プレイヤークラス
/// </summary>
class DemoPlayer
{
public:

	/// <summary>
	/// 初期化。
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="parent">親のワールド変換</param>
	void Initialize(BaseModel* model, WorldTransform* parent);

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera">描画に使用するカメラ</param>
	virtual void Draw(Camera* camera);

	/// <summary>
	/// シーンが終わるときの処理
	/// </summary>
	void SceneEnd();

private:

	/// <summary>
	/// 開始部の更新
	/// </summary>
	void UpdateStart();

	/// <summary>
	/// メインの更新
	/// </summary>
	void UpdateMain();

	/// <summary>
	/// 終了部の更新
	/// </summary>
	void UpdateEnd();

	std::unique_ptr<My3dObject> object_;
	//ワールド変換データ
	WorldTransform worldTransform_;

	enum class Phase
	{
		kStart,	// 開始
		kMain,	// メイン
		kEnd, // 終わり
	};
	//フェーズ
	Phase phase_ = Phase::kStart;

	std::unique_ptr<SRTAnimator> animator_;
};