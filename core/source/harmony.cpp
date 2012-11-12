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

HarmonyCompare::HarmonyCompare(ObjectiveFunction const& f)
    : m_f(f)
{ }

bool HarmonyCompare::operator() (Harmony const* a, Harmony const* b) const {
    // Is Harmony a less than Harmony b? 
    //return (m_f(*a) <= m_f(*b));
	// Applies functions to Harmonies, returns cached values if available
	return (a->ApplyToSelf(m_f) <= b->ApplyToSelf(m_f));
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
    float r = low + (float)rand()/(float)RAND_MAX/(high-low);
    return r;
}

float HarmonyFactory::ModifyVariableTone(unsigned var, float old, float bandwidth) const {
    float low = m_ranges.at(var).first;
    float high = m_ranges.at(var).second;
    float r = (float)rand()/(float)RAND_MAX/2 - 1;
    float modified = old + bandwidth*r;
    modified = std::min(modified, high);
    modified = std::max(modified, low);
    return modified;
}

