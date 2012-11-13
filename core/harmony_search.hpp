#pragma once

#include <util/vector.hpp>
#include <core/harmony.hpp>

class HarmonySearch {
public:
    HarmonySearch(HarmonyCompare const& compare, HarmonyFactory const& factory,
					unsigned varCount, unsigned memorySize, 
					float r_accept, float r_pa, float b_range);
    ~HarmonySearch();
    void Iterate();
	Harmony const* GetRanked(unsigned rank) const;
private:
    HarmonyCompare const& m_compare;
    HarmonyFactory const& m_factory;
    std::vector<Harmony*> m_memory;
    unsigned m_varCount;
    float m_r_accept;
    float m_r_pa;
    float m_b_range;
};

