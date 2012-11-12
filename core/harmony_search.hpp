#pragma once

#include <util/vector.hpp>
#include <core/harmony.hpp>

class HarmonySearch {
public:
    HarmonySearch(ObjectiveFunction const& f, HarmonyFactory const& hf, unsigned vars,
                    unsigned memory, float r_accept, float r_pa, float b_range);
    ~HarmonySearch();
    void Iterate();
	Harmony const& GetRanked(unsigned rank);
private:
    ObjectiveFunction const& m_f;
    HarmonyFactory const& m_hf;
    HarmonyCompare const& m_hc;
    std::vector<Harmony*> m_memory;
    unsigned m_vars;
    float m_r_accept;
    float m_r_pa;
    float m_b_range;
};

