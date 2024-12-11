#pragma once
#include "Base3dObject.h"
#include "WorldTransform.h"
#include "Animation.h"
#include "Input.h"
#include <numbers>
class Camera;
class MapChipField;

class Player
{
public:

	Player();

	void Initialize(BaseModel* model);

	void Update();

	void Draw(Camera* camera);

private:

	//Transform変数を作る
	WorldTransform worldTransform_;

	std::unique_ptr<Base3dObject> object_;

	Input* input_;

	enum  class LRDirection {
		kRight,
		kLeft,
	};

	LRDirection lrDirection_;
	// 左右の自キャラ角度テーブル
	std::vector<float> destinationRotationYTable = { std::numbers::pi_v<float> / 2.0f, std::numbers::pi_v<float> * 3.0f / 2.0f };
	//角度補完レート
	float angleCompletionRate_;
	//目標角度
	float targetAngle_;
	//旋回開始時の角度
	float turnFirstRotationY_;
	//旋回タイマー
	float turnTimer_;

	float speed_;

};

