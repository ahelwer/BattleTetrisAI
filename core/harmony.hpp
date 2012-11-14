#pragma once

#include <algorithm>
#include <functional>
#include <utility>
#include <iostream>
#include <util/vector.hpp>

class ObjectiveFunction;
class Harmony;
typedef std::vector< std::pair<float, float> > HarmonyRanges;
typedef std::binary_function<Harmony*, Harmony*, bool> Comparator;

// Wrapper around std::vector which caches function calculations
class Harmony : public std::vector<float> {
public:
    Harmony();
    float ApplyToSelf(ObjectiveFunction const& f) const;
private:
    mutable bool m_isCached;
    mutable float m_cached;
};

class HarmonyFactory {
public:
    HarmonyFactory(unsigned decisionVarCount, HarmonyRanges const& ranges);
    virtual ~HarmonyFactory();
    virtual Harmony* GenerateRandomHarmony() const;
    virtual float GenerateRandomVariable(unsigned var) const;
    virtual float ModifyVariableTone(unsigned var, float old, float p) const;
protected:
    unsigned m_vCount;
    HarmonyRanges const& m_ranges;
};

class ObjectiveFunction {
public:
    virtual float operator() (Harmony const& h) const = 0;
};

class HarmonyCompare : public Comparator {
public:
    HarmonyCompare(ObjectiveFunction const& f);
    virtual ~HarmonyCompare();
    virtual bool operator() (Harmony const* a, Harmony const* b) const = 0;
protected:
    ObjectiveFunction const& m_f;
};

class HarmonyCompareMin : public HarmonyCompare {
public:
    HarmonyCompareMin(ObjectiveFunction const& f);
    bool operator() (Harmony const* a, Harmony const* b) const;
};

class HarmonyCompareMax : public HarmonyCompare {
public:
    HarmonyCompareMax(ObjectiveFunction const& f);
    bool operator() (Harmony const* a, Harmony const* b) const;
};

/* *
 * class HarmonyCompareWrapper
 *
 * Wrapper class which passes calls to operator() on to the 
 * HarmonyCompare given to its constructor. Necessary because 
 * std::sort does not take abstract type objects as a Comparator.
 * */
class HarmonyCompareWrapper : public Comparator {
public:
    HarmonyCompareWrapper(HarmonyCompare const& comp);
    bool operator() (Harmony const* a, Harmony const* b) const;
private:
    HarmonyCompare const& m_comp;
};

