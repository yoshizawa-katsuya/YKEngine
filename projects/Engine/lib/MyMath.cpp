#include "MyMath.h"

namespace YKEngine
{

Vector3 ClosestPoint(const Vector3& point, const Segment& segment)
{

	Vector3 o = segment.origin;
	Vector3 a = Subtract(point, o);
	Vector3 b = segment.diff;
	float bLength = Length(b);

	float t = Dot(a, b) / (bLength * bLength);

	Vector3 tb = Multiply(t, b);

	Vector3 cp = Add(o, tb);

	return cp;

}

}	//namespcae YKEngine