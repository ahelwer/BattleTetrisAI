#pragma once

#include <util/vector.hpp>
#include <core/harmony.hpp>

/* *
 * Class HarmonySearch
 *
 * Performs a Harmony Search over the given objective function and range.
 * */
class HarmonySearch {
public:
    /* *
     * Parameters:
     *
     * compare - the order function on the harmonies
     * factory - generates and adjusts harmonies and weights
     * varCount - the number of weights in each harmony
     * memorySize - the number of harmonies to keep in memory
     * r_accept - p(using weight from memory) vs generating randomly
     * r_pa - p(adjusting weight) vs keeping it unchanged
     * b_range - percentage of range to adjust weight by
     * */
    HarmonySearch(HarmonyCompareWrapper const& compare, 
                    HarmonyFactory const& factory,
                    unsigned varCount, unsigned memorySize, 
                    float r_accept, float r_pa, float b_range);
    ~HarmonySearch();
    // Initialize values of harmonies; they are random otherwise
    void InitializeHarmonies(std::vector<Harmony> const& init);
    // Executes a single iteration of Harmony Search
    void Iterate();
    // Erases the cached objective function values in the harmonies
    void EraseHarmonyCaches() const;
    // Retrieves harmony of the supplied rank
    Harmony const* GetRanked(unsigned rank) const;
private:
    HarmonyCompareWrapper const& m_compare;
    HarmonyFactory const& m_factory;
    std::vector<Harmony const*> m_memory;
    unsigned m_varCount;
    float m_r_accept;
    float m_r_pa;
    float m_b_range;
};

