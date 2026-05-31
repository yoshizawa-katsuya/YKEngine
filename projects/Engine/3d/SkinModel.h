#pragma once
#include "BaseModel.h"

namespace YKEngine
{

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
	/// メッシュデータを読み込む。
	/// </summary>
	/// <param name="mesh">Assimpのメッシュデータ</param>
	/// <param name="vertexStartIndex">頂点データの開始位置</param>
	void LoadMeshData(aiMesh* mesh, uint32_t vertexStartIndex) override;

	/// <summary>
	/// スキンクラスター作成用のデータを読み込む。
	/// </summary>
	/// <param name="mesh">assimpメッシュ</param>
	/// <param name="vertexStartIndex">頂点データの開始位置</param>
	void LoadSkinCluster(aiMesh* mesh, uint32_t vertexStartIndex);

};

} // namespace YKEngine