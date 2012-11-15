#include <core/harmony.hpp>
#include <cstdlib>

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

float Harmony::EraseCache() const {
    m_isCached = false;
    float cached = m_cached;
    m_cached = 0.0;
    return cached;
}

HarmonyFactory::HarmonyFactory(unsigned decisionVarCount, HarmonyRanges const& ranges)
    : m_vCount(decisionVarCount), m_ranges(ranges)
{ }

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

float HarmonyFactory::ModifyVariableTone(unsigned var, float old, float p) const {
    float low = m_ranges.at(var).first;
    float high = m_ranges.at(var).second;
    float range = (high-low)*p;
    float r = low*p + ((float)rand()/(float)RAND_MAX)*range;
    float modified = old + r;
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

