#pragma once
#include "YKFramework.h"
class EffectManager;

/// <summary>
/// ゲームのメインクラス。
/// YKFrameworkを継承して作成する。
/// YKFrameworkの機能を使用してゲームを作成する。
/// </summary>
class MyGame : public YKFramework
{
public:

	//初期化
	void Initialize() override;

	//終了
	void Finalize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	
private:
	
	EffectManager* effectManager_ = nullptr;

};

