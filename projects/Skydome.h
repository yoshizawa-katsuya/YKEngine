#pragma once
#include "WorldTransform.h"
#include "Base3dObject.h"

class Camera;

/// <summary>
/// 天球
/// </summary>
class Skydome
{
public:

	void Initialzie(BaseModel* model);

	void Update(Camera* camera);

	void Draw();

private:
	WorldTransform worldTransform_;

	std::unique_ptr<Base3dObject> object_;

};

