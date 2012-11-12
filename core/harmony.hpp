#pragma once

#include <algorithm>
#include <functional>
#include <utility>
#include <iostream>
#include <util/vector.hpp>

typedef std::vector< std::pair<float, float> > HarmonyRanges;
class ObjectiveFunction;

// Wrapper around std::vector which caches function calculations
class Harmony {
public:
	Harmony();
	float ApplyToSelf(ObjectiveFunction const& f) const;
	float const& operator[] (unsigned idx) const;
	float& operator[] (unsigned idx);
	float const& at(unsigned idx) const;
	void push_back(float val);	
	unsigned size() const;
	friend std::ostream& operator<< (std::ostream& out, Harmony const& h);
private:
	mutable bool m_isCached;
	mutable float m_cached;
	std::vector<float> m_data;
};

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
	~HarmonyFactory();
    virtual Harmony* GenerateRandomHarmony() const;
    virtual float GenerateRandomVariable(unsigned var) const;
    virtual float ModifyVariableTone(unsigned var, float old, float bandwidth) const;
protected:
    unsigned m_vCount;
    HarmonyRanges const& m_ranges;
};

class ObjectiveFunction {
public:
    virtual float operator() (Harmony const& h) const = 0;
};

