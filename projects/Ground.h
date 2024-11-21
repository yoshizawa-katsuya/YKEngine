#pragma once
#include "Base3dObject.h"
#include "WorldTransform.h"

#include <memory>

/// <summary>
/// 地面
/// </summary>
class Ground
{
public:

	void Initialzie(BaseModel* model);

	void Update(Camera* camera);

	void Draw();

private:

	WorldTransform worldTransform_;

	std::unique_ptr<Base3dObject> object_;
	
};

