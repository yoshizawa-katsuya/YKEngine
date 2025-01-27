#pragma once
#include "GameScene.h"
class TutorialScene : public GameScene
{
public:

	~TutorialScene() override;

	void Initialize() override;

	void Update() override;

	void Draw() override;

private:

	void CreateLevel() override;

	std::unique_ptr<BaseModel> modelPlane_;

	struct Plane {
		std::unique_ptr<Base3dObject> object_;
		uint32_t textureHandle_;
	};

	std::vector<std::unique_ptr<Plane>> planes_;

};

