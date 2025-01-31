#include "LockOn.h"
#include "TextureManager.h"
#include "Matrix.h"
#include "Vector.h"
#include <cmath>

void LockOn::Initialize() {

	uint32_t textureHandle = TextureManager::GetInstance()->Load("./Resources/cursor.png");
	
	lockOnMark_ = std::make_unique<Sprite>();
	lockOnMark_->Initialize(textureHandle);
	lockOnMark_->SetPosition({ 640.0f, 360.0f });
	
}

void LockOn::Update(const std::list<std::unique_ptr<Enemy>>& enemies, Camera* camera) {
	
	
	// ロックオン状態なら
	if (target_) {
		// ロックオン解除処理
		// ロックオンボタンをトリガーしたら
		if (Input::GetInstance()->TriggerButton(XINPUT_GAMEPAD_RIGHT_SHOULDER)) {
			//ロックオンを外す
			target_ = nullptr;
		} 
		//範囲外判定
		else if (IsOutsideRange(camera)) {
			
			//ロックオンを外す
			target_ = nullptr;

		}
	} else {

		// ロックオンボタンをトリガーしたら
		if (Input::GetInstance()->TriggerButton(XINPUT_GAMEPAD_RIGHT_SHOULDER)) {
			// ロックオン対象の検索
			Search(enemies, camera);
		}
	}
	

	// ロックオン継続
	if (target_) {
		//ロックオンマークの座標計算
		
		//敵のロックオン座標取得
		Vector3 positionWorld = target_->GetCenterPosition();
		//ワールド座標からスクリーン座標に変換
		Vector3 positionScreen = ConvertingToScreen(positionWorld, camera);
		//Vector2に格納
		Vector2 positionScreenV2(positionScreen.x - lockOnMark_->GetSize().x / 2, positionScreen.y);
		//スプライトの座標を設定
		lockOnMark_->SetPosition(positionScreenV2);

	}


}

void LockOn::Search(const std::list<std::unique_ptr<Enemy>>& enemies, Camera* camera) {

	//目標
	std::list<std::pair<float, const Enemy*>> targets;

	//すべての敵に対して順にロックオン判定
	for (const std::unique_ptr<Enemy>& enemy : enemies) {
		
		//敵のロックオン座標取得
		Vector3 positionWorld = enemy->GetCenterPosition();

		//ワールド→ビュー座標変換
		Vector3 positionView = Transform(positionWorld, camera->GetViewMatrix());

		//距離条件チェック
		if (minDistance_ <= positionView.z && positionView.z <= maxDistance_) {
		
			//カメラ前方との角度を計算
			float arcTangent = std::atan2(
				std::sqrt(positionView.x * positionView.x + positionView.y * positionView.y),
				positionView.z);

			//角度条件チェック(コーンに収まっているか)
			if (fabs(arcTangent) <= angleRange_) {
				targets.emplace_back(std::make_pair(positionView.z, enemy.get()));
			}
		
		}

	}

	//ロックオン対象をリセット
	target_ = nullptr;
	if (!targets.empty()) {
		//距離で照準にソート
		targets.sort([](auto& pair1, auto& pair2) { return pair1.first < pair2.first; });
		//ソートの結果一番近い敵をロックオン対象とする
		target_ = targets.front().second;
	}

}

bool LockOn::IsOutsideRange(Camera* camera) {

	// 敵のロックオン座標取得
	Vector3 positionWorld = target_->GetCenterPosition();

	// ワールド→ビュー座標変換
	Vector3 positionView = Transform(positionWorld, camera->GetViewMatrix());

	// 距離条件チェック
	if (minDistance_ <= positionView.z && positionView.z <= maxDistance_) {

		// カメラ前方との角度を計算
		float arcTangent = std::atan2(std::sqrt(positionView.x * positionView.x + positionView.y * positionView.y), positionView.z);

		// 角度条件チェック(コーンに収まっているか)
		if (fabs(arcTangent) <= angleRange_) {
			return false;
		}
	}

	return true;
}

void LockOn::Draw() {

	if (target_) {
		lockOnMark_->Draw();
	}

}

Vector3 LockOn::GetTargetPosition() const {
	
	if (target_) {
		return target_->GetCenterPosition();
	}

	return Vector3();

}
