#pragma once
#include "Base3dObject.h"
#include "WorldTransform.h"

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

