#pragma once
#include "GameScene.h"
class TutorialScene : public GameScene
{
public:

	~TutorialScene() override;

	void Initialize() override;

	void Update() override;

private:

	void CreateLevel() override;

};

