#pragma once
#include "BaseModel.h"

/// <summary>
/// スキンメッシュを持つモデル。
/// スキンクラスターを持つ。
/// アニメーションに対応している。
/// アニメーションはスキンクラスターを通じてボーンの変形をメッシュに反映させる。
/// スキンクラスターはSetSkinClusterで設定する。
/// </summary>
class SkinModel : public BaseModel
{
public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	~SkinModel() override;

	/// <summary>
	/// 描画。
	/// </summary>
	/// <param name="usedMaterial">3DObject側でマテリアルを使用するか</param>
	void Draw(bool usedMaterial) override;

	/// <summary>
	/// 描画。(テクスチャハンドル指定版)
	/// </summary>
	/// <param name="textureHandle">テクスチャハンドル</param>
	/// <param name="usedMaterial">3DObject側でマテリアルを使用するか</param>
	void Draw(uint32_t textureHandle, bool usedMaterial) override;

	/// <summary>
	/// スキンクラスターをコマンドリストに設定する。
	/// </summary>
	/// <param name="skinCluster">スキンクラスター</param>
	void SetSkinCluster(const SkinCluster& skinCluster) override;

private:

	/// <summary>
	/// モデルファイル読み込み。
	/// </summary>
	/// <param name="directoryPath">ディレクトリパス</param>
	/// <param name="filename">ファイル名</param>
	void LoadModelFile(const std::string& directoryPath, const std::string& filename) override;

	/// <summary>
	/// スキンクラスター作成用のデータを読み込む。
	/// </summary>
	/// <param name="mesh">assimpメッシュ</param>
	void LoadSkinCluster(aiMesh* mesh);

};

