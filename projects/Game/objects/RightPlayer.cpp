#include "RightPlayer.h"

using namespace YKEngine;

void RightPlayer::Move()
{
	//キャラクターの移動ベクトル
	Vector3 move = { 0.0f, 0.0f, 0.0f };

	move.x = input_->GetLeftStickX();
	move.y = input_->GetLeftStickY();

	if (move.x == 0 && move.y == 0) {
		//左スティックがニュートラルなら、キーボード入力を確認
		//押した方向で移動ベクトルを変更(左右)
		if (input_->PushKey(DIK_LEFT)) {
			move.x = -1.0f;
		}
		else if (input_->PushKey(DIK_RIGHT)) {
			move.x = 1.0f;
		}

		// 押した方向で移動ベクトルを変更(上下)
		if (input_->PushKey(DIK_DOWN)) {
			move.y = -1.0f;
		}
		else if (input_->PushKey(DIK_UP)) {
			move.y = 1.0f;
		}

		move = Normalize(move); //移動ベクトルの正規化

	}
	float kSpeed = 0.1f; //移動速度

	worldTransform_.translation_ += move * kSpeed;
}
