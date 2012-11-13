#pragma once

#include <algorithm>
#include <utility>
#include <util/vector.hpp>

typedef std::vector<float> Harmony;
typedef std::vector< std::pair<float, float> > HarmonyRanges;
class ObjectiveFunction;

class HarmonyCompare : public std::binary_function<Harmony*, Harmony*, bool> {
public:
    HarmonyCompare(ObjectiveFunction const& f);
    bool operator() (Harmony const* a, Harmony const* b) const;
private:
    ObjectiveFunction const& m_f;
};

class HarmonyFactory {
public:
    HarmonyFactory(unsigned decisionVarCount, HarmonyRanges const& ranges);
    Harmony* GenerateRandomHarmony() const;
    float GenerateRandomVariable(unsigned var) const;
    float ModifyVariableTone(unsigned var, float old, float bandwidth) const;
private:
    unsigned m_vCount;
    HarmonyRanges const& m_ranges;
};

class ObjectiveFunction {
public:
    virtual float operator() (Harmony const& h) const = 0;
};

