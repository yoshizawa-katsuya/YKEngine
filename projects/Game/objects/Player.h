#pragma once
#include "BaseModel.h"
#include "SkinModel.h"
#include "WorldTransform.h"
#include "Animation.h"
class Camera;
class MapChipField;

class Player
{
public:

	void Initialize(const std::vector<BaseModel*>& models);

	void Update();

	void Draw(Camera* camera);

private:

	// 各部位のTransform変数を格納する
	std::vector<WorldTransform> worldTransforms_;

	std::vector<BaseModel*> models_;

};

