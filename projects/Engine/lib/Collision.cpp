#include <math.h>
#include "Collision.h"
#include "Matrix.h"
#include "Vector.h"
#include <algorithm>

bool CapsuleCollision(Vector2 capsuleA, Vector2 capsuleB, Vector2 CircleC, float radiusC, float radiusA) {
	float length;

	float dot;

	float t;

	Vector2 d;
	Vector2 ba;
	Vector2 e = { 0, 0 };
	Vector2 f;

	d.x = CircleC.x - capsuleA.x;
	d.y = CircleC.y - capsuleA.y;

	ba.x = capsuleB.x - capsuleA.x;
	ba.y = capsuleB.y - capsuleA.y;

	length = sqrtf(ba.x * ba.x + ba.y * ba.y);

	if (length == 0.0f) {
		length = sqrtf(d.x * d.x + d.y * d.y);

		if (length < radiusC + radiusA) {
			return 1;
		}
		else {
			return 0;
		}
	}

	if (length != 0.0f) {
		e.x = ba.x / length;
		e.y = ba.y / length;
	}

	dot = d.x * e.x + d.y * e.y;

	t = dot / length;

	if (t < 0) {
		t = 0;
	}
	if (t > 1) {
		t = 1;
	}

	f.x = (1.0f - t) * capsuleA.x + t * capsuleB.x;
	f.y = (1.0f - t) * capsuleA.y + t * capsuleB.y;

	length = sqrtf((CircleC.x - f.x) * (CircleC.x - f.x) + (CircleC.y - f.y) * (CircleC.y - f.y));

	if (length < radiusC + radiusA) {
		return true;
	}
	else {
		return false;
	}

}

bool BoxCollision(Vector2 boxA, int boxAwidht, int boxAhight, Vector2 boxB, int boxBwidht, int boxBhight)
{

	if (boxA.x - (boxAwidht / 2) <= boxB.x + (boxBwidht / 2)) {
		if (boxA.x + (boxAwidht / 2) >= boxB.x - (boxBwidht / 2)) {
			if (boxA.y - (boxAhight / 2) <= boxB.y + (boxBhight / 2)) {
				if (boxA.y + (boxAhight / 2) >= boxB.y - (boxBhight / 2)) {
					return true;
				}
			}
		}
	}

	return false;

}

bool IsCollision(const Square& square, const Vector2 point)
{
	if ((point.x <= square.max.x && point.x >= square.min.x) &&
		(point.y <= square.max.y && point.y >= square.min.y)) {
		
		return true;
	}
	return false;
}

bool IsCollision(const Square& square, const Circle& circle)
{
	Vector2 closestPoint{ std::clamp(circle.center.x, square.min.x, square.max.x),
						  std::clamp(circle.center.y, square.min.y, square.max.y),};

	float distance = Length(Subtract(circle.center, closestPoint));

	if (distance <= circle.radius) {
		return true;
	}

	return false;

}

bool IsCollision(const Sphere& s1, const Sphere& s2) {

	//2つの球の中心点間の距離を求める
	float distance = Length(Subtract(s2.center, s1.center));
	//半径の合計よりも短ければ衝突
	if (!(distance <= s1.radius + s2.radius)) {
		return false;
	}
	return true;
}

bool IsCollision(const Sphere& sphere, const Plane& plane) {

	float k = Dot(plane.normal, sphere.center) - plane.distance;
	if (k < 0) {
		k *= -1;
	}

	if (!(k <= sphere.radius)) {
		return false;
	}
	return true;
}

bool IsCollision(const Line& line, const Plane& plane) {

	float dot = Dot(line.diff, plane.normal);
	if (dot == 0.0f) {
		return false;
	}
	return true;

}

bool IsCollision(const Ray& ray, const Plane& plane) {

	float dot = Dot(ray.diff, plane.normal);
	if (dot == 0.0f) {
		return false;
	}

	float t = (plane.distance - Dot(ray.origin, plane.normal)) / dot;
	if (0 <= t) {
		return true;
	}
	return false;


}

bool IsCollision(const Segment& segment, const Plane& plane) {

	float dot = Dot(segment.diff, plane.normal);
	if (dot == 0.0f) {
		return false;
	}

	float t = (plane.distance - Dot(segment.origin, plane.normal)) / dot;
	if (0 <= t && t <= 1) {
		return true;
	}
	return false;

}

bool IsCollision(const Triangle& triangle, const Line& line) {

	Plane plane;
	Vector3 v01 = Subtract(triangle.vertices[1], triangle.vertices[0]);
	Vector3 v12 = Subtract(triangle.vertices[2], triangle.vertices[1]);
	plane.normal = Normalize(Cross(v01, v12));
	plane.distance = Dot(triangle.vertices[0], plane.normal);

	float dot = Dot(line.diff, plane.normal);
	if (dot == 0.0f) {
		return false;
	}

	float t = (plane.distance - Dot(line.origin, plane.normal)) / dot;


	Vector3 p = Add(line.origin, Multiply(t, line.diff));
	Vector3 v20 = Subtract(triangle.vertices[0], triangle.vertices[2]);
	Vector3 v1p = Subtract(p, triangle.vertices[1]);
	Vector3 v2p = Subtract(p, triangle.vertices[2]);
	Vector3 v0p = Subtract(p, triangle.vertices[0]);

	Vector3 cross01 = Cross(v01, v1p);
	Vector3 cross12 = Cross(v12, v2p);
	Vector3 cross20 = Cross(v20, v0p);

	if (Dot(cross01, plane.normal) >= 0.0f &&
		Dot(cross12, plane.normal) >= 0.0f &&
		Dot(cross20, plane.normal) >= 0.0f) {
		return true;
	}



	return false;


}

bool IsCollision(const Triangle& triangle, const Ray& ray) {

	Plane plane;
	Vector3 v01 = Subtract(triangle.vertices[1], triangle.vertices[0]);
	Vector3 v12 = Subtract(triangle.vertices[2], triangle.vertices[1]);
	plane.normal = Normalize(Cross(v01, v12));
	plane.distance = Dot(triangle.vertices[0], plane.normal);

	float dot = Dot(ray.diff, plane.normal);
	if (dot == 0.0f) {
		return false;
	}

	float t = (plane.distance - Dot(ray.origin, plane.normal)) / dot;
	if (t >= 0) {

		Vector3 p = Add(ray.origin, Multiply(t, ray.diff));
		Vector3 v20 = Subtract(triangle.vertices[0], triangle.vertices[2]);
		Vector3 v1p = Subtract(p, triangle.vertices[1]);
		Vector3 v2p = Subtract(p, triangle.vertices[2]);
		Vector3 v0p = Subtract(p, triangle.vertices[0]);

		Vector3 cross01 = Cross(v01, v1p);
		Vector3 cross12 = Cross(v12, v2p);
		Vector3 cross20 = Cross(v20, v0p);

		if (Dot(cross01, plane.normal) >= 0.0f &&
			Dot(cross12, plane.normal) >= 0.0f &&
			Dot(cross20, plane.normal) >= 0.0f) {
			return true;
		}

	}

	return false;


}

bool IsCollision(const Triangle& triangle, const Segment& segment) {

	Plane plane;
	Vector3 v01 = Subtract(triangle.vertices[1], triangle.vertices[0]);
	Vector3 v12 = Subtract(triangle.vertices[2], triangle.vertices[1]);
	plane.normal = Normalize(Cross(v01, v12));
	plane.distance = Dot(triangle.vertices[0], plane.normal);

	float dot = Dot(segment.diff, plane.normal);
	if (dot == 0.0f) {
		return false;
	}

	float t = (plane.distance - Dot(segment.origin, plane.normal)) / dot;
	if (0 <= t && t <= 1) {

		Vector3 p = Add(segment.origin, Multiply(t, segment.diff));
		Vector3 v20 = Subtract(triangle.vertices[0], triangle.vertices[2]);
		Vector3 v1p = Subtract(p, triangle.vertices[1]);
		Vector3 v2p = Subtract(p, triangle.vertices[2]);
		Vector3 v0p = Subtract(p, triangle.vertices[0]);

		Vector3 cross01 = Cross(v01, v1p);
		Vector3 cross12 = Cross(v12, v2p);
		Vector3 cross20 = Cross(v20, v0p);

		if (Dot(cross01, plane.normal) >= 0.0f &&
			Dot(cross12, plane.normal) >= 0.0f &&
			Dot(cross20, plane.normal) >= 0.0f) {
			return true;
		}

	}

	return false;

}

bool IsCollision(const AABB& aabb1, const AABB& aabb2) {

	if ((aabb1.min.x <= aabb2.max.x && aabb1.max.x >= aabb2.min.x) &&
		(aabb1.min.y <= aabb2.max.y && aabb1.max.y >= aabb2.min.y) &&
		(aabb1.min.z <= aabb2.max.z && aabb1.max.z >= aabb2.min.z)) {

		return true;
	}
	return false;

}

bool IsCollision(const AABB& aabb, const Sphere& sphere) {

	Vector3 closestPoint{ std::clamp(sphere.center.x, aabb.min.x, aabb.max.x),
						  std::clamp(sphere.center.y, aabb.min.y, aabb.max.y),
						  std::clamp(sphere.center.z, aabb.min.z, aabb.max.z) };

	float distance = Length(Subtract(sphere.center, closestPoint));

	if (distance <= sphere.radius) {
		return true;
	}

	return false;

}

bool IsCollision(const AABB& aabb, const Line& line) {


	float txMin = (aabb.min.x - line.origin.x) / line.diff.x;
	float txMax = (aabb.max.x - line.origin.x) / line.diff.x;
	float tyMin = (aabb.min.y - line.origin.y) / line.diff.y;
	float tyMax = (aabb.max.y - line.origin.y) / line.diff.y;
	float tzMin = (aabb.min.z - line.origin.z) / line.diff.z;
	float tzMax = (aabb.max.z - line.origin.z) / line.diff.z;

	float tNearX = (std::min)(txMin, txMax);
	float tFarX = (std::max)(txMin, txMax);
	float tNearY = (std::min)(tyMin, tyMax);
	float tFarY = (std::max)(tyMin, tyMax);
	float tNearZ = (std::min)(tzMin, tzMax);
	float tFarZ = (std::max)(tzMin, tzMax);

	float tmin = (std::max)((std::max)(tNearX, tNearY), tNearZ);
	float tmax = (std::min)((std::min)(tFarX, tFarY), tFarZ);

	if (tmin <= tmax) {
		return true;
	}

	return false;


}

bool IsCollision(const AABB& aabb, const Ray& ray) {

	float txMin = (aabb.min.x - ray.origin.x) / ray.diff.x;
	float txMax = (aabb.max.x - ray.origin.x) / ray.diff.x;
	float tyMin = (aabb.min.y - ray.origin.y) / ray.diff.y;
	float tyMax = (aabb.max.y - ray.origin.y) / ray.diff.y;
	float tzMin = (aabb.min.z - ray.origin.z) / ray.diff.z;
	float tzMax = (aabb.max.z - ray.origin.z) / ray.diff.z;

	float tNearX = (std::min)(txMin, txMax);
	float tFarX = (std::max)(txMin, txMax);
	float tNearY = (std::min)(tyMin, tyMax);
	float tFarY = (std::max)(tyMin, tyMax);
	float tNearZ = (std::min)(tzMin, tzMax);
	float tFarZ = (std::max)(tzMin, tzMax);

	float tmin = (std::max)((std::max)(tNearX, tNearY), tNearZ);
	float tmax = (std::min)((std::min)(tFarX, tFarY), tFarZ);

	if (tmin <= tmax) {
		if (tmax >= 0.0f) {
			return true;
		}
	}

	return false;

}

bool IsCollision(const AABB& aabb, const Segment& segment) {

	float txMin = (aabb.min.x - segment.origin.x) / segment.diff.x;
	float txMax = (aabb.max.x - segment.origin.x) / segment.diff.x;
	float tyMin = (aabb.min.y - segment.origin.y) / segment.diff.y;
	float tyMax = (aabb.max.y - segment.origin.y) / segment.diff.y;
	float tzMin = (aabb.min.z - segment.origin.z) / segment.diff.z;
	float tzMax = (aabb.max.z - segment.origin.z) / segment.diff.z;

	float tNearX = (std::min)(txMin, txMax);
	float tFarX = (std::max)(txMin, txMax);
	float tNearY = (std::min)(tyMin, tyMax);
	float tFarY = (std::max)(tyMin, tyMax);
	float tNearZ = (std::min)(tzMin, tzMax);
	float tFarZ = (std::max)(tzMin, tzMax);

	float tmin = (std::max)((std::max)(tNearX, tNearY), tNearZ);
	float tmax = (std::min)((std::min)(tFarX, tFarY), tFarZ);

	if (tmin <= tmax) {
		if ((tmin >= 0.0f && tmin <= 1.0f) || (tmax >= 0.0f && tmax <= 1.0f)) {
			return true;
		}
		else if (tmin < 0.0f && tmax > 1.0f) {
			return true;
		}
	}

	return false;

}

bool IsCollision(const OBB& obb, const Sphere& sphere) {

	Matrix4x4 obbWorldMatrixInverse = Inverse(Matrix4x4{ { {obb.orientations[0].x,obb.orientations[0].y,obb.orientations[0].z, 0.0f},
													   {obb.orientations[1].x,obb.orientations[1].y,obb.orientations[1].z, 0.0f},
													   {obb.orientations[2].x,obb.orientations[2].y,obb.orientations[2].z, 0.0f},
													   {obb.center.x,obb.center.y,obb.center.z, 1.0f}, } });

	Vector3 centerInOBBLocalSpace = Transform(sphere.center, obbWorldMatrixInverse);

	AABB aabbOBBLocal{ Multiply(-1.0f, obb.size), obb.size };
	Sphere sphereOBBLocal{ centerInOBBLocalSpace, sphere.radius };
	//ローカル空間で衝突判定
	return IsCollision(aabbOBBLocal, sphereOBBLocal);

}

bool IsCollision(const OBB& obb, const Line& line) {

	Matrix4x4 obbWorldMatrixInverse = Inverse(Matrix4x4{ { {obb.orientations[0].x,obb.orientations[0].y,obb.orientations[0].z, 0.0f},
													   {obb.orientations[1].x,obb.orientations[1].y,obb.orientations[1].z, 0.0f},
													   {obb.orientations[2].x,obb.orientations[2].y,obb.orientations[2].z, 0.0f},
													   {obb.center.x,obb.center.y,obb.center.z, 1.0f}, } });

	Vector3 localOrigin = Transform(line.origin, obbWorldMatrixInverse);
	Vector3 localEnd = Transform(Add(line.origin, line.diff), obbWorldMatrixInverse);

	AABB localAABB{ Multiply(-1.0f, obb.size), obb.size };

	Line localLine;
	localLine.origin = localOrigin;
	localLine.diff = Subtract(localEnd, localOrigin);

	return IsCollision(localAABB, localLine);

}

bool IsCollision(const OBB& obb, const Ray& ray) {

	Matrix4x4 obbWorldMatrixInverse = Inverse(Matrix4x4{ { {obb.orientations[0].x,obb.orientations[0].y,obb.orientations[0].z, 0.0f},
													   {obb.orientations[1].x,obb.orientations[1].y,obb.orientations[1].z, 0.0f},
													   {obb.orientations[2].x,obb.orientations[2].y,obb.orientations[2].z, 0.0f},
													   {obb.center.x,obb.center.y,obb.center.z, 1.0f}, } });

	Vector3 localOrigin = Transform(ray.origin, obbWorldMatrixInverse);
	Vector3 localEnd = Transform(Add(ray.origin, ray.diff), obbWorldMatrixInverse);

	AABB localAABB{ Multiply(-1.0f, obb.size), obb.size };

	Ray localRay;
	localRay.origin = localOrigin;
	localRay.diff = Subtract(localEnd, localOrigin);

	return IsCollision(localAABB, localRay);

}

bool IsCollision(const OBB& obb, const Segment& segemnt) {

	Matrix4x4 obbWorldMatrixInverse = Inverse(Matrix4x4{ { {obb.orientations[0].x,obb.orientations[0].y,obb.orientations[0].z, 0.0f},
													   {obb.orientations[1].x,obb.orientations[1].y,obb.orientations[1].z, 0.0f},
													   {obb.orientations[2].x,obb.orientations[2].y,obb.orientations[2].z, 0.0f},
													   {obb.center.x,obb.center.y,obb.center.z, 1.0f}, } });

	Vector3 localOrigin = Transform(segemnt.origin, obbWorldMatrixInverse);
	Vector3 localEnd = Transform(Add(segemnt.origin, segemnt.diff), obbWorldMatrixInverse);

	AABB localAABB{ Multiply(-1.0f, obb.size), obb.size };

	Segment localSegment;
	localSegment.origin = localOrigin;
	localSegment.diff = Subtract(localEnd, localOrigin);

	return IsCollision(localAABB, localSegment);

}

bool IsCollision(const OBB& obb1, const OBB& obb2) {

	Vector3 separationAxis;

	for (int i = 0; i < 15; i++) {

		if (i < 3) {
			separationAxis = obb1.orientations[i];
		}
		else if (i < 6) {
			separationAxis = obb2.orientations[i - 3];
		}
		else {
			if (i < 9) {
				separationAxis = Cross(obb1.orientations[0], obb2.orientations[i - 6]);
			}
			else if (i < 12) {
				separationAxis = Cross(obb1.orientations[1], obb2.orientations[i - 9]);
			}
			else if (i < 15) {
				separationAxis = Cross(obb1.orientations[2], obb2.orientations[i - 12]);
			}
		}

		// 各OBBの投影範囲を計算
		float projection1 =
			obb1.size.x * std::abs(Dot(separationAxis, obb1.orientations[0])) +
			obb1.size.y * std::abs(Dot(separationAxis, obb1.orientations[1])) +
			obb1.size.z * std::abs(Dot(separationAxis, obb1.orientations[2]));

		float projection2 =
			obb2.size.x * std::abs(Dot(separationAxis, obb2.orientations[0])) +
			obb2.size.y * std::abs(Dot(separationAxis, obb2.orientations[1])) +
			obb2.size.z * std::abs(Dot(separationAxis, obb2.orientations[2]));

		float distance = std::abs(Dot(Subtract(obb2.center, obb1.center), separationAxis));

		if (!(distance <= (projection1 + projection2))) {
			return false;
		}

	}

	return true;

}