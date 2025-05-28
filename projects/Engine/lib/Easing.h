#pragma once

enum class EasingType
{
	Linear,
	EaseInSine,
	EaseOutSine,
	EaseInQuad,
	EaseOutQuad,
	EaseInCubic,
	EaseOutCubic,
};

float ApplyEasing(EasingType type, float t);

float EaseInSine(float x);

float EaseOutSine(float x);

float EaseInQuad(float x);

float EaseOutQuad(float x);

float EaseInCubic(float x);

float EaseOutCubic(float x);