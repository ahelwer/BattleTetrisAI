#include <core/harmony_search.hpp>
#include <cstdlib>
#include <algorithm>
#include <omp.h>

HarmonySearch::HarmonySearch(HarmonyCompareWrapper const& compare, HarmonyFactory const& factory,
                                unsigned varCount, unsigned memorySize, 
                                float r_accept, float r_pa, float b_range)
    : m_compare(compare), m_factory(factory), m_varCount(varCount), m_r_accept(r_accept), 
        m_r_pa(r_pa), m_b_range(b_range)
{
    for (unsigned i = 0; i < memorySize; ++i)
        m_memory.push_back(m_factory.GenerateRandomHarmony());
    std::sort(m_memory.begin(), m_memory.end(), m_compare);
}

HarmonySearch::~HarmonySearch() {
    for (unsigned i = 0; i < m_memory.size(); ++i) {
        if (m_memory.at(i) != NULL) {
            delete m_memory.at(i);
            m_memory.at(i) = NULL;
        }
    }
}

void HarmonySearch::InitializeHarmonies(std::vector<Harmony> const& init) {
    unsigned harmonyCount = std::min(m_memory.size(), init.size());
    for (unsigned i = 0; i < harmonyCount; ++i) {
        if (m_memory.at(i) != NULL) {
            delete m_memory.at(i);
            m_memory.at(i) = NULL;
        }
        m_memory.at(i) = new Harmony(init.at(i));
    }
}

void HarmonySearch::Iterate() {
    // Pre-calculates/caches objective function value in parallel
    ObjectiveFunction const& f = m_compare.GetObjectiveFunction();
    int harmonyCount = m_memory.size();
    #pragma omp parallel for
    for (int i = 0; i < harmonyCount; ++i) {
        f(*(m_memory.at(i))); 
    }

    // Sorts harmonies under objective function
    std::sort(m_memory.begin(), m_memory.end(), m_compare);

    Harmony* newHarmony = new Harmony();
    for (unsigned i = 0; i < m_varCount; ++i) {
        float r1 = (float)rand()/(float)RAND_MAX; 
        if (r1 < m_r_accept) { // Use memory
            float chosen = m_memory.at(rand() % m_memory.size())->at(i);
            float r2 = (float)rand()/(float)RAND_MAX;
            if (r2 < m_r_pa) { // Modify tone
                chosen = m_factory.ModifyVariableTone(i, chosen, m_b_range);
            }
            newHarmony->push_back(chosen);
        }
        else { // Generate new value
            newHarmony->push_back(m_factory.GenerateRandomVariable(i));
        }
    }

    // Compares new harmony to worst harmony
    Harmony const* worst = m_memory.back();
    if (m_compare(newHarmony, worst)) {
        delete worst;
        worst = NULL;
        m_memory.back() = newHarmony;
        std::sort(m_memory.begin(), m_memory.end(), m_compare);
    }
    else {
        delete newHarmony;
    }
}

void HarmonySearch::EraseHarmonyCaches() const {
    for (unsigned i = 0; i < m_memory.size(); ++i) {
        Harmony const* h = m_memory.at(i);
        h->EraseCache();
    }
}

Harmony const* HarmonySearch::GetRanked(unsigned rank) const {
    if (rank >= m_memory.size())
        return NULL;
    else 
        return new Harmony(*(m_memory.at(rank)));
}

