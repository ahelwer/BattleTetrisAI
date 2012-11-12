#include <core/harmony_search.hpp>
#include <cstdlib>
#include <algorithm>

HarmonySearch::HarmonySearch(ObjectiveFunction const& f, HarmonyFactory const& hf,
                                unsigned vars, unsigned memory, float r_accept, 
                                float r_pa, float b_range)
    : m_f(f), m_hf(hf), m_hc(f), m_vars(vars), m_r_accept(r_accept), 
        m_r_pa(r_pa), m_b_range(b_range)
{
    for (unsigned i = 0; i < memory; ++i)
        m_memory.push_back(m_hf.GenerateRandomHarmony());
    std::sort(m_memory.begin(), m_memory.end(), m_hc);
}

HarmonySearch::~HarmonySearch() {
    for (unsigned i = 0; i < m_memory.size(); ++i) {
        if (m_memory[i] != NULL)
            delete m_memory[i];
    }
}

void HarmonySearch::Iterate() {
    Harmony* newHarmony = new Harmony();
    for (unsigned i = 0; i < m_vars; ++i) {
        float r1 = (float)rand()/(float)RAND_MAX; 
        if (r1 < m_r_accept) { // Use memory
            float chosen = m_memory.at(rand() % m_memory.size())->at(i);
            float r2 = (float)rand()/(float)RAND_MAX;
            if (r2 < m_r_pa) { // Modify tone
                chosen = m_hf.ModifyVariableTone(i, chosen, m_b_range);
            }
            newHarmony->push_back(chosen);
        }
        else { // Generate new value
            newHarmony->push_back(m_hf.GenerateRandomVariable(i));
        }
    }
    // Compares new harmony to worst harmony
    if (m_hc(newHarmony, m_memory.back())) {
        // Removes last element
        delete m_memory.back();
        m_memory.pop_back();
        m_memory.push_back(newHarmony);
		std::sort(m_memory.begin(), m_memory.end(), m_hc);
    }
}

Harmony const& HarmonySearch::GetRanked(unsigned rank) const {
	Harmony* ranked = m_memory.at(rank);
	return (*ranked);
}

