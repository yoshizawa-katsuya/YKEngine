#pragma once
#include "Input.h"
class Camera;
class MapChipField;

#include "BaseCharacter.h"

class Player : public BaseCharacter
{
public:

	void Initialize(BaseModel* model) override;

	void Update() override;

	void Draw(Camera* camera) override;

private:


};

