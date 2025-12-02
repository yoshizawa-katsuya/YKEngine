#pragma once
#include "Sprite.h"

/// <summary>
/// アニメーションスプライトを扱うクラス。
/// スプライトシートを使用してアニメーションを実装する。
/// Spriteクラスを継承している。
/// </summary>
class AnimatedSprite : public Sprite
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="textureHandle">テクスチャハンドル</param>
	/// <param name="horizontalDivisionNum">横の分割数</param>
	/// <param name="verticalDivisionNum">縦の分割数</param>
	void Initialize(uint32_t textureHandle, int32_t horizontalDivisionNum, int32_t verticalDivisionNum);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 逆再生用の初期化。
	/// </summary>
	/// <param name="textureHandle">テクスチャハンドル</param>
	/// <param name="horizontalDivisionNum">横の分割数</param>
	/// <param name="verticalDivisionNum">縦の分割数</param>
	void InitializeReverseAnimation(uint32_t textureHandle, uint32_t horizontalDivisionNum, uint32_t verticalDivisionNum);

	/// <summary>
	/// アニメーションをリセットする。
	/// </summary>
	void Reset();

	/// <summary>
	/// 逆再生アニメーションをリセットする。
	/// </summary>
	void ResetReverseAnimation();

	/// <summary>
	/// アニメーションの更新に必要なフレーム数を設定する。
	/// </summary>
	/// <param name="updateFrame">更新に必要なフレーム数</param>
	void SetUpdateFrame(uint32_t updateFrame) { updateFrame_ = updateFrame; }

	/// <summary>
	/// アニメーションをループ再生するか設定する。
	/// </summary>
	/// <param name="isLoop">ループするかどうか</param>
	void SetIsLoop(bool isLoop) { isLoop_ = isLoop; }

	/// <summary>
	/// アニメーションを逆再生するか設定する。
	/// </summary>
	/// <param name="isReverse">逆再生するかどうか</param>
	void SetIsReverse(bool isReverse) { isReverse_ = isReverse; }

	/// <summary>
	/// アニメーションが最後まで到達したか取得する。
	/// </summary>
	/// <returns>最後まで到達したか</returns>
	bool GetIsEnd() { return isEnd_; }

private:

	/// <summary>
	/// 共通の初期化処理
	/// </summary>
	/// <param name="textureHandle">テクスチャハンドル</param>
	/// <param name="horizontalDivisionNum">横の分割数</param>
	/// <param name="verticalDivisionNum">縦の分割数</param>
	void CommonInitialize(uint32_t textureHandle, int32_t horizontalDivisionNum, int32_t verticalDivisionNum);

	/// <summary>
	/// アニメーションをループ再生する更新。
	/// </summary>
	void LoopUpdate();

	/// <summary>
	/// アニメーションを再生する更新。
	/// </summary>
	void NoLoopUpdate();

	/// <summary>
	/// アニメーションをループ逆再生する更新。
	/// </summary>
	void ReverseLoopUpdate();

	/// <summary>
	/// アニメーションを逆再生する更新。
	/// </summary>
	void ReverseNoLoopUpdate();

	/// <summary>
	/// UV座標を更新する。
	/// </summary>
	void UpdateUV();

	//分割数
	int32_t horizontalDivisionNum_ = 1;
	int32_t verticalDivisionNum_ = 1;

	//現在のインデックス
	int32_t horizontalIndex_ = 0;
	int32_t verticalIndex_ = 0;

	//テクスチャの移動量
	float horizontalMovingDistance_ = 0.0f;
	float verticalMovingDistance_ = 0.0f;

	//アニメーションの更新に必要なフレーム数
	uint32_t updateFrame_ = 1;

	//現在のフレーム数
	uint32_t currentFrame_ = 0;

	//アニメーションが最後まで到達したか
	bool isEnd_ = false;

	//ループするかどうか
	bool isLoop_ = true;

	//逆再生するかどうか
	bool isReverse_ = false;
};

