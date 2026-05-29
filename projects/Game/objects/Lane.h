#pragma once
#include "Wall.h"

class WallModels;

/// <summary>
/// レーンクラス
/// 壁が流れてくるレーンを管理する
/// </summary>
class Lane
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="isStart">流れ始めるかどうか</param>
	/// <param name="wallModels">壁のモデル</param>
	void Initialize(bool* isStart, WallModels* wallModels);

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
	/// 壁を追加する
	/// </summary>
	/// <param name="wallData">壁のデータ</param>
	void AddWall(const YKEngine::WallData& wallData);

	// 壁を取得する
	const std::vector<std::unique_ptr<Wall>>& GetWalls() const { return walls_; }

	void SetRotate(const YKEngine::Vector3& rotate) { worldTransform_.rotation_ = rotate; }

private:

	//壁のモデル
	WallModels* wallModels_ = nullptr;

	//ワールド変換
	YKEngine::WorldTransform worldTransform_;
	//3Dオブジェクト
	std::unique_ptr<YKEngine::My3dObject> object_;

	//壁
	std::vector<std::unique_ptr<Wall>> walls_;

	//流れ始めるかどうかのフラグのポインタ
	bool* isStart_ = nullptr;

};

