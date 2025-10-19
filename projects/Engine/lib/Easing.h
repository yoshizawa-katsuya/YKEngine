#pragma once

/// <summary>
/// イージングの種類
/// https://easings.net/ja
/// </summary>
enum class EasingType
{
	Linear,
	EaseInSine,
	EaseOutSine,
	EaseInQuad,
	EaseOutQuad,
	EaseInCubic,
	EaseOutCubic,
	EaseInBack,
	EaseOutBack,
};

/// <summary>
/// イージングを適用する。
/// </summary>
/// <param name="type">イージングの種類</param>
/// <param name="t">0.0から1.0の範囲の値</param>
/// <returns>イージングが適用された値</returns>
float ApplyEasing(EasingType type, float t);

/// <summary>
/// EaseInSineイージングを適用する。
/// </summary>
/// <param name="x">0.0から1.0の範囲の値</param>
/// <returns>イージングが適用された値</returns>
float EaseInSine(float x);

/// <summary>
/// EaseOutSineイージングを適用する。
/// </summary>
/// <param name="x">0.0から1.0の範囲の値</param>
/// <returns>イージングが適用された値</returns>
float EaseOutSine(float x);

/// <summary>
/// EaseInQuadイージングを適用する。
/// </summary>
/// <param name="x">0.0から1.0の範囲の値</param>
/// <returns>イージングが適用された値</returns>
float EaseInQuad(float x);

/// <summary>
/// EaseOutQuadイージングを適用する。
/// </summary>
/// <param name="x">0.0から1.0の範囲の値</param>
/// <returns>イージングが適用された値</returns>
float EaseOutQuad(float x);

/// <summary>
/// EaseInCubicイージングを適用する。
/// </summary>
/// <param name="x">0.0から1.0の範囲の値</param>
/// <returns>イージングが適用された値</returns>
float EaseInCubic(float x);

/// <summary>
/// EaseOutCubicイージングを適用する。
/// </summary>
/// <param name="x">0.0から1.0の範囲の値</param>
/// <returns>イージングが適用された値</returns>
float EaseOutCubic(float x);

/// <summary>
/// EaseInBackイージングを適用する。
/// </summary>
/// <param name="x">0.0から1.0の範囲の値</param>
/// <returns>イージングが適用された値</returns>
float EaseInBack(float x);

/// <summary>
/// EaseOutBackイージングを適用する。
/// </summary>
/// <param name="x">0.0から1.0の範囲の値</param>
/// <returns>イージングが適用された値</returns>
float EaseOutBack(float x);
