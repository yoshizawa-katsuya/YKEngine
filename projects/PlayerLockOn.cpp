#include "PlayerLockOn.h"
#include "Matrix.h"
#include "WinApp.h"
#include <cassert>
#include <Boss.h>


void PlayerLockOn::Initialize(Camera* camera)
{
	camera_ = camera;
}

void PlayerLockOn::Update(const std::unique_ptr<Player>& player)
{
	viewProjection_ = camera_->GetViewProjection();
	matView_ = camera_->GetViewMatrix();
	matProjection_ = camera_->GetProjection();

	// ロックオン状態なら
	if (target_) {
		// 範囲外判定
		if (OutsideSelectionRange()) {
			// ロックオンを外す
			target_ = nullptr;
		}
	} else {
		// ロックオン対象の検索
		Search(player);
	}
}

void PlayerLockOn::Search(const std::unique_ptr<Player>& player)
{
	// 目標
	std::list<std::pair<float, const Player*>> targets;

	// プレイヤーに対してロックオン判定
	// 敵のロックオン座標を取得
	Vector3 positionWorld = player->GetCenterPosition();

	// ワールド->ビュー座標変換
	Vector3 positionView = Transform(positionWorld, matView_);

	// 距離条件チェック
	if (minDistance_ <= positionView.z && positionView.z <= maxDistance_) {
		// カメラ前方との角度を計算
		float arcTangent = std::atan2(std::sqrtf(positionView.x * positionView.x + positionView.y * positionView.y), positionView.z);

		// 角度条件チェック（コーンに収まっているか）
		if (std::abs(arcTangent) <= angleRange_) {
			targets.emplace_back(std::make_pair(positionView.z, player.get()));
		}
	}

	// ロックオン対象をリセット
	target_ = nullptr;
	if (!targets.empty()) {
		// 距離で昇順にソート
		targets.sort([](auto& pair1, auto& pair2) { return pair1.first < pair2.first; });
		// ソートの結果一番近いプレイヤーをロックオン対象とする
		target_ = targets.front().second;
	}

}

Vector3 PlayerLockOn::WorldToScreen(Vector3& worldPosition)
{
	// ビューポート行列
	Matrix4x4 matViewport = MakeViewportMatrix(0, 0, WinApp::kClientWidth, WinApp::kClientHeight, 0, 1);
	// ビュー行列とプロジェクション行列、ビューポート行列を合成する
	Matrix4x4 matViewProjectionViewport = matView_ * matProjection_ * matViewport;
	// ワールド->スクリーン変換
	Vector3 screenPosition = Transform(worldPosition, matViewProjectionViewport);
	return screenPosition;
}

bool PlayerLockOn::OutsideSelectionRange()
{
	std::list<std::pair<float, const Boss*>> targets;

	// すべての敵に対して順にロックオン判定
	// 敵のロックオン座標を取得
	Vector3 positionWorld = target_->GetCenterPosition();
	// ワールド->ビュー座標変換
	Vector3 positionView = Transform(positionWorld, matView_);
	// 距離条件のチェック
	if (minDistance_ <= positionView.z && positionView.z <= maxDistance_) {
		// カメラ前方との角度を計算
		float arcTangent = std::atan2(std::sqrt(positionView.x * positionView.x + positionView.y * positionView.y), positionView.z);
		// 角度条件チェック(コーンにおさまってるか)
		if (std::abs(arcTangent) <= angleRange_) {
			// 範囲外ではない
			return false;
		}
	}
	// 範囲外である
	return true;
}

Vector3 PlayerLockOn::GetTargetPosition() const
{
	if (target_) {
		return target_->GetCenterPosition();
	}
	return Vector3{};
}

Vector3 PlayerLockOn::Transform(const Vector3& vector, const Matrix4x4& matrix)
{
	Vector3 result{};
	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + 1.0f * matrix.m[3][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + 1.0f * matrix.m[3][1];
	result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + 1.0f * matrix.m[3][2];
	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + 1.0f * matrix.m[3][3];
	assert(w != 0.0f);
	result.x /= w;
	result.y /= w;
	result.z /= w;
	return result;
}
