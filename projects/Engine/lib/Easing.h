#pragma once

enum class EasingType
{
	Linear,
	EaseIn,
	EaseOut,
};

float ApplyEasing(EasingType type, float t);

float EaseIn(float x);

float EaseOut(float x);