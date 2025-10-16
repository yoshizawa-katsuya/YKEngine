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

	~SkinModel() override;

	void Draw(bool usedMaterial) override;

	void Draw(uint32_t textureHandle, bool usedMaterial) override;

	void SetSkinCluster(const SkinCluster& skinCluster) override;

private:

	void LoadModelFile(const std::string& directoryPath, const std::string& filename) override;

	//スキンクラスター作成用のデータ読み込み
	void LoadSkinCluster(aiMesh* mesh);

};

