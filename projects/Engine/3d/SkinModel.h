#pragma once
#include "BaseModel.h"

class SkinModel : public BaseModel
{
public:

	~SkinModel() override;

	void CreateModel(const std::string& directoryPath, const std::string& filename) override;

	void Update() override;

	void Update(Animation* animation) override;

	void Draw() override;

	void Draw(uint32_t textureHandle) override;

	void SetSkinCluster(const SkinCluster& skinCluster) override;

private:

	void LoadModelFile(const std::string& directoryPath, const std::string& filename) override;

	//スキンクラスター作成用のデータ読み込み
	void LoadSkinCluster(aiMesh* mesh);

};

