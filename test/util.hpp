#pragma once

#include <algorithm>
#include <cmath>

bool FloatEqual(float a, float b, float p = 0.1) {
	float pa = abs(a-b)/std::max(a,b);
	return (pa < p);
}

