#include "Curve.h"
#include "Matrix.h"

Vector2 Bezier(Vector2 p0, Vector2 p1, Vector2 p2, float t)
{

	Vector2 p0p1 = Lerp(p0, p1, t);

	Vector2 p1p2 = Lerp(p1, p2, t);

	Vector2 p = Lerp(p0p1, p1p2, t);

	return p;
}

Vector3 Bezier(const Vector3& p0, const Vector3& p1, const Vector3 p2, float t)
{

	Vector3 p0p1 = Lerp(p0, p1, t);

	Vector3 p1p2 = Lerp(p1, p2, t);

	Vector3 p = Lerp(p0p1, p1p2, t);

	return p;

}

Vector2 CatmullRom(Vector2 p0, Vector2 p1, Vector2 p2, Vector2 p3, float t)
{

	Vector2 anser;
	
	anser.x = (1.0f / 2.0f) * ((-p0.x + 3.0f * p1.x - 3.0f * p2.x + p3.x) * (t * t * t) +
						(2.0f * p0.x - 5.0f * p1.x + 4.0f * p2.x - p3.x) * (t * t) +
						(-p0.x + p2.x) * t + 2.0f * p1.x);

	anser.y = (1.0f / 2.0f) * ((-p0.y + 3.0f * p1.y - 3.0f * p2.y + p3.y) * (t * t * t) +
						(2.0f * p0.y - 5.0f * p1.y + 4.0f * p2.y - p3.y) * (t * t) +
						(-p0.y + p2.y) * t + 2.0f * p1.y);

	return anser;

}

Vector3 CatmullRom(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3, float t)
{

	Vector3 p = Multiply(0.5f, Add(Add(Add(Multiply((t * t * t), Add(Add(Add(Multiply(-1.0f, p0), Multiply(3.0f, p1)), Multiply(-3.0f, p2)), p3)), 
								Multiply((t * t), Add(Add(Add(Multiply(2.0f, p0), Multiply(-5.0f, p1)), Multiply(4.0f, p2)), Multiply(-1.0f, p3)))), 
								Multiply(t, Add(Multiply(-1.0f, p0), p2))), Multiply(2.0f, p1)));

	return p;

}

std::vector<Vector3> GenerateCatmullRomSplinePoints(std::vector<Vector3>& controlPoints, uint32_t numPoints) 
{
	std::vector<Vector3> splinePoints;

	// 制御点が3点未満の場合は空のベクトルを返す
	if (controlPoints.size() < 3)
	{
		return splinePoints;
	}

	
	Vector3 p0 = controlPoints[0];
	Vector3 p1 = controlPoints[0];
	Vector3 p2 = controlPoints[1];
	Vector3 p3 = controlPoints[2];
	// 最初のセグメント
	for (uint32_t j = 0; j <= numPoints; ++j)
	{
		float t = static_cast<float>(j) / static_cast<float>(numPoints);
		splinePoints.push_back(CatmullRom(p0, p1, p2, p3, t));
	}

	size_t i = 0;
	// 中間のセグメント
	for (; i < controlPoints.size() - 3; ++i) {
		p0 = controlPoints[i];
		p1 = controlPoints[i + 1];
		p2 = controlPoints[i + 2];
		p3 = controlPoints[i + 3];

		for (uint32_t j = 0; j <= numPoints; ++j) {
			float t = static_cast<float>(j) / static_cast<float>(numPoints);
			splinePoints.push_back(CatmullRom(p0, p1, p2, p3, t));
		}
	}

	p0 = controlPoints[i];
	p1 = controlPoints[i + 1];
	p2 = controlPoints[i + 2];
	p3 = controlPoints[i + 2];
	// 最後のセグメント
	for (uint32_t j = 0; j <= numPoints; ++j) 
	{
		float t = static_cast<float>(j) / static_cast<float>(numPoints);
		splinePoints.push_back(CatmullRom(p0, p1, p2, p3, t));
	}

	return splinePoints;
}

std::vector<Vector3> GenerateCatmullRomSplinePointsLoop(std::vector<Vector3>& controlPoints, uint32_t numPoints)
{
	std::vector<Vector3> splinePoints;

	size_t n = controlPoints.size();
	if (n < 3) {
		return splinePoints;
	}

	// 各制御点区間をループさせる
	for (size_t i = 0; i < n; ++i) {
		Vector3 p0 = controlPoints[(i + n - 1) % n];
		Vector3 p1 = controlPoints[i % n];
		Vector3 p2 = controlPoints[(i + 1) % n];
		Vector3 p3 = controlPoints[(i + 2) % n];

		for (uint32_t j = 0; j < numPoints; ++j) {
			float t = static_cast<float>(j) / static_cast<float>(numPoints);
			splinePoints.push_back(CatmullRom(p0, p1, p2, p3, t));
		}
	}

	// 最後に始点と一致する点を追加して完全なループにする
	splinePoints.push_back(splinePoints.front());

	return splinePoints;
}
