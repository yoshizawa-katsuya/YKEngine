#pragma once
#include "BaseModel.h"

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

