#pragma once
#include <numbers>

struct ParticleRandomizationFlags {
	bool color = false;
	bool translate = false;
	bool velocity = false;
	bool rotate = false;
	bool scale = false;
	bool lifeTime = false;
};

template<typename T>
struct RandomRange
{
	T max;
	T min;
};

struct EmitterRangeParams
{
	EmitterRangeParams()
		: translate({ Vector3{ 1.0f, 1.0f, 1.0f }, Vector3{ -1.0f, -1.0f, -1.0f } })
		, scale ({ { 1.0f, 1.0f, 1.0f } , { -1.0f, -1.0f, -1.0f } })
		, rotate ({ { std::numbers::pi_v<float>, std::numbers::pi_v<float>, std::numbers::pi_v<float> } ,{ -std::numbers::pi_v<float>, -std::numbers::pi_v<float>, -std::numbers::pi_v<float> } })
		, velocity({ Vector3{ 1.0f, 1.0f, 1.0f }, Vector3{ -1.0f, -1.0f, -1.0f } })
	{
	}
	RandomRange<Vector3> translate;
	RandomRange<Vector3> scale;
	RandomRange<Vector3> rotate;
	RandomRange<Vector3> velocity;
};