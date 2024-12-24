#pragma once
class Collider
{
public:

	virtual ~Collider() = default;

protected:
	//衝突半径
	float radius_ = 1.5f;
};

