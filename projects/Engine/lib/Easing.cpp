#define _USE_MATH_DEFINES
#include <math.h>
#include "Easing.h"

float EaseIn(float x) {
	return (1 - cosf((x * float(M_PI)) / 2));
}

float EaseOut(float x) {
	return sinf((x * float(M_PI)) / 2);
}