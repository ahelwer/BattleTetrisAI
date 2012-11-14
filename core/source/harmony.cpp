#include <core/harmony.hpp>
#include <cstdlib>
//#include <random>

Harmony::Harmony()
    : std::vector<float>(), m_isCached(false), m_cached(0.0)
{ }

float Harmony::ApplyToSelf(ObjectiveFunction const& f) const {
    if (m_isCached)
        return m_cached;
    float result = f(*this);
    m_cached = result;
    m_isCached = true;
    return result;
}

HarmonyFactory::HarmonyFactory(unsigned decisionVarCount, HarmonyRanges const& ranges)
    : m_vCount(decisionVarCount), m_ranges(ranges)
{ 
    srand(time(NULL));
}

HarmonyFactory::~HarmonyFactory()
{ }

Harmony* HarmonyFactory::GenerateRandomHarmony() const {
    Harmony* randomHarmony = new Harmony();
    for (unsigned i = 0; i < m_vCount; ++i)
        randomHarmony->push_back(GenerateRandomVariable(i));
    return randomHarmony;
}

float HarmonyFactory::GenerateRandomVariable(unsigned var) const {
    float low = m_ranges.at(var).first;
    float high = m_ranges.at(var).second;
    float r = low + ((float)rand()/(float)RAND_MAX)*(high-low);
    return r;
}

float HarmonyFactory::ModifyVariableTone(unsigned var, float old, float bandwidth) const {
    float low = m_ranges.at(var).first;
    float high = m_ranges.at(var).second;
    float r = ((float)rand()/(float)RAND_MAX)*2 - 1;
    float modified = old + bandwidth*r;
    modified = std::min(modified, high);
    modified = std::max(modified, low);
    return modified;
}

HarmonyCompare::HarmonyCompare(ObjectiveFunction const& f)
    : m_f(f)
{ }

HarmonyCompare::~HarmonyCompare()
{ }

HarmonyCompareMin::HarmonyCompareMin(ObjectiveFunction const& f)
    : HarmonyCompare(f)
{ }

bool HarmonyCompareMin::operator() (Harmony const* a, Harmony const* b) const {
    // Is Harmony a less than Harmony b? 
    return (a->ApplyToSelf(HarmonyCompare::m_f) 
                < b->ApplyToSelf(HarmonyCompare::m_f));
}

HarmonyCompareMax::HarmonyCompareMax(ObjectiveFunction const& f)
    : HarmonyCompare(f)
{ }

bool HarmonyCompareMax::operator() (Harmony const* a, Harmony const* b) const {
    // Is Harmony a greater than Harmony b? 
    return (a->ApplyToSelf(HarmonyCompare::m_f) 
                > b->ApplyToSelf(HarmonyCompare::m_f));
}

HarmonyCompareWrapper::HarmonyCompareWrapper(HarmonyCompare const& comp)
    : m_comp(comp)
{ }

bool HarmonyCompareWrapper::operator() (Harmony const* a, Harmony const* b) const {
    return m_comp(a, b);
}

