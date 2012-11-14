#include <test/util.hpp>

bool FloatEqual(float a, float b, float p) {
    float pa = (a-b)/std::max(a,b);
    return (pa < p && pa > -1*p);
}

