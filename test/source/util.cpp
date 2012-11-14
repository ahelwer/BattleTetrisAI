#include <test/util.hpp>

bool FloatEqual(float a, float b, float p) {
    float pa = abs(a-b)/std::max(a,b);
    return (pa < p);
}

