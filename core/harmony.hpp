#pragma once

#include <algorithm>
#include <functional>
#include <utility>
#include <iostream>
#include <util/vector.hpp>

class ObjectiveFunction;
class Harmony;
typedef std::vector< std::pair<float, float> > HarmonyRanges;
typedef std::binary_function<Harmony const*, Harmony const*, bool> Comparator;

/* *
 * Class Harmony
 *
 * Represents list of weight variables. Basically a wrapper around
 * an std::vector which caches applications of the objective function
 * to itself.
 * */
class Harmony : public std::vector<float> {
public:
    Harmony();
    // Applies function to itself, caching and returning result
    float ApplyToSelf(ObjectiveFunction const& f) const;
    // Erases cached value of objective function
    float EraseCache() const;
private:
    mutable bool m_isCached;
    mutable float m_cached;
};

/* *
 * Class HarmonyFactory
 *
 * Generates harmonies within provided range, generates random
 * individual weights, or adjusts individual weights.
 * */
class HarmonyFactory {
public:
    /* *
     * Parameters:
     *
     * decisionVarCount - number of weights in harmony
     * ranges - ranges each harmony weight is restricted to
     * */
    HarmonyFactory(unsigned decisionVarCount, HarmonyRanges const& ranges);
    virtual ~HarmonyFactory();
    // Generates purely random harmony within ranges
    virtual Harmony* GenerateRandomHarmony() const;
    // Generates random individual weight within range
    virtual float GenerateRandomVariable(unsigned var) const;
    // Adjusts individual weight within range
    virtual float ModifyVariableTone(unsigned var, float old, float p) const;
protected:
    unsigned m_vCount;
    HarmonyRanges const& m_ranges;
};

/* *
 * Class ObjectiveFunction
 *
 * Interface class, evaluates harmony for efficacy.
 * */
class ObjectiveFunction {
public:
    virtual float operator() (Harmony const& h) const = 0;
};

/* *
 * Class HarmonyCompare
 *
 * Interface class, used to order harmonies according to a
 * provided ObjectiveFunction.
 * */
class HarmonyCompare : public Comparator {
public:
    HarmonyCompare(ObjectiveFunction const& f);
    virtual ~HarmonyCompare();
    virtual bool operator() (Harmony const* a, Harmony const* b) const = 0;
    virtual ObjectiveFunction const& GetObjectiveFunction() const;
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
    virtual ObjectiveFunction const& GetObjectiveFunction() const;
private:
    HarmonyCompare const& m_comp;
};

