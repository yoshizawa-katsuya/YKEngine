#pragma once
#include "Wall.h"
#include "LevelDataLoader.h"

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
	/// <param name="wallDatas">壁のデータ</param>
	void Initialize(const std::vector<YKEngine::WallData>& wallDatas);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera">カメラ</param>
	void Draw(YKEngine::Camera* camera);

	// 壁を取得する
	const std::vector<std::unique_ptr<Wall>>& GetWalls() const { return walls_; }

private:

	//壁
	std::vector<std::unique_ptr<Wall>> walls_;

	bool isStart_ = false;

};

