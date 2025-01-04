#pragma once
#include "Struct.h"

bool CapsuleCollision(Vector2 capsuleA, Vector2 capsuleB, Vector2 CircleC, float radiusC, float radiusA);

bool BoxCollision(Vector2 boxA, int boxAwidht, int boxAhight, Vector2 boxB, int boxBwidht, int boxBhight);

//円と四角の衝突判定
bool IsCollision(const Square& square, const Circle& circle);

//円と円の消灯判定
bool IsCollision(const Circle& circle1, const Circle& circle2);

//球と球の衝突判定
bool IsCollision(const Sphere& s1, const Sphere& s2);

//球と平面の衝突判定
bool IsCollision(const Sphere& sphere, const Plane& plane);

//直線と平面の衝突判定
bool IsCollision(const Line& line, const Plane& plane);

//半直線と平面の衝突判定
bool IsCollision(const Ray& ray, const Plane& plane);

//線分と平面の衝突判定
bool IsCollision(const Segment& segment, const Plane& plane);

//三角形と直線の衝突判定
bool IsCollision(const Triangle& triangle, const Line& line);

//三角形と半直線の衝突判定
bool IsCollision(const Triangle& triangle, const Ray& ray);

//三角形と線分の衝突判定
bool IsCollision(const Triangle& triangle, const Segment& segment);

//AABB同士の衝突判定
bool IsCollision(const AABB& aabb1, const AABB& aabb2);

//AABBと球の衝突判定
bool IsCollision(const AABB& aabb, const Sphere& sphere);

//AABBと直線の衝突判定
bool IsCollision(const AABB& aabb, const Line& line);

//AABBと半直線の衝突判定
bool IsCollision(const AABB& aabb, const Ray& ray);

//AABBと線分の衝突判定
bool IsCollision(const AABB& aabb, const Segment& segment);

//OBBと球の衝突判定
bool IsCollision(const OBB& obb, const Sphere& sphere);

//OBBと直線の衝突判定
bool IsCollision(const OBB& obb, const Line& line);

//OBBと半直線の衝突判定
bool IsCollision(const OBB& obb, const Ray& ray);

//OBBと線分の衝突判定
bool IsCollision(const OBB& obb, const Segment& segemnt);

//OBB同士の衝突判定
bool IsCollision(const OBB& obb1, const OBB& obb2);